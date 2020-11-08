#include "acutest.h"

#include "../list/list.h"
#include "../tuples/tuples.h"

#define STRSIZE 100


void test_appendList_string(void){
	/* this is a test function for lists that contain strings.
	In our code these are: cliques and propertyValueList */

	/* initializing a big number in order to try and cause 
	stack overflow because our function is recursive*/
	unsigned int N=10000;
	char** array = calloc(N,sizeof(char*));

	node* n=NULL;

	for( unsigned int i = 0; i<N; i++ ){
		
		array[i] = calloc(STRSIZE,sizeof(char));
		sprintf(array[i],"%d",i);
		//printf("%s\n", array[i]);

		n = appendList(n,array[i]);
	}

	node* temp=n;
	for( unsigned int i = 0; i<N; i++ ){
		// checking if values are inserted as expected
		TEST_ASSERT(!strcmp(temp->data,array[i]));
		temp=temp->next;
	}

	// free not needed memo
	destroyListOfStrings(n, true );
	free(array); array=NULL;

	return;
}

void test_appendList_Tuple(void){
	/* this is a test function for lists that tuples that are <string,
	listOfStrings>*/

	/* initializing a big number in order to try and cause 
	stack overflow because our function is recursive*/
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
}

TEST_LIST = {
	{ "appendList_string", test_appendList_string },
	{ "appendList_tuple", test_appendList_Tuple },
	{ NULL, NULL } // end of tests
};