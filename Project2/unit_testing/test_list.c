#include "../../unit_testing/acutest.h"

#include <time.h>
#include "../list/list.h"
#include "../word_info/word_info.h"

#define STRSIZE 100

//___test_appendList_string_________________________________________________________
void test_appendList_string(void){
	/* this is a test function for lists that contain strings.
	In our code these are: cliques and wordInfoList */

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


//___test_appendList_wordInfo_________________________________________________________
void test_appendList_wordInfo(void){
	//this is a test function for lists of wordInfo (<string,count>)

	unsigned int N=1000;
	char** arrayOfWords = calloc(N,sizeof(char*));
	node* wordInfoList=NULL;

	char* str;
	
	// for example if j=5 then the wordInfo must be [5,0]
	for( unsigned int j=0; j<N; j++ ){
		
		arrayOfWords[j]=calloc(STRSIZE,sizeof(char));
		sprintf(arrayOfWords[j],"%d",j);
		wordInfo* w=calloc(1,sizeof(wordInfo));

		wordInfoInitialization(w,arrayOfWords[j]);

		wordInfoList = appendList(wordInfoList,w);
	}

	// update wordInfoList with counts: i.e. wordInfo that was inserted 5th has count 4 
	node* tempWords=wordInfoList;
	for( int j=N-1; j>=0; j-- ){
		((wordInfo *)(tempWords->data))->count=j;
		tempWords=tempWords->next;
	}

	// time to check if values have been inserted as expected
	tempWords=wordInfoList;

	for( int j=N-1; j>=0; j-- ){
		// checking if values at word are the same
		TEST_ASSERT(!strcmp(((wordInfo *)(tempWords->data))->word,arrayOfWords[j]));

		TEST_ASSERT(((wordInfo *)(tempWords->data))->count==j);
		tempWords=tempWords->next;
	}

	// free not needed memo
	destroyListOfWordInfo(wordInfoList, (void*)wordInfoDeletion );
	free(arrayOfWords); arrayOfWords=NULL;

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
	unsigned int randomNumForListPosition=rand()%N;
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
	means that it cannot be used to allocate new memory [IT SHOULD NOT BE FOUND] */
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

//___test_removeNodet___________________________________________________________
void test_removeNode(void){
	/* function to check if node with certain address(we have a list of addresses) 
	is correctly removed from a list */

	unsigned int N=1000;
	
	node* n=NULL;

	// keep random position of address in list
	srand(time(NULL));
	unsigned int randomNumForListPositionR=rand()%N;
	unsigned int randomNumForListPositionS;
	/* make sure that the two positions are different
	so that we don't remove the same and after look at something
	we have removed*/
	do{
		randomNumForListPositionS=rand()%N;
	}while(randomNumForListPositionS==randomNumForListPositionR);

	int* keepRandomAddressToRemove;
	int* keepRandomAddressToSearch;

	/* we will insert addresses and keep a random one to remove later */
	for( unsigned int i = 0; i<N; i++ ){
		
		int* addr = calloc(1,sizeof(int)); *addr = i;
		// at a random position keep address in order to remove later
		if(i==randomNumForListPositionR) keepRandomAddressToRemove = addr;
		if(i==randomNumForListPositionS) keepRandomAddressToSearch = addr;

		n = appendList(n,addr);
	}
	
	// remove that address
	node* _n_ = removeNode(&n,keepRandomAddressToRemove);
	if( _n_!=NULL ){
		free(_n_->data); _n_->data=NULL;
		free(_n_); _n_=NULL;
	}
	// try and find the removed address [IT SHOULD NOT BE FOUND]
	node* temp=n;
	for( unsigned int i = 0; i<N-1; i++ ){
		// checking if randomStoredAddress is found
		TEST_ASSERT(temp->data!=keepRandomAddressToRemove);
		temp=temp->next;
	}

	// try and find the not removed address [IT SHOULD BE FOUND]
	temp=n;
	for( unsigned int i = 0; i<N-1; i++ ){
		/* checking if randomStoredAddress is found and in expected position
		   the position depends on the removed address, if it is before the one
		   we are going to search, or after */
		if( randomNumForListPositionR > randomNumForListPositionS ){
			if(i==N-randomNumForListPositionS-2){
				TEST_ASSERT(temp->data==keepRandomAddressToSearch);
			} 
		}else{
			if(i==N-randomNumForListPositionS-1){
				TEST_ASSERT(temp->data==keepRandomAddressToSearch);
			} 
		}
		temp=temp->next;
	}

	// free not needed memo
	destroyListOfStrings(n, true );

	return;
}

TEST_LIST = {
	{ "appendList_string", test_appendList_string },
	{ "appendList_wordInfo", test_appendList_wordInfo },
	{ "merge", test_mergeTwoLists },
	{ "addrFoundinList", test_addrFoundinList },
	{ "removeNode", test_removeNode },
	{ NULL, NULL } // end of tests
};
