#ifndef _JOB_SCH_
#define _JOB_SCH_

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

#include "circ_buffer.h"

typedef struct JobScheduler{

	int execution_threads;
	//Queue* q;
	circBuffer* cb;
	pthread_t* tids;

	pthread_mutex_t mtx;
	pthread_cond_t cond;

}JobScheduler;

JobScheduler* initialize_scheduler(int execution_threads);
int submit_job(JobScheduler* sch, Batch* j);
/*int execute_all_jobs(JobScheduler* sch);
int wait_all_tasks_finish(JobScheduler* sch);*/
void destroy_scheduler(JobScheduler* sch);

#endif