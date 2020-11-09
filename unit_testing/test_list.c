#include "acutest.h"

#include <time.h>
#include "../list/list.h"
#include "../tuples/tuples.h"

#define STRSIZE 100

//___test_appendList_string_________________________________________________________
void test_appendList_string(void){
	/* this is a test function for lists that contain strings.
	In our code these are: cliques and propertyValueList */

	unsigned int N=200000;
	char** array = calloc(N,sizeof(char*));

	node* n=NULL;

	for( unsigned int i = 0; i<N; i++ ){
		
		array[i] = calloc(STRSIZE,sizeof(char));
		sprintf(array[i],"%d",i);

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

//___test_appendList_Tuple_________________________________________________________
void test_appendList_Tuple(void){
	//this is a test function for lists that tuples that are <string,
	//listOfValues>

	unsigned int N=1000;
	char** arrayOfNames = calloc(N,sizeof(char*));
	node* listOfTuples=NULL;

	char* str;
	
	// for example if j=5 then the tuple must be [5,[5,4,3,2,1,0]]
	for( unsigned int j=0; j<N; j++ ){
		
		arrayOfNames[j]=calloc(STRSIZE,sizeof(char));
		sprintf(arrayOfNames[j],"%d",j);
		TuplePtr t=calloc(1,sizeof(Tuple));

		for( unsigned int i = 0; i<=j; i++ ){
			// in this loop we are making the listOfStrings
			
			str = calloc(j,sizeof(char));
			sprintf(str,"%d",i);

			if(i==0) // first time
				tupleInitialization(t,arrayOfNames[j],str);
			else insertAtValueList(t,str);
		}

		listOfTuples = appendList(listOfTuples,t);
	}

	// time to check if values have been inserted as expected
	node* tempTuples=listOfTuples;

	for( int j=N-1; j>=0; j-- ){
		// checking if values at name are the same
		TEST_ASSERT(!strcmp(((TuplePtr)(tempTuples->data))->propertyName,arrayOfNames[j]));

		str = calloc(j,sizeof(char));
		node* tempValues=(((TuplePtr)(tempTuples->data))->propertyValueList);

		for( int i = j; i>=0; i-- ){
			sprintf(str,"%d",i);
			// checking if value at propertyValues list are the same
			TEST_ASSERT(!strcmp((char*)(tempValues->data),str));

			tempValues=tempValues->next;
		}
		free(str); str=NULL;
		tempTuples=tempTuples->next;
	}

	// free not needed memo
	destroyListOfTuples(listOfTuples, (void*)tupleDeletion );
	free(arrayOfNames); arrayOfNames=NULL;

	return;
}

//___test_mergeTwoLists___________________________________________________________
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

	/* check if the values are insertes as expected*/
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

//___test_addrFoundinList___________________________________________________________
void test_addrFoundinList(void){
	/* function to check if addrFoundinList finds the address correctly in a list
	of addresses */

	unsigned int N=10000;
	
	node* n=NULL;

	// keep random position of address in list
	srand(time(NULL));
	unsigned int randomNumForListPosition=rand();
	int* keepRandomAddress;

	/* we will insert addresses and keep a random one to check later */
	for( unsigned int i = 0; i<N; i++ ){
		
		int* addr = calloc(1,sizeof(int)); *addr = i;
		// at a random position keep address in order to search later
		if(i==randomNumForListPosition) keepRandomAddress = addr;

		n = appendList(n,addr);
	}

	// try and find the address [IT SHOULD BE FOUND]
	node* temp=n;
	for( unsigned int i = 0; i<N; i++ ){
		// checking if randomStoredAddress is found in expected position
		if(i==N-randomNumForListPosition-1) TEST_ASSERT(temp->data==keepRandomAddress);
		temp=temp->next;
	}

	/* now try and locate the address where this function is stored, which
	means that it cannot be used to allocate new memory [IT SHOULD NOT BE FOUND]*/
	void *addressOfThisFunction=(void*)&test_addrFoundinList;

	temp=n;
	for( unsigned int i = 0; i<N; i++ ){
		// if it is found, it is incorrect behavior
		TEST_ASSERT(temp->data!=test_addrFoundinList);
		temp=temp->next;
	}

	// free not needed memo
	destroyListOfStrings(n, true );

	return;
}

TEST_LIST = {
	{ "appendList_string", test_appendList_string },
	{ "appendList_tuple", test_appendList_Tuple },
	{ "merge", test_mergeTwoLists },
	{ "addrFoundinList", test_addrFoundinList },
	{ NULL, NULL } // end of tests
};