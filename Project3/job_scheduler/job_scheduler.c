#define BUFFSIZE 600

#include "job_scheduler.h"

typedef struct for_thread{ 
	pthread_mutex_t* mtx; //pointer to mutex
	pthread_cond_t* cond; //pointer to condition
}for_thread;


bool ncreated = false ;

void *threadFunction(void* args){
	printf("locking to mutex[%ld]\n", pthread_self() );

	pthread_mutex_lock(((for_thread*)args)->mtx);

	while(!ncreated){
		pthread_cond_wait(((for_thread*)args)->cond,((for_thread*)args)->mtx);
	}
	pthread_mutex_unlock(((for_thread*)args)->mtx);

	printf("I am thread with id [%ld]. Now I die?\n", pthread_self() );

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

	pthread_mutex_init(&(job_sch->mtx), NULL); pthread_cond_init(&(job_sch->cond), NULL);

	for_thread* args = calloc(1,sizeof(for_thread));
	args->mtx = &(job_sch->mtx);
	args->cond = &(job_sch->cond);
	

	pthread_mutex_lock(((for_thread*)args)->mtx); 
	for( unsigned int i=0; i <execution_threads; ++i){
		pthread_create(&(job_sch->tids[i]), NULL, threadFunction, (for_thread*)args );
	}
	ncreated=true;
	pthread_cond_broadcast(&(((JobScheduler*)job_sch)->cond)); //release the threads
	pthread_mutex_unlock(((for_thread*)args)->mtx); 

	job_sch->cb = circBufferInit(BUFFSIZE);

	//wait for all the thread to terminate
	for( unsigned int i=0; i <execution_threads; ++i){
		pthread_join(job_sch->tids[i], NULL);
	}

	free(args);
	return job_sch;
}

int submit_job(JobScheduler* sch, Batch* j){



}

/*int execute_all_jobs(JobScheduler* sch){}
int wait_all_tasks_finish(JobScheduler* sch){}*/

// destroy job sceduler
void destroy_scheduler(JobScheduler* sch){
	//destroy mutex & cond
	pthread_mutex_destroy(&(sch->mtx)); pthread_cond_destroy(&(sch->cond));
	
	free(sch->tids); sch->tids = NULL;
	circBufferDealloc(sch->cb);
	free(sch); sch=NULL;

	return;
}