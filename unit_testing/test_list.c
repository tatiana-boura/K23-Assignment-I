#include "acutest.h"

#include "../list/list.h"
#include "../tuples/tuples.h"

#define STRSIZE 100


void test_appendList_string(void){
	/* this is a test function for lists that contain strings.
	In our code these are: cliques and propertyValueList */

	unsigned int N=200000;
	char** array = calloc(N,sizeof(char*));

	node* n=NULL;

	for( unsigned int i = 0; i<N; i++ ){
		
		array[i] = calloc(STRSIZE,sizeof(char));
		sprintf(array[i],"%d",i);
		//printf("%s\n", array[i]);

		n = appendList(n,array[i]);
	}

	node* temp=n;
	for( int i = N-1; i>=0; i-- ){
		// checking if values are inserted as expected
		TEST_ASSERT(!strcmp(temp->data,array[i]));
		temp=temp->next;
	}

	// free not needed memo
	destroyListOfStrings(n, true );
	free(array); array=NULL;

	return;
}

/*
void test_appendList_Tuple(void){
	//this is a test function for lists that tuples that are <string,
	//listOfValues>

	unsigned int N=10000;
	char** arrayOfNames = calloc(N,sizeof(char*));
	node* listOfTuples=NULL;

	char* str;
	
	for( unsigned int j=0; j<N; j++ ){

		node* listOfValues=NULL;
		
		for( unsigned int i = 0; i<j; i++ ){
			// in this loop we are making the listOfStrings
			
			str = calloc(j,sizeof(char));
			sprintf(str,"%d",i);

			listOfValues = appendList(listOfValues,str);
		}

		TuplePtr t;

		sprintf(arrayOfNames[j],"%d",j);
		tupleInitialization(t,arrayOfNames[j],listOfValues);

		listOfTuples = appendList(listOfTuples,t);
	}


	node* tempTuples=listOfTuples;
	for( unsigned int j=0; j<N; j++ ){

		TEST_ASSERT(!strcmp((((TuplePtr)(tempTuples->data))->propertyName),arrayOfNames[j]));

		str = calloc(j,sizeof(char));

		node* tempValues=(((TuplePtr)(tempTuples->data))->propertyValueList);
		for( unsigned int i = 0; i<j; i++ ){
			
			sprintf(str,"%d",i);
			
			TEST_ASSERT(!strcmp((((TuplePtr)(tempValues->data))),str));

			tempValues=tempValues->next;
		}
		free(str); str=NULL;

		tempTuples=tempTuples->next;
	}


	// free not needed memo

	destroyListOfTuples(listOfTuples, true );
	free(arrayOfNames); arrayOfNames=NULL;

	return;
}*/

void test_mergeTwoLists(void){
	/* this is a test function for merging lists */ 

	/* create two lists */
	unsigned int N1=5000, N2=10000;
	char** array1 = calloc(N1,sizeof(char*));
	node* n1=NULL;

	for( unsigned int i = 0; i<N1; i++ ){
		
		array1[i] = calloc(STRSIZE,sizeof(char));
		sprintf(array1[i],"%d",i);
		//printf("%s\n", array1[i]);

		n1 = appendList(n1,array1[i]);
	}

	char** array2 = calloc(N2,sizeof(char*));
	node* n2=NULL;

	for( unsigned int i = N1; i<N2; i++ ){
		
		array2[i] = calloc(STRSIZE,sizeof(char));
		sprintf(array2[i],"%d",i);
		//printf("%s\n", array2[i]);

		n2 = appendList(n2,array2[i]);
	}

	/* merge them */
	n1=mergeTwoLists(n1,n2);

	node* temp=n1;
	for( int i = N1-1; i>=0; i-- ){
		// checking if values are inserted as expected
		TEST_ASSERT(!strcmp(temp->data,array1[i]));
		temp=temp->next;
	}

	for( unsigned int i = N2-1; i>=N2; i-- ){
		// checking if values are inserted as expected
		TEST_ASSERT(!strcmp(temp->data,array2[i]));
		temp=temp->next;
	}

	// free not needed memo
	destroyListOfStrings(n1, true );
	free(array1); array1=NULL;
	free(array2); array2=NULL;

	return;
}

TEST_LIST = {
	{ "appendList_string", test_appendList_string },
	//{ "appendList_tuple", test_appendList_Tuple },
	{ "merge", test_mergeTwoLists },
	{ NULL, NULL } // end of tests
};