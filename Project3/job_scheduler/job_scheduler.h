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

	// mutexes needed

}JobScheduler;

JobScheduler* initialize_scheduler(int execution_threads);
/*int submit_job(JobScheduler* sch, Job* j);
int execute_all_jobs(JobScheduler* sch);
int wait_all_tasks_finish(JobScheduler* sch);*/
void destroy_scheduler(JobScheduler* sch);

#endif