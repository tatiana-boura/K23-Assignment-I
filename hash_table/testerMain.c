#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hash_table.h"

#define BUCKETSIZE 50
#define HTSIZE 2 

//to trexw ws eksis: gcc -o prog testerMain.c ../list/list.c hash_table.c ../tuples/tuples.c


int main(void){

	//___INITIALIZING_HASH_TABLE____________________________________________________ 

	char* s1 = calloc(100,sizeof(char)); strcpy(s1,"https://ebay/home.com");
	char* s2 = calloc(100,sizeof(char)); strcpy(s2,"https://amazon/lavie.com");
	char* s3 = calloc(100,sizeof(char)); strcpy(s3,"https://skroutz/chilly.com");
	char* s4 = calloc(100,sizeof(char)); strcpy(s4,"https://netflix/favourites.com");
	char* s5 = calloc(100,sizeof(char)); strcpy(s5,"https://aegeanairlines/tickets.com");
	char* s6 = calloc(100,sizeof(char)); strcpy(s6,"https://ebay/home0.com");

	char* k1 = calloc(20,sizeof(char)); strcpy(k1,"one");
	char* k2 = calloc(20,sizeof(char)); strcpy(k2,"two");
	char* k3 = calloc(20,sizeof(char)); strcpy(k3,"three");
	char* k4 = calloc(20,sizeof(char)); strcpy(k4,"four");
	char* k5 = calloc(20,sizeof(char)); strcpy(k5,"five");
	char* k6 = calloc(20,sizeof(char)); strcpy(k6,"six");
	char* k7 = calloc(20,sizeof(char)); strcpy(k7,"seven");
	char* k8 = calloc(20,sizeof(char)); strcpy(k8,"eight");
	char* k9 = calloc(20,sizeof(char)); strcpy(k9,"nine");
	char* k10 = calloc(20,sizeof(char)); strcpy(k10,"ten");
	char* k11 = calloc(20,sizeof(char)); strcpy(k11,"eleven");
	char* k12 = calloc(20,sizeof(char)); strcpy(k12,"twelve");
	char* k13 = calloc(20,sizeof(char)); strcpy(k13,"thirteen");
	char* k14 = calloc(20,sizeof(char)); strcpy(k14,"fourteen");
	char* k15 = calloc(20,sizeof(char)); strcpy(k15,"fifteen");
	char* k16 = calloc(20,sizeof(char)); strcpy(k16,"sixteen");
	char* k17 = calloc(20,sizeof(char)); strcpy(k17,"seventeen");
	char* k18 = calloc(20,sizeof(char)); strcpy(k18,"eighteen");
	char* k19 = calloc(20,sizeof(char)); strcpy(k19,"nineteen");
	char* k20 = calloc(20,sizeof(char)); strcpy(k20,"twenty");
	char* k21 = calloc(20,sizeof(char)); strcpy(k21,"twentyone");
	char* k22 = calloc(20,sizeof(char)); strcpy(k22,"twentytwo");
	char* k23 = calloc(20,sizeof(char)); strcpy(k23,"twentythree");
	char* k24 = calloc(20,sizeof(char)); strcpy(k24,"twentyfour");
	char* k25 = calloc(20,sizeof(char)); strcpy(k25,"twentyfive");
	char* k26 = calloc(20,sizeof(char)); strcpy(k26,"twentysix");

	TuplePtr t1 = calloc(1,sizeof(Tuple)); tupleInitialization(t1,k1,k2);
	insertAtValueList(t1,k21); insertAtValueList(t1,k22); insertAtValueList(t1,k23); insertAtValueList(t1,k24);
	TuplePtr t2 = calloc(1,sizeof(Tuple)); tupleInitialization(t2,k3,k4);
	TuplePtr t3 = calloc(1,sizeof(Tuple)); tupleInitialization(t3,k5,k6);
	TuplePtr t4 = calloc(1,sizeof(Tuple)); tupleInitialization(t4,k7,k8);
	insertAtValueList(t4,k25); insertAtValueList(t4,k26);
	TuplePtr t5 = calloc(1,sizeof(Tuple)); tupleInitialization(t5,k9,k10);
	TuplePtr t6 = calloc(1,sizeof(Tuple)); tupleInitialization(t6,k11,k12);
	TuplePtr t7 = calloc(1,sizeof(Tuple)); tupleInitialization(t7,k13,k14);
	TuplePtr t8 = calloc(1,sizeof(Tuple)); tupleInitialization(t8,k15,k16);
	TuplePtr t9 = calloc(1,sizeof(Tuple)); tupleInitialization(t9,k17,k18);
	TuplePtr t10 = calloc(1,sizeof(Tuple)); tupleInitialization(t10,k19,k20);


	node* l1=NULL; l1 = appendList(l1,t1); l1 = appendList(l1,t2); l1 = appendList(l1,t3); l1 = appendList(l1,t4);
	node* l2=NULL; l2 = appendList(l2,t5);
	node* l3=NULL; l3 = appendList(l3,t6); l3 = appendList(l3,t7);
	node* l4=NULL; l4 = appendList(l4,t8);
	node* l5=NULL; l5 = appendList(l5,t9);
	node* l6=NULL; l6 = appendList(l6,t10);


	hashTable* ht = createHT(HTSIZE);

	addtoHT(ht, s1, BUCKETSIZE, l1);
	printf("--------------------------------------\n");
	addtoHT(ht, s2, BUCKETSIZE, l2);
	printf("--------------------------------------\n");
	addtoHT(ht, s3, BUCKETSIZE, l3);
	printf("--------------------------------------\n");
	addtoHT(ht, s4, BUCKETSIZE ,l4);
	printf("--------------------------------------\n");
	addtoHT(ht, s5, BUCKETSIZE ,l5);
	printf("--------------------------------------\n");
	addtoHT(ht, s6, BUCKETSIZE ,l6);

	//__END_INITIALIZING_HASH_TABLE____________________________________________________________________________________

	//__CHECK_THAT_FOUND_INT_HT_FUNCTION_WORKS_________________________________________________________________________

	printf("\n");

	unsigned int entryNum;
	bucket* bucketFound;

	bool found=false;
	
	for( unsigned int i=0; i<HTSIZE; i++ ){
		found = foundInHT(ht, s1, BUCKETSIZE, &entryNum, &bucketFound );
		if(found) break;
	}

	if(found) printf("found\n");
	else printf("not found\n");

	found=false;

	for( unsigned int i=0; i<HTSIZE; i++ ){
		found = foundInHT(ht, "an to vreis sfira mou", BUCKETSIZE, &entryNum, &bucketFound );
		if(found) break;
	}

	if(found) printf("found\n");
	else printf("not found\n");

	printf("\n");

	//__END__CHECK_THAT_FOUND_INT_HT_FUNCTION_WORKS____________________________________________________________________

	//__PLAY_WITH_POINTERS_____________________________________________________________________________________________

	unsigned int entryNum1, entryNum2;
	bucket* bucketFound1;
	bucket* bucketFound2;

	found=false;
	
	for( unsigned int i=0; i<HTSIZE; i++ ){
		found = foundInHT(ht, s1, BUCKETSIZE, &entryNum1, &bucketFound1 );
		if(found) break;
	}

	found=false;

	for( unsigned int i=0; i<HTSIZE; i++ ){
		found = foundInHT(ht, s6, BUCKETSIZE, &entryNum2, &bucketFound2 );
		if(found) break;
	}

	changePointers(ht, BUCKETSIZE,&bucketFound1, entryNum1, &bucketFound2, entryNum2 );

	found=false;
	
	for( unsigned int i=0; i<HTSIZE; i++ ){
		found = foundInHT(ht, s6, BUCKETSIZE, &entryNum1, &bucketFound1 );
		if(found) break;
	}

	found=false;

	for( unsigned int i=0; i<HTSIZE; i++ ){
		found = foundInHT(ht, s4, BUCKETSIZE, &entryNum2, &bucketFound2 );
		if(found) break;
	}

	changePointers(ht, BUCKETSIZE,&bucketFound1, entryNum1, &bucketFound2, entryNum2 );

	found=false;
	
	for( unsigned int i=0; i<HTSIZE; i++ ){
		found = foundInHT(ht, s6, BUCKETSIZE, &entryNum1, &bucketFound1 );
		if(found) break;
	}

	found=false;

	for( unsigned int i=0; i<HTSIZE; i++ ){
		found = foundInHT(ht, s3, BUCKETSIZE, &entryNum2, &bucketFound2 );
		if(found) break;
	}

	changePointers(ht, BUCKETSIZE,&bucketFound1, entryNum1, &bucketFound2, entryNum2 );

	//printHT(ht);

	makeOutputFile(ht, BUCKETSIZE);


	//__END_PLAY_WITH_POINTERS_________________________________________________________________________________________

	//_MEM_DEALLOC_____________________________________________________________________________________________________

	destroyHT(ht,BUCKETSIZE);

	free(s1); s1=NULL; free(s2); s2=NULL; free(s3); s3=NULL; free(s4); s4=NULL; free(s5); s5=NULL; free(s6); s6=NULL;
	free(k1); k1=NULL; free(k3); k3=NULL; free(k5); k5=NULL; free(k7); k7=NULL; free(k9); k9=NULL; free(k11); k11=NULL; 
	free(k13); k13=NULL; free(k15); k15=NULL; free(k17); k17=NULL; free(k19); k19=NULL; 
	//free(k21); k21=NULL; free(k22); k22=NULL; free(k23); k23=NULL; free(k24); k24=NULL; 
	//free(k25); k25=NULL; free(k26); k26=NULL;

	//destroyHT(ht,BUCKETSIZE);

	return 0;
}