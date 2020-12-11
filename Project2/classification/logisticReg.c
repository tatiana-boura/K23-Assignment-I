#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// w[0] has b , w[1]..w[r] the weights

float innerPoduct(float* x_i, float* w, unsigned int r){

	float innerProduct = 0.0;
	for( unsigned int j=1; j<=r; j++ ){
		innerProduct += x_i[j]*w[j];
	}
	return innerProduct;
}

/* sigmoid function
   returns number in [0,1] */
float sigmoid(float t){
	return 1.0/(1 + exp(-t));
}

float hypothesis(float* x_i, float* w, unsigned int r){
  /* compute sigmod value for current tfidf vector (line)*/
  return sigmoid(innerPoduct( x_i, w, r) + w[0]);
}

void gradient_descent(float** x, float* y, float* w, unsigned int n, unsigned int r, float alpha){

  float _hypothesis_, error, sum_e, e_mul_x, sum_e_mul_x;
  
  for (unsigned int k = 0; k < 100; k++){
    
    // Clear variable
    _hypothesis_ = 0.0;
    error = 0.0;

    sum_e = 0.0;
    e_mul_x = 0.0;
    sum_e_mul_x = 0.0;
    
    for (unsigned int i = 0; i < n; i++){
	    // Calculate hypothesis
	    _hypothesis_ = hypothesis(x[i]);
	    // Calculate error
	    error = _hypothesis_ - y[i];
	    // Calculate sum of error
	    sum_e = sum_e + error;      
	    // Calculate error*x
	    e_mul_x = error * x[i];
	    // Calculate sum of error*x
	    sum_e_mul_x = sum_e_mul_x + e_mul_x;
    }
 
    // Calculate d_J/d_theta_0
    float d_J_d_theta_0 = sum_e / m;  
    // Calculate d_J/d_theta_1
    float d_J_d_theta_1 = sum_e_mul_x / m;
    
    // simultaneous update
    //for( unsigned int j=0; j<r; j++ ){
    	//update w's here here
    //}
    theta_0 = theta_0 - alpha*d_J_d_theta_0;    
    theta_1 = theta_1 - alpha*d_J_d_theta_1;
  }
}




int main(void){

	float* a = calloc(10,sizeof(float));
	float* b = calloc(10,sizeof(float));

	for( unsigned int i = 0; i<10; i++){
		a[i] = 1;
		b[i] = 1;
	}

	printf("%f\n", hypothesis(a, b, 10) );

	return 0;
}