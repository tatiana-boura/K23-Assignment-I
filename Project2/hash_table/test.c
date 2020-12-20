#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../list/list.h"

int main(void){

	unsigned int* one = calloc(1,sizeof(unsigned int)); *one = 1;
	unsigned int* two = calloc(1,sizeof(unsigned int)); *two = 2;
	unsigned int* three = calloc(1,sizeof(unsigned int)); *three = 3;
	unsigned int* four = calloc(1,sizeof(unsigned int)); *four = 4;
	unsigned int* five = calloc(1,sizeof(unsigned int)); *five = 5;
	unsigned int* six = calloc(1,sizeof(unsigned int)); *six = 6;

	node* n=NULL;

	n = appendList(n,one); n = appendList(n,two); n = appendList(n,three);
	n = appendList(n,four); n = appendList(n,five); n = appendList(n,six);

	bool del[6] = { true, true, false, false, true, true };

	deleteWords( &n, del, 6 );

	node* temp=n;
	while( temp!=NULL ){
		printf("%d\t", *((unsigned int*)temp->data));
		temp=temp->next;
	}
	printf("\n");

	free(one); free(two); free(three); free(four); free(five); free(six);

	return 0;
}