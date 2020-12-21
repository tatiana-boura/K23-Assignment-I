#include <math.h>

#include "../../unit_testing/acutest.h"

#include "../hash_table/hash_table.h"
#include "../vocabulary/vocabulary.h"


#define BUCKETSIZEVOC 100
#define BUCKETSIZE 200
#define HTSIZE 20
#define HTSIZEVOC 3
#define STRSIZE 50

node* vocabulary = NULL;

//___test_making_bow_for_each_JSON____________________________________________________________________________________
void test_bow_vector(void){

    hashTableVOC* htVOC = createHTVOC(HTSIZEVOC);

    unsigned int size=0; 

    /* First sentence / .json */
    char* w1 = calloc(STRSIZE,sizeof(char)); strcpy(w1,"red");
    char* w2 = calloc(STRSIZE,sizeof(char)); strcpy(w2,"rose");
    char* w3 = calloc(STRSIZE,sizeof(char)); strcpy(w3,"is");
    char* w4 = calloc(STRSIZE,sizeof(char)); strcpy(w4,"red");
    
    /* WordInfoListA gets w1-w4  */
    node* wordInfoListA=NULL; 
    
    addToVoc(&vocabulary,w1, wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w1); 
   
    addToVoc(&vocabulary,w2,wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w2); 
   
    addToVoc(&vocabulary,w3,wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w3); 
  
    addToVoc(&vocabulary,w4,wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w4);
    
    /* Second sentence / .json */
    char* w5 = calloc(STRSIZE,sizeof(char)); strcpy(w5,"violet");
    char* w6 = calloc(STRSIZE,sizeof(char)); strcpy(w6,"is");
    char* w7 = calloc(STRSIZE,sizeof(char)); strcpy(w7,"red");
    char* w8 = calloc(STRSIZE,sizeof(char)); strcpy(w8,"and");
    char* w9 = calloc(STRSIZE,sizeof(char)); strcpy(w9,"blue");

    // WordInfoListB gets w5-w9 
    node* wordInfoListB=NULL; 

    addToVoc(&vocabulary,w5,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w5); 
    
    addToVoc(&vocabulary,w6,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w6); 
   
    addToVoc(&vocabulary,w7,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w7); 
    
    addToVoc(&vocabulary,w8,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w8);
 
    addToVoc(&vocabulary,w9,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w9); 

    // make two 'json'
    char* sA = calloc(STRSIZE,sizeof(char)); strcpy(sA,"roses");
    char* sB = calloc(STRSIZE,sizeof(char)); strcpy(sB,"violets");

    // initialize hash
    hashTable* ht = createHT(HTSIZE);
 	// add json into hash
	addtoHT(ht, sA, BUCKETSIZE, wordInfoListA);
    addtoHT(ht, sB, BUCKETSIZE, wordInfoListB);

    printf("\n");

    //_____ compute BoW vector for every JSON file _________________________________________
    size = make_BoW_vectorsDROP( ht, BUCKETSIZE, size, &vocabulary, 2, htVOC, BUCKETSIZEVOC );

    /* --------------------------------------------------------------
    After computing the BoW we expect the 
    initial vocabulary -- 'blue	and	violet is rose red'
    to become          -- 'blue	and	violet rose'
    because the tfidf of the words 'is' and 'red' is zero(0.000), as 
    they exist in both json in our hash and they will be cut due to 
    the lower limit of the allowed average tfidf. 
    Thus for json with name "violets" we expect the BoW vector to be:
            1.000000	1.000000	1.000000	0.000000
    and for json with name "roses" we expect the BoW vector to be: 
    		0.000000	0.000000	0.000000	1.000000							
    ------------------------------------------------------------------*/

    // go through the hash and check the above expectation
    unsigned int numOfEntries =(BUCKETSIZE-sizeof(bucket*))/sizeof(bucketEntry*);

    for( unsigned int i=0; i<ht->size; i++ ){
        if(ht->table[i] != NULL){ 
		    node* temp = ht->table[i];
		    while(temp!=NULL){
		        bucketEntry** entryTable = temp->data;
		        for(unsigned int i = 0;i<numOfEntries;i++){
		            if((entryTable[i]!=NULL)){
		            	
		            	// for json with name "roses"
		            	if( strcmp(entryTable[i]->path,"roses") == 0 ){
		            		// check values of the vector
		            		TEST_ASSERT(entryTable[i]->vector[0] == 0.0);
		            		TEST_ASSERT(entryTable[i]->vector[1] == 0.0);
		            		TEST_ASSERT(entryTable[i]->vector[2] == 0.0);
		            		TEST_ASSERT(entryTable[i]->vector[3] == 1.0);
		            		
		            	}else if ( strcmp(entryTable[i]->path,"violets") == 0 ){
		            		// check values of the vector
		            		TEST_ASSERT(entryTable[i]->vector[0] == 1.0);
		            		TEST_ASSERT(entryTable[i]->vector[1] == 1.0);
		            		TEST_ASSERT(entryTable[i]->vector[2] == 1.0);
		            		TEST_ASSERT(entryTable[i]->vector[3] == 0.0);
		            	// no other file should be found	
		            	}else TEST_ASSERT(false);
		            }
		        }
		        temp=temp->next;
		    }
		}
    }
        
	//________________________________________________________________________________________________________________

    destroyListOfWordInfo(vocabulary,(void*)wordInfoDeletion);
    destroyHT(ht,BUCKETSIZE);
    destroyHTVOC(htVOC,BUCKETSIZE);

	return;
}

