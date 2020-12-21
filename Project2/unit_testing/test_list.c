#include "../../unit_testing/acutest.h"

#include <time.h>
#include "../list/list.h"
#include "../word_info/word_info.h"

#define STRSIZE 100

//___test_appendList_string___________________________________________________________
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

//___test_mergeTwoLists_______________________________________________________________
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

//___test_addrFoundinList_____________________________________________________________
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

//___test_removeNode__________________________________________________________________
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

//___test_notInList___________________________________________________________________
void test_notInList(void){
	char** array = calloc(4,sizeof(char*));

	array[0]=calloc(STRSIZE,sizeof(char)); strcpy(array[0],"zero");
    array[1]=calloc(STRSIZE,sizeof(char)); strcpy(array[1],"one");
	array[2]=calloc(STRSIZE,sizeof(char)); strcpy(array[2],"two");
	array[3]=calloc(STRSIZE,sizeof(char)); strcpy(array[3],"three");
    node* list=NULL; 

	for( unsigned int j=0; j<4; j++ )
		list = appendList(list,array[j]);

	for( unsigned int j=0; j<4; j++ )
		TEST_ASSERT(!notInlist(list,array[j]));
	
	TEST_ASSERT(notInlist(list,"four"));

	destroyListOfStrings(list, true );
	free(array); array=NULL;
}

//___test_sortedInsertStr_____________________________________________________________
void test_sortedInsertStr(void){

	unsigned int N=5;
	char** arrayOfWords = calloc(N,sizeof(char*));
	node* wordInfoList=NULL;

	srand(time(NULL));
	// for example if j=5 then the wordInfo must be [5,0]
	for( unsigned int j=0; j<N; j++ ){
		
		arrayOfWords[j]=calloc(STRSIZE,sizeof(char));
		//Create a wordInfo struct with a char A-Z as word
		char rand_char = (rand() % (90 - 65 + 1)) + 65;
		sprintf(arrayOfWords[j],"%c",rand_char);
		wordInfo* w=calloc(1,sizeof(wordInfo));

		wordInfoInitialization(w,arrayOfWords[j]);

		sortedInsertStr(&wordInfoList,w);
	}

	// update wordInfoList with counts: i.e. wordInfo that was inserted 5th has count 4 
	node* tempWords=wordInfoList;
	for( int j=N-1; j>=0; j-- ){
		((wordInfo *)(tempWords->data))->count=j;
		tempWords=tempWords->next;
	}

	// time to check if values have been inserted as expected
	tempWords=wordInfoList;
	node* previous=NULL; 
	for( int j=N-1; j>=0; j-- ){
		if(previous!=NULL)
			TEST_ASSERT((strcmp(((wordInfo *)(tempWords->data))->word,((wordInfo *)(previous->data))->word)>=0));
				
		// checking if count at word is the same
		TEST_ASSERT(((wordInfo *)(tempWords->data))->count==j);
		previous=tempWords;
		tempWords=tempWords->next;
	}

	// free not needed memo
	destroyListOfWordInfo(wordInfoList, (void*)wordInfoDeletion );
	free(arrayOfWords); arrayOfWords=NULL;

	return;
}

//___test_foundInSortedListStr________________________________________________________
void test_foundInSortedListStr(void){

	char** arrayOfWords = calloc(5,sizeof(char*));

	arrayOfWords[0]=calloc(STRSIZE,sizeof(char)); strcpy(arrayOfWords[0],"red");
    arrayOfWords[1]=calloc(STRSIZE,sizeof(char)); strcpy(arrayOfWords[1],"yellow");
	arrayOfWords[2]=calloc(STRSIZE,sizeof(char)); strcpy(arrayOfWords[2],"pink");
	arrayOfWords[3]=calloc(STRSIZE,sizeof(char)); strcpy(arrayOfWords[3],"blue");
	arrayOfWords[4]=calloc(STRSIZE,sizeof(char)); strcpy(arrayOfWords[4],"gray");

    node* wordInfoList=NULL; 

	for( unsigned int j=0; j<5; j++ ){
		wordInfo* w=calloc(1,sizeof(wordInfo));
		wordInfoInitialization(w,arrayOfWords[j]);
		sortedInsertStr(&wordInfoList,w);
	}

	//Check if pink is found and increase its counter (initialized to 1)
	TEST_ASSERT(foundInSortedListStr(wordInfoList,"pink",true));

	//Check if pink's counter is indeed 2
	node* tempWords=wordInfoList;
	while(tempWords!=NULL){
		if(!strcmp(((wordInfo *)(tempWords->data))->word,"pink"))
			TEST_ASSERT(((wordInfo *)(tempWords->data))->count==2);
		tempWords=tempWords->next;
	}

	//Check without increasing their counters
	for( unsigned int j=0; j<5; j++ )
		TEST_ASSERT(foundInSortedListStr(wordInfoList,arrayOfWords[j],false));
	
	//Check if "orange" is found - it shouldnt
	TEST_ASSERT(!foundInSortedListStr(wordInfoList,"orange",false));

	destroyListOfWordInfo(wordInfoList, (void*)wordInfoDeletion );
	free(arrayOfWords); arrayOfWords=NULL;

	return;
}

//___test_deleteWords_________________________________________________________________
void test_deleteWords(void){

	char** array = calloc(4,sizeof(char*));
	array[0]=calloc(STRSIZE,sizeof(char)); strcpy(array[0],"zero");
    array[1]=calloc(STRSIZE,sizeof(char)); strcpy(array[1],"one");
	array[2]=calloc(STRSIZE,sizeof(char)); strcpy(array[2],"two");
	array[3]=calloc(STRSIZE,sizeof(char)); strcpy(array[3],"three");

    node* wordInfoList=NULL; 

	for( unsigned int j=0; j<4; j++ ){
		wordInfo* w=calloc(1,sizeof(wordInfo));
		wordInfoInitialization(w,array[j]);
		wordInfoList = appendList(wordInfoList,w);
	}

	// each bool in the array represents the position that will either
	// be deleted or not - careful the list appends through the front
	// so if del[0] is true, the last wordInfo of the list must be deleted
	bool del[4] = { true, false, false, true };

	deleteWords( &wordInfoList, del, 4 );

	node* temp=wordInfoList;
	// The first node now should contain wordInfo with word "two" because 
	// del[2] was false
	TEST_ASSERT(!strcmp(((wordInfo *)(temp->data))->word,"two"));
	temp = temp->next;
	// The second node should be "one" because del[1] was false
	TEST_ASSERT(!strcmp(((wordInfo *)(temp->data))->word,"one"));
	// The list should end here
	TEST_ASSERT(temp->next==NULL);

	destroyListOfWordInfo(wordInfoList, (void*)wordInfoDeletion );
	free(array); array=NULL;
	return;
}

TEST_LIST = {
	{ "appendList_string", test_appendList_string },
	{ "appendList_wordInfo", test_appendList_wordInfo },
	{ "merge", test_mergeTwoLists },
	{ "addrFoundinList", test_addrFoundinList },
	{ "removeNode", test_removeNode },
	{ "notInList", test_notInList },
	{ "sortedInsertStr", test_sortedInsertStr },
	{ "foundInSortedListStr", test_foundInSortedListStr },
	{ "deleteWords", test_deleteWords },
	{ NULL, NULL } // end of tests
};
