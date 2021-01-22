#ifndef _JOB_SCH_
#define _JOB_SCH_

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>

#include "circ_buffer.h"
#include "../list/list.h"

typedef node* Queue;

typedef struct JobScheduler{

	int execution_threads;
	Queue q;
	//circBuffer* cb;
	pthread_t* tids;

	pthread_mutex_t mtx;
	pthread_mutex_t wrt;
	pthread_mutex_t rd;
	pthread_cond_t cond;

}JobScheduler;

JobScheduler* initialize_scheduler(int execution_threads);
int submit_job(JobScheduler* sch, Batch* data, bool sendSig);
int execute_all_jobs(JobScheduler* sch);
int wait_all_tasks_finish(JobScheduler* sch);
void destroy_scheduler(JobScheduler* sch);

#endif