#define BUFFSIZE 600

#include "job_scheduler.h"

unsigned int active_readers = 0;
bool ncreated = false ;

void *threadFunction(void* args){

	JobScheduler* job_sch = (JobScheduler*)args;

	pthread_mutex_lock(&(job_sch->mtx));

	while(!ncreated){
		pthread_cond_wait(&(job_sch->cond),&(job_sch->mtx));
	}
	pthread_mutex_unlock(&(job_sch->mtx));
	pthread_cond_signal(&(job_sch->cond));

	//read job from buffer
	while(1){
		pthread_mutex_lock(&(job_sch->mtx)); 
		
		// if buffer is not empty -- read if it is your turn
		if(job_sch->q){
		//if(!circBufferEmpty(job_sch->cb)){

			//bool popped=false;
			node* _data_=NULL;
		
			pthread_mutex_lock(&(job_sch->rd));
			
			printf("Reading [%ld]\n",pthread_self());
			active_readers++;
			
			if(active_readers==1){ // only one reads from buff	
				pthread_mutex_lock(&(job_sch->wrt));
				//popped = circBufferPop(job_sch->cb, _data_);
				//popped=true;
				_data_ = pop(&(job_sch->q));
			}
			pthread_mutex_unlock(&(job_sch->rd));
			pthread_mutex_lock(&(job_sch->rd));
			active_readers--;
			
			if(active_readers==0){ // now start the writer
				pthread_mutex_unlock(&(job_sch->wrt));
			} 
			pthread_mutex_unlock(&(job_sch->rd));
			pthread_mutex_unlock(&(job_sch->mtx)); 

			//if(popped){
				printf("Thread [%ld] got data: [%d-%d]\n",pthread_self(),((Batch*)_data_->data)->start, ((Batch*)_data_->data)->end );
				//free(_data_);_data_=NULL;
			//}
		
		}else{ // if buffer empty -- job done
			pthread_mutex_unlock(&(job_sch->mtx));
			printf("Leaving [%ld]\n",pthread_self());
			break;
		}
	}


	/*while( popped==false){
		// read one by one from the buffer
		pthread_mutex_lock(&readFromBuffer);
		popped = circBufferPop(cbuf, &_data_);
		pthread_mutex_unlock(&readFromBuffer);

		if( popped==false ){
			// no entries in the array
			pthread_mutex_lock(&bufferIsFull);
		    pthread_cond_wait(&cond, &bufferIsFull);
		    pthread_mutex_unlock(&bufferIsFull);
		}
	}*/

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

	// initialize circ-buffer
	job_sch->q = NULL;
	//job_sch->cb = circBufferInit(BUFFSIZE);

	// create threads
	pthread_mutex_lock(&(((JobScheduler*)job_sch)->mtx)); //release the threads

	for( unsigned int i=0; i <execution_threads; ++i){
		pthread_create(&(job_sch->tids[i]), NULL, threadFunction, (JobScheduler*)job_sch );
	}
	ncreated=true; // should be here
	
	pthread_mutex_unlock(&(((JobScheduler*)job_sch)->mtx)); //release the threads
	
	return job_sch;
}

// insert job into circ-buffer
int submit_job(JobScheduler* sch, Batch* data){

    pthread_mutex_lock(&(sch->wrt));
    sch->q = appendListEnd(sch->q,data);
    //circBufferInsert(sch->cb,*data);
    pthread_mutex_unlock(&(sch->wrt));

	return 1;
}

int execute_all_jobs(JobScheduler* sch){

	pthread_cond_signal(&(((JobScheduler*)sch)->cond));

	return 1;

}

int wait_all_tasks_finish(JobScheduler* sch){
	//wait for all the thread to terminate
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
	free(sch->tids); sch->tids = NULL;
	//circBufferDealloc(sch->cb);
	destroyListOfStrings(sch->q, true );
	free(sch); sch=NULL;

	return;
}