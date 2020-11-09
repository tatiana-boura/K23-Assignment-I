#include "acutest.h"

#include "../tuples/tuples.h"
#include "../list/list.h"

#define STRSIZE 100

//___test_tupleInitialization_________________________________________________________
void test_tupleInitialization(void){
	/* this is a test function for testing the initialization of a tuples */

	// make two strings
	char* _pName_=calloc(STRSIZE,sizeof(char)); strcpy(_pName_,"name");
	char* _pValue_=calloc(STRSIZE,sizeof(char)); strcpy(_pValue_,"value");

	// make tuple and initialize it
	TuplePtr t=calloc(1,sizeof(Tuple));
	tupleInitialization(t,_pName_,_pValue_);

	// check if values are inserted as expected
	TEST_ASSERT(!strcmp(t->propertyName,_pName_));
	TEST_ASSERT(!strcmp(t->propertyValueList->data,_pValue_));

	tupleDeletion(t);	

	return;
}

//___test_insertAtValueList___________________________________________________________
void test_insertAtValueList(void){
	/* checks if multiple values are correctly inserted in propertyValueList*/

	// make two strings
	char* _pName_=calloc(STRSIZE,sizeof(char)); strcpy(_pName_,"name");
	char* _pValue_=calloc(STRSIZE,sizeof(char)); strcpy(_pValue_,"value");

	// make tuple and initialize it
	TuplePtr t=calloc(1,sizeof(Tuple));
	tupleInitialization(t,_pName_,_pValue_);

	// now create values in order to insert at propertyValueList
	unsigned int N=10;
	char* str;
	// start the insertion
	for( unsigned int i=0; i<N; i++ ){
		str = calloc(STRSIZE,sizeof(char));
		sprintf(str,"%d",i);
		insertAtValueList(t,str);
	}

	// check if propertyName is as expected
	TEST_ASSERT(!strcmp(t->propertyName,_pName_));	
	// now check if propertyValueList is as expected
	node* temp=t->propertyValueList;
	for( int i=N-1; i>=-1; i-- ){
		// if value inserted at tupleInitialization
		if(i==-1) TEST_ASSERT(!strcmp(temp->data,_pValue_));
		else{
			str = calloc(STRSIZE,sizeof(char));
			sprintf(str,"%d",i);
			TEST_ASSERT(!strcmp(temp->data,str));
		}
		temp=temp->next;
	}

	tupleDeletion(t);	

	return;
}

TEST_LIST = {
	{ "tupleInit", test_tupleInitialization },
	{ "insertAtValueList", test_insertAtValueList},
	{ NULL, NULL } // end of tests
};