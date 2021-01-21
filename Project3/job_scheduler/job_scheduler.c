#define BUFFSIZE 600

#include "job_scheduler.h"

void *threadFunction(){

	printf("Hello I am thread with id [%ld]. Now I die?\n", pthread_self() );
	pthread_exit(NULL);

	return NULL;

}

// initilize job scheduler
JobScheduler* initialize_scheduler(int execution_threads){

	// initialize scheduler struct
	JobScheduler* job_sch = calloc(1,sizeof(JobScheduler)); assert(job_sch!=NULL);
	// store number
	job_sch->execution_threads = execution_threads;
	// making threads
	job_sch->tids = calloc(execution_threads,sizeof(pthread_t)); assert( job_sch->tids != NULL );
	for( unsigned int i=0; i <execution_threads; ++i){
		pthread_create(&(job_sch->tids[i]), NULL, threadFunction, NULL );
	}

	job_sch->cb = circBufferInit(BUFFSIZE);

	return job_sch;

}

/*int submit_job(JobScheduler* sch, Job* j){}
int execute_all_jobs(JobScheduler* sch){}
int wait_all_tasks_finish(JobScheduler* sch){}*/

// destroy job sceduler
void destroy_scheduler(JobScheduler* sch){

	free(sch->tids); sch->tids = NULL;
	circBufferDealloc(sch->cb);
	free(sch); sch=NULL;

	return;
}