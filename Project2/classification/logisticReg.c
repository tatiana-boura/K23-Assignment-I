#include "logisticReg.h"

float innerPoduct(float* x_i, float* w, unsigned int r){

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
	return sigmoid(innerPoduct( x_i, w, r) + bias);
}

void gradient_descent(float** x, float* y, float* w, float* bias, unsigned int m, unsigned int r, float eta, float epsilon){

	float _hypothesis_, sum_weights;
	float sum_bias = 0.0;

	float* J_weight = calloc(r,sizeof(float));
	float J_bias;
  
	for (unsigned int k = 0; k < 100; k++){
	
	    for( unsigned int j = 0; j < r+1; j++ ){

	    	sum_weights = 0.0;

		    for (unsigned int i = 0; i < m; i++){

			    _hypothesis_ = hypothesis(x[i], w, (*bias), r);
			    if( j!= r) // weights
			    	sum_weights += (_hypothesis_- y[i])*x[i][j];      
			    else	   // bias
			    	sum_bias += (_hypothesis_- y[i]);
		    }

		    J_weight[j] = sum_weights;
		    if( j==r ) J_bias = sum_bias;
		}  

		// J_weight/bias[j]/m ??????

		// simultaneous update
		// update bias:
		*bias = *bias - eta*J_bias;
		// update other weights
		for( unsigned int j = 0; j < r; j++ )
		    w[j] = w[j] - eta*J_weight[j];
		
	}

	for( unsigned int j=0; j<r; j++ ){
		free(J_weight); J_weight=NULL;
	}


	return;
}

