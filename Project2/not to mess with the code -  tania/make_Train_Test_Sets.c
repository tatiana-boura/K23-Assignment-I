#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define COLUMNSIZE 5


void createSets( float** total_set, unsigned int* total_y, unsigned int total_size ,float*** train_set, unsigned int* n, float*** test_set, unsigned int* m, unsigned int** y_train, unsigned int** y_test ){
	/* total_set  : whole set that will be split
	   total_y    : labels for whole set
	   total_size : size of whole set
	   train_set  : training set to be returned
	   n		  : size of training set
	   test_set   : test set to be retuned
	   m          : size of test set            
	   y_train    : labels for training set to be returned
	   y_test     : labels for test set to be returned */	
		
	// the training size will be the 80% of the initial	
	*n = 0.8 * total_size;
	// the test size is the rest	
	*m = total_size - *n;
	// initialize memo for train and test
	(*train_set) = calloc(*n, sizeof(float*));
	(*y_train) = calloc(*n, sizeof(float*));
	(*test_set) = calloc(*m, sizeof(float*));
	(*y_test) = calloc(*m, sizeof(float*));

	srand(time(NULL));

	/* we will use a boolean array of size as total lines 
	   -- true: will be in train_set 
	   -- false: will be in test set */
	bool visitedNumbers[total_size];
	for( unsigned int i=0; i <total_size; i++)
		visitedNumbers[i] = true;

	unsigned int _n_=0, _m_=0;
	// until the number needed is succeded
	while( _m_!= *m ){
		// choose randomly and number
		unsigned int randomNumber = rand()%total_size;
		// if this number has not been discovered yet
		if(visitedNumbers[randomNumber]){
			visitedNumbers[randomNumber] = false;
			_m_++;
		}
	}
	/* not new memory for line -- use pointer from initial array*/
	_n_=0, _m_=0;
	for( unsigned int j = 0; j <total_size; j++ ){
		if( visitedNumbers[j] ){ 
			(*train_set)[_n_] = total_set[j];
			(*y_train)[_n_++] = total_y[j];
		}else{
			(*test_set)[_m_] = total_set[j];
			(*y_test)[_m_++] = total_y[j];
		}
	}

	return;
}



int main(void){

	unsigned int n,m;

	float** train_set;
	float** test_set;
	unsigned int* y_train;
	unsigned int* y_test;

	unsigned int total_size = 15;

	float** total_set = calloc(total_size,sizeof(float*));
	for( unsigned int i=0; i < total_size; i++ ){
		total_set[i] = calloc(COLUMNSIZE,sizeof(float));
	}

	unsigned int* total_y = calloc(total_size,sizeof(float));
	for( unsigned int i=0; i < total_size; i++ ){
		total_y[i] = ((i % 3)==0);
	}

	/*for( unsigned int i = 0; i < total_size; i++ ){
		for( unsigned int j = 0; j< COLUMNSIZE; j++ ){
			total_set[i][j] = (float)i*j;
			printf("%f\t",total_set[i][j] );
		}
		printf("\t %d\n",total_y[i]);
	}*/

	createSets(total_set, total_y, total_size, &train_set, &n, &test_set, &m, &y_train, &y_test );

	/*for( unsigned int i = 0; i < n; i++ ){
		for( unsigned int j = 0; j< COLUMNSIZE; j++ ){
			printf("%f\t",train_set[i][j] );
		}
		printf("\t %d\n",y_train[i]);
	}

	printf("_________________________________________________________________________\n\n");

	for( unsigned int i = 0; i < m; i++ ){
		for( unsigned int j = 0; j< COLUMNSIZE; j++ ){
			printf("%f\t",test_set[i][j] );
		}
		printf("\t %d\n",y_test[i]);
	}
	*/

	for( unsigned int i=0; i < total_size; i++ ) free(total_set[i]);
	free(y_train); free(y_test); free(total_y);
	free(train_set); free(test_set); free(total_set);

	return 0;
}