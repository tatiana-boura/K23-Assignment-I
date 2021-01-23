#ifndef _JOB_SCH_
#define _JOB_SCH_

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>

#include "job.h"

typedef node* Queue;

typedef struct JobScheduler{

	int execution_threads;
	Queue q;
	pthread_t* tids;

	pthread_mutex_t mtx;
	pthread_mutex_t wrt;
	pthread_mutex_t rd;
	pthread_mutex_t res_ins;
	pthread_cond_t cond;

	bool last_job;

}JobScheduler;

JobScheduler* initialize_scheduler(int execution_threads);
int submit_job(JobScheduler* sch, void* data);
//int execute_all_jobs(JobScheduler* sch);
int wait_all_tasks_finish(JobScheduler* sch);
void destroy_scheduler(JobScheduler* sch);

#endif