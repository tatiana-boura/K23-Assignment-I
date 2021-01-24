/*Tester for threads - to test the correctness of the method 
used in job_scheduler.c and logisticReg.c with a simplified 
struct for Jobs and actions to be done by threads 
(they just append a list with the int given by the job assigned)*/

#include "../../Project1/unit_testing/acutest.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include "../list/list.h"

/*Change NUM_OF_THREADS or NUM_OF_JOBS to check that threadFunction 
works for all cases: less, equal or more threads than jobs*/
#define NUM_OF_THREADS 10
#define NUM_OF_JOBS 5

bool empty_queue = true; 
unsigned int active_readers = 0;

/*Counters to check if the correct amount
of threads is created and exited*/
unsigned int number_created = 0;
unsigned int number_exited = 0;

/*Queue for Jobs*/
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

/*Simplified job struct*/
typedef struct Job{
	node** all_thread_results; 
    int number;
}Job;

void* threadFunction(void* args);
void create_job(Job* new_job, node** all_thread_results, unsigned int number);
JobScheduler* initialize_scheduler(int execution_threads);
int submit_job(JobScheduler* sch, void* job);
int wait_all_tasks_finish(JobScheduler* sch);
void destroy_scheduler(JobScheduler* sch);

void test_threads(void){
    JobScheduler* js = initialize_scheduler(NUM_OF_THREADS);
    node* all_thread_results = NULL;

    /*Create a table that holds all the jobs
    i.e. each item is its position (input_jobs[2]==2)*/
    unsigned int input_jobs[NUM_OF_JOBS];
    for(unsigned int i=0; i<NUM_OF_JOBS;i++){
        input_jobs[i] = i;
        Job* job = calloc(1,sizeof(Job));
        create_job(job,&all_thread_results,i);
        submit_job(js,job);
    }
    // Let the threads know that all jobs have been sumbitted
    js->last_job = true;

    wait_all_tasks_finish(js);

    // Check if the list that the threads need to append isn't empty
    TEST_ASSERT(all_thread_results!=NULL);
    // Check if the correct amount of threads was created and exited
    TEST_ASSERT(number_created==NUM_OF_THREADS);
    TEST_ASSERT(number_created==number_exited);

    for(unsigned int i=0; i<NUM_OF_JOBS;i++){
        /*Check if each job assigned has been processed by 
        a thread (found in list of results)*/
        bool found = false;
        node* temp = all_thread_results;
        printf("In list\n");
        while(temp!=NULL){
            if(((unsigned int)(temp->data)) == input_jobs[i])
                found = true;
            temp = temp->next;
        }
        TEST_ASSERT(found);
    }

    destroyList(all_thread_results,NULL,false,NULL);

    destroy_scheduler(js);
    return;
}

void* threadFunction(void* args){
    //Get thread argument
	JobScheduler* job_sch = (JobScheduler*)args;

    number_created++;
	pthread_mutex_lock(&(job_sch->mtx));
	// while there is no job sumbitted
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
            // Simplified action by thread: get job and append list with the number included (to check that job data is collected correctly)
			pthread_mutex_lock(&(job_sch->res_ins));
            *(((Job*)_job_->data)->all_thread_results) = appendList(*(((Job*)_job_->data)->all_thread_results),(void*)(((Job*)(_job_->data))->number));
			pthread_mutex_unlock(&(job_sch->res_ins));

			// destroy job
			free(_job_->data); _job_->data=NULL;
			free(_job_); _job_=NULL;
		}
			
		if(job_sch->last_job && !(job_sch->q)){ // if queue empty -- job done
			break;
		}
	}
    number_exited++;
	pthread_exit(NULL);
	return NULL;

}

void create_job(Job* new_job, node** all_thread_results, unsigned int number){
    new_job->all_thread_results = all_thread_results;
    new_job->number = number;
    return;
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
int submit_job(JobScheduler* sch, void* job){
    pthread_mutex_lock(&(sch->wrt));
    sch->q = appendListEnd(sch->q,job);
	empty_queue=false;
    pthread_mutex_unlock(&(sch->wrt));
	return 1;
}

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

TEST_LIST = {
	{ "threads", test_threads },
	{ NULL, NULL } // end of tests
};