//___test_making_tfidf_for_each_JSON(drop columns and then recompute tfidf)___________________________________________
void test_tfidf_vectorDROPnRECOMPUTE(void){

    hashTableVOC* htVOC = createHTVOC(HTSIZEVOC);

    unsigned int size=0; 

    /* First sentence / .json */
    char* w1 = calloc(STRSIZE,sizeof(char)); strcpy(w1,"red");
    char* w2 = calloc(STRSIZE,sizeof(char)); strcpy(w2,"rose");
    char* w3 = calloc(STRSIZE,sizeof(char)); strcpy(w3,"is");
    char* w4 = calloc(STRSIZE,sizeof(char)); strcpy(w4,"red");
    
    /* WordInfoListA gets w1-w4  */
    node* wordInfoListA=NULL; 
    
    addToVoc(&vocabulary,w1, wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w1); 
   
    addToVoc(&vocabulary,w2,wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w2); 
   
    addToVoc(&vocabulary,w3,wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w3); 
  
    addToVoc(&vocabulary,w4,wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w4);
    
    /* Second sentence / .json */
    char* w5 = calloc(STRSIZE,sizeof(char)); strcpy(w5,"violet");
    char* w6 = calloc(STRSIZE,sizeof(char)); strcpy(w6,"is");
    char* w7 = calloc(STRSIZE,sizeof(char)); strcpy(w7,"red");
    char* w8 = calloc(STRSIZE,sizeof(char)); strcpy(w8,"and");
    char* w9 = calloc(STRSIZE,sizeof(char)); strcpy(w9,"blue");

    // WordInfoListB gets w5-w9 
    node* wordInfoListB=NULL; 

    addToVoc(&vocabulary,w5,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w5); 
    
    addToVoc(&vocabulary,w6,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w6); 
   
    addToVoc(&vocabulary,w7,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w7); 
    
    addToVoc(&vocabulary,w8,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w8);
 
    addToVoc(&vocabulary,w9,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w9); 

    // make two 'json'
    char* sA = calloc(STRSIZE,sizeof(char)); strcpy(sA,"roses");
    char* sB = calloc(STRSIZE,sizeof(char)); strcpy(sB,"violets");

    // initialize hash
    hashTable* ht = createHT(HTSIZE);
 	// add json into hash
	addtoHT(ht, sA, BUCKETSIZE, wordInfoListA);
    addtoHT(ht, sB, BUCKETSIZE, wordInfoListB);

    printf("\n");

    //_____ compute tfidf vector for every JSON file _________________________________________
    size = make_tfidf_vectorsDROPnRECOMPUTE( ht, BUCKETSIZE, size, &vocabulary, 2, htVOC, BUCKETSIZEVOC );

    /* --------------------------------------------------------------
    After computing the tfidf we expect the 
    initial vocabulary -- 'blue	and	violet is rose red'
    to become          -- 'blue	and	violet rose'
    because the tfidf of the words 'is' and 'red' is zero(0.000), as 
    they exist in both json in our hash and they will be cut due to 
    the lower limit of the allowed average tfidf.

    So, BEFORE erasing the words from the vocabulary

	--  for json with name "violets" we expect the tfidf vector to be:
		0.060206	0.060206	0.060206	0.000000	0.000000	0.000000
	--  and for json with name "roses" we expect the tfidf vector to be: 
    	0.000000	0.000000	0.000000	0.000000	0.075258	0.000000

	and AFTER erasing the words from the vocabulary and recomputation of tfidf:
	--  for json with name "violets" we expect the tfidf vector to be:
		0.100343	0.100343	0.100343	0.000000
    --  and for json with name "roses" we expect the tfidf vector to be: 
    	0.000000	0.000000	0.000000	0.301030							
    ------------------------------------------------------------------*/

    // go through the hash and check the above expectation
    unsigned int numOfEntries =(BUCKETSIZE-sizeof(bucket*))/sizeof(bucketEntry*);

    for( unsigned int i=0; i<ht->size; i++ ){
        if(ht->table[i] != NULL){ 
		    node* temp = ht->table[i];
		    while(temp!=NULL){
		        bucketEntry** entryTable = temp->data;
		        for(unsigned int i = 0;i<numOfEntries;i++){
		            if((entryTable[i]!=NULL)){
		            	// for json with name "violets"
		            	if( strcmp(entryTable[i]->path,"violets") == 0 ){
		            		// check values of the vector
		            		/* NOTE: we check absolute difference because the equality depents
		            		on number of digits displayed and the float error*/
		            		TEST_ASSERT(abs(entryTable[i]->vector[0] - 0.100343) < 0.0000001);
		            		TEST_ASSERT(abs(entryTable[i]->vector[1] - 0.100343) < 0.0000001);
		            		TEST_ASSERT(abs(entryTable[i]->vector[2] - 0.100343) < 0.0000001);
		            		TEST_ASSERT(entryTable[i]->vector[3] == 0.0);
		            	// for json with name "roses"	
		            	}else if ( strcmp(entryTable[i]->path,"roses") == 0 ){
		            		// check values of the vector
		            		/* NOTE: we check absolute difference because the equality depents
		            		on number of digits displayed and the float error*/
		            		TEST_ASSERT(entryTable[i]->vector[0] == 0.0);
		            		TEST_ASSERT(entryTable[i]->vector[1] == 0.0);
		            		TEST_ASSERT(entryTable[i]->vector[2] == 0.0);
		            		TEST_ASSERT(abs(entryTable[i]->vector[3] - 0.301030) < 0.0000001);
		            	// no other file should be found	
		            	}else TEST_ASSERT(false);
		            }
		        }
		        temp=temp->next;
		    }
		}
    }
     
	//________________________________________________________________________________________________________________

    destroyListOfWordInfo(vocabulary,(void*)wordInfoDeletion);
    destroyHT(ht,BUCKETSIZE);
    destroyHTVOC(htVOC,BUCKETSIZE);

	return;
}

