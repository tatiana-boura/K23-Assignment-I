#include "logisticReg.h"

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
	/* compute sigmod value for current tfidf vector (line)*/
	return sigmoid(innerProduct( x_i, w, r) + bias);
}

bool* predict( float** x_test, float* y_test, float* w, float bias, unsigned int m, unsigned int r ){
	// predict class of x_test
	/* return a boolean array where: 
	   true -- classified correctly, false -- not classified correctly */

	bool* correctClass = calloc(m,sizeof(bool)); assert( correctClass != NULL );

	for( unsigned int i=0; i<m; i++ ){
		correctClass[i] = (hypothesis(x_test[i], w, bias, r) == y_test[i]);
	}

	return correctClass;
}

void gradient_descent(float** x_train, float* y_train, float* w, float* bias, unsigned int m, unsigned int r, float eta, float epsilon){
	// train the model
	float _hypothesis_, sum_weights;
	float sum_bias;

	float* J_weight = calloc(r,sizeof(float)); assert(J_weight!=NULL);
	float J_bias;
  
  	// this sould be changed -- needed the abs(w_new - w_prev) < epsilon
	for (unsigned int k = 0; k < 100; k++){
		// for every w_j
		sum_bias = 0.0;
	    for( unsigned int j = 0; j < r+1; j++ ){

	    	sum_weights = 0.0;
	    	// for every observation
		    for (unsigned int i = 0; i < m; i++){
		    	// compute the predicted value
			    _hypothesis_ = hypothesis(x_train[i], w, (*bias), r);
			    // update d/dw_j J(w,b) or bias
			    if( j!= r) // weights
			    	sum_weights += (_hypothesis_- y_train[i])*x_train[i][j];      
			    else	   // bias
			    	sum_bias += (_hypothesis_- y_train[i]);
		    }

		    // J_weight/bias[j]/m ?????? 
		    // I know that the /m is needed but they don't tell us so
		    J_weight[j] = sum_weights;
		    if( j==r ) J_bias = sum_bias;
		}  

		// simultaneous update -- beceause it is more efficient
		// update bias:
		*bias = *bias - eta*J_bias;
		// update other weights
		for( unsigned int j = 0; j < r; j++ )
		    w[j] = w[j] - eta*J_weight[j];
		
	}

	// free not needed memo
	for( unsigned int j=0; j<r; j++ ){
		free(J_weight); J_weight=NULL;
	}

	return;
}

