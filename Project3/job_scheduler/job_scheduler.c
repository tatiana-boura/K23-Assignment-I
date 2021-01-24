#define BUFFSIZE 600

#include "job_scheduler.h"

unsigned int active_readers = 0;
bool empty_queue = true ;

void *threadFunction(void* args){
	// get thread arguments
	JobScheduler* job_sch = (JobScheduler*)args;

	pthread_mutex_lock(&(job_sch->mtx));
	// while there in no job sumbitted
	while(empty_queue){
		pthread_cond_wait(&(job_sch->cond),&(job_sch->mtx));
	}
	pthread_mutex_unlock(&(job_sch->mtx));
	pthread_cond_signal(&(job_sch->cond));

	//read job from queue
	while(1){

		pthread_mutex_lock(&(job_sch->mtx));
		node* _job_=NULL;

		// if queue is not empty -- read if it is your turn
		if(job_sch->q){

			pthread_mutex_lock(&(job_sch->rd));
			active_readers++;
			if(active_readers==1){ // only one reads from queue	
				pthread_mutex_lock(&(job_sch->wrt));
				// this thread gets a job
				_job_ = pop(&(job_sch->q));

			}
			pthread_mutex_unlock(&(job_sch->rd));
			
			pthread_mutex_lock(&(job_sch->rd));
			active_readers--;
			// when no one reads -- start the writer
			if(active_readers==0){ 
				pthread_mutex_unlock(&(job_sch->wrt));
			} 
			pthread_mutex_unlock(&(job_sch->rd));
		}
		
		pthread_mutex_unlock(&(job_sch->mtx));

		// if this thread got a job
		if(_job_){
			((Job*)_job_->data)->batch_training = job_batch_training;
			// do the batch training
			J_thread_results* result= ((Job*)_job_->data)->batch_training(((Job*)_job_->data)->x_train,((Job*)_job_->data)->y_train,((Job*)_job_->data)->w,((Job*)_job_->data)->bias,((Job*)_job_->data)->r,((Job*)_job_->data)->batch);
			// append the result to the global list -- in order to process later
			pthread_mutex_lock(&(job_sch->res_ins));
			*(((Job*)_job_->data)->all_thread_results) = appendList(*(((Job*)_job_->data)->all_thread_results),(J_thread_results*)result);
			pthread_mutex_unlock(&(job_sch->res_ins));

			// destroy job
			free(_job_->data); _job_->data=NULL;
			free(_job_); _job_=NULL;
		}
			
		if(job_sch->last_job && !(job_sch->q)){ // if queue empty -- job done
			break;
		}
	}

	pthread_exit(NULL);
	return NULL;


}

// initialize job scheduler
JobScheduler* initialize_scheduler(int execution_threads){

	// initialize scheduler struct
	JobScheduler* job_sch = calloc(1,sizeof(JobScheduler)); assert(job_sch!=NULL);
	// store number
	job_sch->execution_threads = execution_threads;
	// making threads
	job_sch->tids = calloc(execution_threads,sizeof(pthread_t)); assert( job_sch->tids != NULL );

	// initialize mutexes and conditions
	pthread_mutex_init(&(job_sch->mtx), NULL); pthread_cond_init(&(job_sch->cond), NULL);
	pthread_mutex_init(&(job_sch->wrt), NULL); pthread_mutex_init(&(job_sch->rd), NULL);
	pthread_mutex_init(&(job_sch->res_ins), NULL);

	job_sch->last_job = false;
	// initialize queue
	job_sch->q = NULL;

	// create threads
	pthread_mutex_lock(&(((JobScheduler*)job_sch)->mtx)); //release the threads
	for( unsigned int i=0; i <execution_threads; ++i){
		pthread_create(&(job_sch->tids[i]), NULL, threadFunction, (JobScheduler*)job_sch );
	}
	pthread_mutex_unlock(&(((JobScheduler*)job_sch)->mtx)); //release the threads
	
	return job_sch;
}

// insert job into queue
int submit_job(JobScheduler* sch, void* data){

    pthread_mutex_lock(&(sch->wrt));
    sch->q = appendListEnd(sch->q,data);
	empty_queue=false; // should be here 
    pthread_mutex_unlock(&(sch->wrt));

	return 1;
}

/*int execute_all_jobs(JobScheduler* sch){
	return 1;
}*/

int wait_all_tasks_finish(JobScheduler* sch){
	//wait for all the threads to terminate
	for( unsigned int i=0; i <sch->execution_threads; ++i){
		pthread_join(sch->tids[i], NULL);
	}
	return 1;
}

// destroy job sceduler
void destroy_scheduler(JobScheduler* sch){
	//destroy mutex & cond
	pthread_mutex_destroy(&(sch->mtx)); pthread_cond_destroy(&(sch->cond));
	pthread_mutex_destroy(&(sch->wrt)); pthread_mutex_destroy(&(sch->rd));
	pthread_mutex_destroy(&(sch->res_ins));
	// destroy list of thread id's
	free(sch->tids); sch->tids = NULL;
	destroyListOfStrings(sch->q, false );
	free(sch); sch=NULL;

	return;
}