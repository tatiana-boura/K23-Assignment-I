#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void createSets( float** total_set, unsigned int total_size ,float*** train_set, unsigned int* n, float*** test_set, unsigned int* m ){
	/* total_set  : whole set that will be split
	   total_size : size of whole set
	   train_set  : training set to be returned
	   n		  : size of training set
	   test_set   : test set to be retuned
	   m          : size of test set            */	
		
	// the training size will be the 80% of the initial	
	*n = 0.8 * total_size;	
	*m = total_size - *n;

	return;
}



int main(void){

	unsigned int n,m;
	createSets(NULL, 183, NULL, &n, NULL, &m );
	printf("%d -- %d\n", n, m);

	return 0;
}