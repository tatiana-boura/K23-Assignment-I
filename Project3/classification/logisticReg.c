#include "logisticReg.h"

#include <time.h>

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

void gradient_descent(float** x_train, unsigned int* y_train, float* w, float* bias, unsigned int n, unsigned int r, float eta, float epsilon){
	
	/* Note: At this point we use Stochastic Gradient Descent if you want to use non-stochastic 
	and non-batch Gradient Descent uncomment the commented lines under the :___ GRADIENT DESCENT __*/ 

	// train the model
	float _hypothesis_, sum_weights;
	float sum_bias;

	float* J_weight = calloc(r,sizeof(float)); assert(J_weight!=NULL);
	float J_bias;

	bool stopTraining=false;

	srand(time(NULL));
  
  	// norm(w_new - w_prev) < epsilon
	while( stopTraining == false ){
	//for (unsigned int k = 0; k < 10; k++){
		// for every w_j
		sum_bias = 0.0;

		
		
	    for( unsigned int j = 0; j < r+1; j++ ){

	    	sum_weights = 0.0;

	    	/*____ STOCHASTIC GRADIENT ______
	    	  ____(get random line of x)_____ */
	    	/*unsigned int i = rand()%n;
	    	// compute the predicted value
		    _hypothesis_ = hypothesis(x_train[i], w, (*bias), r);
		    // update d/dw_j J(w,b) or bias
		    if( j!= r) // weights
		    	sum_weights += (_hypothesis_- (float)y_train[i])*x_train[i][j];      
		    else	   // bias
		    	sum_bias += (_hypothesis_- (float)y_train[i]);*/

		    //____ GRADIENT DESCENT __________
			for (unsigned int i = 0; i < n; i++){
			
		    	// compute the predicted value
			    _hypothesis_ = hypothesis(x_train[i], w, (*bias), r);
			    // update d/dw_j J(w,b) or bias
			    if( j!= r) // weights
			    	sum_weights += (_hypothesis_- (float)y_train[i])*x_train[i][j];      
			    else	   // bias
			    	sum_bias += (_hypothesis_- (float)y_train[i]);
		    }

		    if( j==r ) J_bias = sum_bias;
		    else J_weight[j] = sum_weights;
			
		}  

		// simultaneous update -- because it is more efficient
		// update bias:
		*bias = *bias - eta*J_bias;
		// update other weights
		float weightOld;
		// if norm(w_new - w_prev) < epsilon the training will stop
		stopTraining = true;

		for( unsigned int j = 0; j < r; j++ ){
			// keep old weight j
			weightOld = w[j];
			// update new weight j
		    w[j] = w[j] - eta*J_weight[j];

		    if( abs(w[j] - weightOld) > epsilon ){
		    	// some weights need more training
		    	stopTraining = false;  
		    }
		}
		
	}

	// free not needed memo
	free(J_weight); J_weight=NULL;

	return;
}

