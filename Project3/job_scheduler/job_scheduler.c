#define BUFFSIZE 600

#include "job_scheduler.h"

typedef struct for_thread{ 
	circBuffer** cb;
	pthread_mutex_t* mtx; //pointer to mutex
	pthread_cond_t* cond; //pointer to condition
	pthread_mutex_t* wrt;
	pthread_mutex_t* rd;
}for_thread;

unsigned int active_readers = 0;
bool ncreated = false ;

void *threadFunction(void* args){
	pthread_mutex_lock(((for_thread*)args)->mtx);

	while(!ncreated){
		pthread_cond_wait((((for_thread*)args)->cond),((for_thread*)args)->mtx);
	}
	pthread_mutex_unlock(((for_thread*)args)->mtx);
	pthread_cond_signal(((for_thread*)args)->cond);

	//read job from buffer
	while(1){
		pthread_mutex_lock(((for_thread*)args)->mtx); 
			// if buffer is not empty -- read if it is your turn
		if(!circBufferEmpty(*(((for_thread*)args)->cb))){
			pthread_mutex_lock(((for_thread*)args)->rd);
			printf("Reading [%ld]\n",pthread_self());
			active_readers++;
			if(active_readers==1){ // only one reads from buff
				pthread_mutex_lock(((for_thread*)args)->wrt);
				Batch _data_;
				bool popped = circBufferPop(*(((for_thread*)args)->cb), &_data_);
			}
			pthread_mutex_unlock(((for_thread*)args)->rd);
			pthread_mutex_lock(((for_thread*)args)->rd);
			active_readers--;
			if(active_readers==0) // now start the writer
				pthread_mutex_unlock(((for_thread*)args)->wrt);
			pthread_mutex_unlock(((for_thread*)args)->rd);
			pthread_mutex_unlock(((for_thread*)args)->mtx); 
		
		}else{ // if buffer empty -- job done
			pthread_mutex_unlock(((for_thread*)args)->mtx);
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
	job_sch->cb = circBufferInit(BUFFSIZE);

	// create arg array
	for_thread* args = calloc(1,sizeof(for_thread));
	args->mtx = &(job_sch->mtx);
	args->cond = &(job_sch->cond);
	args->rd = &(job_sch->rd);
	args->wrt = &(job_sch->wrt);
	//args->cb = calloc(1,sizeof(circBuffer));
	args->cb = &(job_sch->cb);
	
	// create threads and then broadcast
	pthread_mutex_lock(((for_thread*)args)->mtx); 
	for( unsigned int i=0; i <execution_threads; ++i){
		pthread_create(&(job_sch->tids[i]), NULL, threadFunction, (for_thread*)args );
	}
	ncreated=true; // should be here
	//pthread_cond_broadcast(&(((JobScheduler*)job_sch)->cond)); //release the threads
	pthread_mutex_unlock(((for_thread*)args)->mtx); 

	

	//wait for all the thread to terminate
	/*for( unsigned int i=0; i <execution_threads; ++i){
		pthread_join(job_sch->tids[i], NULL);
	}*/

	//free(args); 
	return job_sch;
}

// insert job into circ-buffer
int submit_job(JobScheduler* sch, Batch* data){
	// insert in circ buffer.
	// the buffer itself checks if its full
	// and does not insert if so

    pthread_mutex_lock(&(sch->wrt));
    circBufferInsert(sch->cb,*data);
    pthread_mutex_unlock(&(sch->wrt));

	//return circBufferInsert(sch->cb,data);
	return 1;
}

int execute_all_jobs(JobScheduler* sch){

	//pthread_mutex_lock(&(sch->mtx)); 
	//pthread_cond_broadcast(&(((JobScheduler*)sch)->cond)); //release the threads
	pthread_cond_signal(&(((JobScheduler*)sch)->cond));
	//pthread_mutex_unlock(&(sch->mtx)); 

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
	circBufferDealloc(sch->cb);
	free(sch); sch=NULL;

	return;
}