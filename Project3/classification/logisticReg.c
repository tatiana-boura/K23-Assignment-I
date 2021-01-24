#include "logisticReg.h"

#define NUM_OF_THREADS 10
#define BATCH_SIZE 1024
#define EPOCH_NUM 5

float innerProduct(float* x_i, float* w, unsigned int r){

	float innerProduct = 0.0;
	for( unsigned int j=0; j<r; j++ ){
		innerProduct += x_i[j]*w[j];
	}
	return innerProduct;
}

/* sigmoid function
   returns number in [0,1] */
float sigmoid(float t){
	return 1.0/(1 + exp(-t));
}

float hypothesis(float* x_i, float* w, float bias, unsigned int r){
	/* compute sigmod value for current vector (line)*/
	return sigmoid(innerProduct( x_i, w, r) + bias);
}

bool* predict( float** x_valid, unsigned int* y_valid, float* w, float bias, unsigned int n, unsigned int r ){
	// predict class of x_valid
	/* return a boolean array where: 
	   true -- classified correctly, false -- not classified correctly */
	bool* correctClass = calloc(n,sizeof(bool)); assert( correctClass != NULL );

	for( unsigned int i=0; i<n; i++ ){

		bool class;

		if( hypothesis(x_valid[i], w, bias, r) <= 0.5 ) class = 0;
		else class = 1;

		correctClass[i] = (class == y_valid[i]);
	}
	return correctClass;
}

float* predict_proba( float** x_valid, float* w, float bias, unsigned int n, unsigned int r ){
	// predict class of x_valid
	/* return a probability array */
	float* probabilities = calloc(n,sizeof(float)); assert( probabilities != NULL );

	for( unsigned int i=0; i<n; i++ ){
		probabilities[i] = hypothesis(x_valid[i], w, bias, r);
	}
	return probabilities;
}

void gradient_descent(float** x_train, unsigned int* y_train, float** w, float* bias, unsigned int n, unsigned int r, float eta, float epsilon){
	
	/* Batch Gradient Descent where every thread deals with a batch */ 

	// train the model
	float* J_weight = calloc(r,sizeof(float)); assert(J_weight!=NULL);
	float J_bias;

  	// do EPOCH_NUM of epochs
	for( unsigned int epoch = 0; epoch < EPOCH_NUM; epoch++ ){
		//initialize scheduler --> create threads
		JobScheduler* js = initialize_scheduler(NUM_OF_THREADS);
		node* all_thread_results = NULL;

		J_bias = 0.0;

		printf("%d' epoch\n",epoch+1 );

		//create and submit jobs for threads
		unsigned int num_of_batches = 0;
		for(unsigned int b=0; b<n ; b+=BATCH_SIZE ){

			Job* job = calloc(1,sizeof(Job));
			Batch* batch=calloc(1,sizeof(batch));
			//Batch batch;
			batch->start = b;
			if( b+BATCH_SIZE < n ) batch->end = b+BATCH_SIZE; 
			else batch->end = n;

			create_job(job, &all_thread_results,x_train, y_train, *w, *bias, r, batch);
			submit_job(js,(void*)job); //1st submit wakes thread
			// count number of batches
			num_of_batches++;
		}	
		//wait till all the threads return
		js->last_job = true;
		wait_all_tasks_finish(js);

		// use the mean of all res->J_weight
		node* temp = all_thread_results;
		while(temp){
			J_thread_results* res = (J_thread_results*)temp->data;
			for(unsigned int j=0 ; j<r ; j++){
				J_weight[j] += res->J_weight[j];		
			}	
			J_bias += res->J_bias;
			temp = temp->next;
		}
		
		// simultaneous update -- because it is more efficient
		// update weights after the return of the threads
		*bias = *bias - eta*(J_bias/(float)num_of_batches);
		for( unsigned int j = 0; j < r; j++ )
		    (*w)[j] = (*w)[j] - eta*(J_weight[j]/(float)num_of_batches);
		
		destroyListOfWordInfo(all_thread_results, (void*)destroy_res);
		destroy_scheduler(js);
	}
	// free not needed memo
	free(J_weight); J_weight=NULL;

	return;
}