//___test_making_tfidf_for_each_JSON(drop columns and then recompute tfidf)___________________________________________
void test_tfidf_vectorDROP(void){

    hashTableVOC* htVOC = createHTVOC(HTSIZEVOC);

    unsigned int size=0; 

    /* First sentence / .json */
    char* w1 = calloc(STRSIZE,sizeof(char)); strcpy(w1,"red");
    char* w2 = calloc(STRSIZE,sizeof(char)); strcpy(w2,"rose");
    char* w3 = calloc(STRSIZE,sizeof(char)); strcpy(w3,"is");
    char* w4 = calloc(STRSIZE,sizeof(char)); strcpy(w4,"red");
    
    /* WordInfoListA gets w1-w4  */
    node* wordInfoListA=NULL; 
    
    addToVoc(&vocabulary,w1, wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w1); 
   
    addToVoc(&vocabulary,w2,wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w2); 
   
    addToVoc(&vocabulary,w3,wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w3); 
  
    addToVoc(&vocabulary,w4,wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w4);
    
    /* Second sentence / .json */
    char* w5 = calloc(STRSIZE,sizeof(char)); strcpy(w5,"violet");
    char* w6 = calloc(STRSIZE,sizeof(char)); strcpy(w6,"is");
    char* w7 = calloc(STRSIZE,sizeof(char)); strcpy(w7,"red");
    char* w8 = calloc(STRSIZE,sizeof(char)); strcpy(w8,"and");
    char* w9 = calloc(STRSIZE,sizeof(char)); strcpy(w9,"blue");

    // WordInfoListB gets w5-w9 
    node* wordInfoListB=NULL; 

    addToVoc(&vocabulary,w5,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w5); 
    
    addToVoc(&vocabulary,w6,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w6); 
   
    addToVoc(&vocabulary,w7,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w7); 
    
    addToVoc(&vocabulary,w8,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w8);
 
    addToVoc(&vocabulary,w9,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w9); 

    // make two 'json'
    char* sA = calloc(STRSIZE,sizeof(char)); strcpy(sA,"roses");
    char* sB = calloc(STRSIZE,sizeof(char)); strcpy(sB,"violets");

    // initialize hash
    hashTable* ht = createHT(HTSIZE);
 	// add json into hash
	addtoHT(ht, sA, BUCKETSIZE, wordInfoListA);
    addtoHT(ht, sB, BUCKETSIZE, wordInfoListB);

    printf("\n");

    //_____ compute tfidf vector for every JSON file _________________________________________
    size = make_tfidf_vectorsDROP( ht, BUCKETSIZE, size, &vocabulary, 2, htVOC, BUCKETSIZEVOC );

    /* --------------------------------------------------------------
    After computing the tfidf we expect the 
    initial vocabulary -- 'blue	and	violet is rose red'
    to become          -- 'blue	and	violet rose'
    because the tfidf of the words 'is' and 'red' is zero(0.000), as 
    they exist in both json in our hash and they will be cut due to 
    the lower limit of the allowed average tfidf.

    So, BEFORE erasing the words from the vocabulary

	--  for json with name "violets" we expect the tfidf vector to be:
		0.060206	0.060206	0.060206	0.000000	0.000000	0.000000
	--  and for json with name "roses" we expect the tfidf vector to be: 
    	0.000000	0.000000	0.000000	0.000000	0.075258	0.000000

	and AFTER erasing the words from the vocabulary(in this function we do not recompute):
	--  for json with name "violets" we expect the tfidf vector to be:
		0.060206	0.060206	0.060206	0.000000
    --  and for json with name "roses" we expect the tfidf vector to be: 
    	0.000000	0.000000	0.000000	0.075258							
    ------------------------------------------------------------------*/

    // go through the hash and check the above expectation
    unsigned int numOfEntries =(BUCKETSIZE-sizeof(bucket*))/sizeof(bucketEntry*);

    for( unsigned int i=0; i<ht->size; i++ ){
        if(ht->table[i] != NULL){ 
		    node* temp = ht->table[i];
		    while(temp!=NULL){
		        bucketEntry** entryTable = temp->data;
		        for(unsigned int i = 0;i<numOfEntries;i++){
		            if((entryTable[i]!=NULL)){
		            	// for json with name "violets"
		            	if( strcmp(entryTable[i]->path,"violets") == 0 ){
		            		// check values of the vector
		            		/* NOTE: we check absolute difference because the equality depents
		            		on number of digits displayed and the float error*/
		            		TEST_ASSERT(abs(entryTable[i]->vector[0] - 0.060206) < 0.0000001);
		            		TEST_ASSERT(abs(entryTable[i]->vector[1] - 0.060206) < 0.0000001);
		            		TEST_ASSERT(abs(entryTable[i]->vector[2] - 0.060206) < 0.0000001);
		            		TEST_ASSERT(entryTable[i]->vector[3] == 0.0);
		            	// for json with name "roses"	
		            	}else if ( strcmp(entryTable[i]->path,"roses") == 0 ){
		            		// check values of the vector
		            		/* NOTE: we check absolute difference because the equality depents
		            		on number of digits displayed and the float error*/
		            		TEST_ASSERT(entryTable[i]->vector[0] == 0.0);
		            		TEST_ASSERT(entryTable[i]->vector[1] == 0.0);
		            		TEST_ASSERT(entryTable[i]->vector[2] == 0.0);
		            		TEST_ASSERT(abs(entryTable[i]->vector[3] - 0.075258) < 0.0000001);
		            	// no other file should be found	
		            	}else TEST_ASSERT(false);
		            }
		        }
		        temp=temp->next;
		    }
		}
    }
   
	//________________________________________________________________________________________________________________

    destroyListOfWordInfo(vocabulary,(void*)wordInfoDeletion);
    destroyHT(ht,BUCKETSIZE);
    destroyHTVOC(htVOC,BUCKETSIZE);

	return;
}

TEST_LIST = {
	{ "test_bow_vector", test_bow_vector },
	{ "test_tfidf_vectorDROPnRECOMPUTE", test_tfidf_vectorDROPnRECOMPUTE },
	{ "test_tfidf_vectorDROP", test_tfidf_vectorDROP },
	{ NULL, NULL } // end of tests
};