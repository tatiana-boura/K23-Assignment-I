#ifndef _JOB_H_
#define _JOB_H_

#include "../list/list.h"


typedef struct J_thread_results{
    Batch* batch;
	float* J_weight;
	float J_bias;
}J_thread_results;

typedef struct Batch{
	unsigned int start;
	unsigned int end;
}Batch;

typedef struct Job{
	node* all_thread_results; 
	J_thread_results* (*batch_training)(float**, unsigned int*, float*, float, unsigned int, Batch*);
	float** x_train;
	unsigned int* y_train;
	float* w;
	float bias; 
	unsigned int r;
	Batch* batch;
}Job;

void create_job(Job* new_job, node** all_thread_results, float** x_train, unsigned int* y_train, float* w, float bias, unsigned int r, Batch* batch);
J_thread_results* job_batch_training(float** x_train, unsigned int* y_train, float* w, float bias, unsigned int r, Batch* batch);
#endif 