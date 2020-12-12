#include "logisticReg.h"

// gcc -g -o test testLogReg.c logisticReg.c -lm

int main(void){

	float* a = calloc(10,sizeof(float));
	float* b = calloc(10,sizeof(float));

	for( unsigned int i = 0; i<10; i++){
		a[i] = 1;
		b[i] = 1;
	}

	printf("%f\n", hypothesis(a, b, 1, 10) );

	free(a); a=NULL;
	free(b); b=NULL;

	return 0;
}