#include "job.h"

void create_job(Job* new_job, node** all_thread_results,float** x_train, unsigned int* y_train, float* w, float bias, unsigned int r, Batch* batch){
   
    new_job->all_thread_results = all_thread_results;
    
    new_job->x_train = x_train;
    new_job->y_train = y_train;
    new_job->w = w;
    new_job->bias = bias;
    new_job->r = r;
    new_job->batch = batch;

    return;
}

void destroy_res(J_thread_results* res){

	free(res->batch); res->batch=NULL;
	free(res->J_weight); res->J_weight=NULL;

	free(res); res=NULL;

	return;
}

J_thread_results* job_batch_training(float** x_train, unsigned int* y_train, float* w, float bias, unsigned int r, Batch* batch){
	//Create struct to store result of batch training (batch,J_weights[],J_bias)
	J_thread_results* res = calloc(1,sizeof(J_thread_results));
	res->batch = batch;
	res->J_bias = 0.0;
	res->J_weight = calloc(r,sizeof(float)); assert(res->J_weight!=NULL);
	
	float sum_bias = 0.0;
	for( unsigned int j = 0; j < r+1; j++ ){

		float sum_weights = 0.0;

		//____ BATCH GRADIENT DESCENT __________
		for (unsigned int i = batch->start; i < batch->end; i++){
		
			// compute the predicted value
			float _hypothesis_ = hypothesis(x_train[i], w, bias, r);
			// update d/dw_j J(w,b) or bias
			if( j!= r) // weights
				sum_weights += (_hypothesis_- (float)y_train[i])*x_train[i][j];      
			else	   // bias
				sum_bias += (_hypothesis_- (float)y_train[i]);
		}

		if( j==r ) res->J_bias = sum_bias;
		else (res->J_weight)[j] = sum_weights;
		
	}
	return res;  
}