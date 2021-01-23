#include <math.h>

#include "../../Project1/unit_testing/acutest.h"

#include "../hash_table/hash_table.h"
#include "../vocabulary/vocabulary.h"
#include "../classification/data_for_training.c"

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
    
    addToVoc(&vocabulary,w1, wordInfoListA,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w1); 
   
    addToVoc(&vocabulary,w2,wordInfoListA,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w2); 
   
    addToVoc(&vocabulary,w3,wordInfoListA,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w3); 
  
    addToVoc(&vocabulary,w4,wordInfoListA,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w4);
    
    /* Second sentence / .json */
    char* w5 = calloc(STRSIZE,sizeof(char)); strcpy(w5,"violet");
    char* w6 = calloc(STRSIZE,sizeof(char)); strcpy(w6,"is");
    char* w7 = calloc(STRSIZE,sizeof(char)); strcpy(w7,"red");
    char* w8 = calloc(STRSIZE,sizeof(char)); strcpy(w8,"and");
    char* w9 = calloc(STRSIZE,sizeof(char)); strcpy(w9,"blue");

    // WordInfoListB gets w5-w9 
    node* wordInfoListB=NULL; 

    addToVoc(&vocabulary,w5,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w5); 
    
    addToVoc(&vocabulary,w6,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w6); 
   
    addToVoc(&vocabulary,w7,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w7); 
    
    addToVoc(&vocabulary,w8,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w8);
 
    addToVoc(&vocabulary,w9,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
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
		            	// for json with name "violets"		
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
    destroyHTVOC(htVOC,BUCKETSIZEVOC);

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
    
    addToVoc(&vocabulary,w1, wordInfoListA,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w1); 
   
    addToVoc(&vocabulary,w2,wordInfoListA,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w2); 
   
    addToVoc(&vocabulary,w3,wordInfoListA,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w3); 
  
    addToVoc(&vocabulary,w4,wordInfoListA,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w4);
    
    /* Second sentence / .json */
    char* w5 = calloc(STRSIZE,sizeof(char)); strcpy(w5,"violet");
    char* w6 = calloc(STRSIZE,sizeof(char)); strcpy(w6,"is");
    char* w7 = calloc(STRSIZE,sizeof(char)); strcpy(w7,"red");
    char* w8 = calloc(STRSIZE,sizeof(char)); strcpy(w8,"and");
    char* w9 = calloc(STRSIZE,sizeof(char)); strcpy(w9,"blue");

    // WordInfoListB gets w5-w9 
    node* wordInfoListB=NULL; 

    addToVoc(&vocabulary,w5,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w5); 
    
    addToVoc(&vocabulary,w6,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w6); 
   
    addToVoc(&vocabulary,w7,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w7); 
    
    addToVoc(&vocabulary,w8,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w8);
 
    addToVoc(&vocabulary,w9,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
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

    /* -----------------------------------------------------------------------
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
    ---------------------------------------------------------------------------*/

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
    destroyHTVOC(htVOC,BUCKETSIZEVOC);

	return;
}

//___test_making_tfidf_for_each_JSON(drop columns and not recompute tfidf)____________________________________________
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
    
    addToVoc(&vocabulary,w1, wordInfoListA,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w1); 
   
    addToVoc(&vocabulary,w2,wordInfoListA,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w2); 
   
    addToVoc(&vocabulary,w3,wordInfoListA,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w3); 
  
    addToVoc(&vocabulary,w4,wordInfoListA,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w4);
    
    /* Second sentence / .json */
    char* w5 = calloc(STRSIZE,sizeof(char)); strcpy(w5,"violet");
    char* w6 = calloc(STRSIZE,sizeof(char)); strcpy(w6,"is");
    char* w7 = calloc(STRSIZE,sizeof(char)); strcpy(w7,"red");
    char* w8 = calloc(STRSIZE,sizeof(char)); strcpy(w8,"and");
    char* w9 = calloc(STRSIZE,sizeof(char)); strcpy(w9,"blue");

    // WordInfoListB gets w5-w9 
    node* wordInfoListB=NULL; 

    addToVoc(&vocabulary,w5,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w5); 
    
    addToVoc(&vocabulary,w6,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w6); 
   
    addToVoc(&vocabulary,w7,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w7); 
    
    addToVoc(&vocabulary,w8,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w8);
 
    addToVoc(&vocabulary,w9,wordInfoListB,&size,htVOC, BUCKETSIZEVOC);
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

    /* ----------------------------------------------------------------------------------
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
    --------------------------------------------------------------------------------------*/

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
    destroyHTVOC(htVOC,BUCKETSIZEVOC);

	return;
}

//___test_making_x_and_y_test_for_all_json_____________________________________________________________________________
void test_create_x_y_array(void){

	/* In this function we will use BoW vectors but nothing about this 
	function(create_x_y_array()) would change if we used tfidf arrays. */

    hashTableVOC* htVOC = createHTVOC(HTSIZEVOC);

    unsigned int vocSize=0; 

    /* First sentence / .json */
    char* w1 = calloc(STRSIZE,sizeof(char)); strcpy(w1,"red");
    char* w2 = calloc(STRSIZE,sizeof(char)); strcpy(w2,"rose");
    char* w3 = calloc(STRSIZE,sizeof(char)); strcpy(w3,"is");
    char* w4 = calloc(STRSIZE,sizeof(char)); strcpy(w4,"red");
    
    /* WordInfoListA gets w1-w4  */
    node* wordInfoListA=NULL; 
    
    addToVoc(&vocabulary,w1, wordInfoListA,&vocSize,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w1); 
   
    addToVoc(&vocabulary,w2,wordInfoListA,&vocSize,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w2); 
   
    addToVoc(&vocabulary,w3,wordInfoListA,&vocSize,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w3); 
  
    addToVoc(&vocabulary,w4,wordInfoListA,&vocSize,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListA,w4);
    
    /* Second sentence / .json */
    char* w5 = calloc(STRSIZE,sizeof(char)); strcpy(w5,"violet");
    char* w6 = calloc(STRSIZE,sizeof(char)); strcpy(w6,"is");
    char* w7 = calloc(STRSIZE,sizeof(char)); strcpy(w7,"red");
    char* w8 = calloc(STRSIZE,sizeof(char)); strcpy(w8,"and");
    char* w9 = calloc(STRSIZE,sizeof(char)); strcpy(w9,"blue");

    // WordInfoListB gets w5-w9 
    node* wordInfoListB=NULL; 

    addToVoc(&vocabulary,w5,wordInfoListB,&vocSize,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w5); 
    
    addToVoc(&vocabulary,w6,wordInfoListB,&vocSize,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w6); 
   
    addToVoc(&vocabulary,w7,wordInfoListB,&vocSize,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w7); 
    
    addToVoc(&vocabulary,w8,wordInfoListB,&vocSize,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w8);
 
    addToVoc(&vocabulary,w9,wordInfoListB,&vocSize,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&wordInfoListB,w9); 

    /* Third sentence / .json */
    char* w10 = calloc(STRSIZE,sizeof(char)); strcpy(w10,"red");
    char* w11 = calloc(STRSIZE,sizeof(char)); strcpy(w11,"is");
    char* w12 = calloc(STRSIZE,sizeof(char)); strcpy(w12,"pretty");

    // WordInfoListC gets w10-w12 
    node* WordInfoListC=NULL; 

    addToVoc(&vocabulary,w10,WordInfoListC,&vocSize,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&WordInfoListC,w10); 
    
    addToVoc(&vocabulary,w11,WordInfoListC,&vocSize,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&WordInfoListC,w11); 
   
    addToVoc(&vocabulary,w12,WordInfoListC,&vocSize,htVOC, BUCKETSIZEVOC);
    addToWordInfoList(&WordInfoListC,w12); 

    // make three 'json'
    // A
    char* sA = calloc(STRSIZE,sizeof(char)); strcpy(sA,"roses");
    // B
    char* sB = calloc(STRSIZE,sizeof(char)); strcpy(sB,"violets");
    // C
    char* sC = calloc(STRSIZE,sizeof(char)); strcpy(sC,"pretty");

    // initialize hash
    hashTable* ht = createHT(HTSIZE);
 	// add json into hash
	addtoHT(ht, sA, BUCKETSIZE, wordInfoListA);
    addtoHT(ht, sB, BUCKETSIZE, wordInfoListB);
    addtoHT(ht, sC, BUCKETSIZE, WordInfoListC);

    //_____ compute BoW vector for every JSON file _________________________________________
    printf("\n");
    vocSize = make_BoW_vectorsDROP( ht, BUCKETSIZE, vocSize, &vocabulary, 3, htVOC, BUCKETSIZEVOC );

    /* --------------------------------------------------------------
    After computing the BoW we expect the 
    initial vocabulary -- 'pretty blue and violet is rose red'
    to become          -- 'pretty blue and violet rose'
    because the tfidf of the words 'is' and 'red' is zero(0.000), as 
    they exist in both json in our hash and they will be cut due to 
    the lower limit of the allowed average tfidf. 
	
	So, AFTER erasing the words from the vocabulary:

    for json with name "violets" we expect the BoW vector to be:
        0.000000	1.000000	1.000000	1.000000	0.000000
    as for json with name "roses" we expect the BoW vector to be: 
    	0.000000	0.000000	0.000000	0.000000	1.000000
    and for json with name "pretty" we expect the BoW vector to be:	
	    1.000000	0.000000	0.000000	0.000000	0.000000
    ------------------------------------------------------------------*/

    /* now create cliques and anti-cliques:
     			A 	B 	1
     			A 	C 	0				  */

    unsigned int entryNum1, entryNum2;
    bucket* bucketFound1;
    bucket* bucketFound2;

    // A 	B 	1
    bool found_left=false; bool found_right = false;
    for( unsigned int i=0; i<HTSIZE; i++ ){
        found_left = foundInHT(ht, sA, BUCKETSIZE, &entryNum1, &bucketFound1 );
        if(found_left) break;
    }
    for( unsigned int i=0; i<HTSIZE; i++ ){
        found_right = foundInHT(ht, sB, BUCKETSIZE, &entryNum2, &bucketFound2 );
        if(found_right) break;
    }
    changePointers(ht, BUCKETSIZE,&bucketFound1, entryNum1, &bucketFound2, entryNum2 );

    // A 	C 	0
    found_left=false; found_right = false;
    for( unsigned int i=0; i<HTSIZE; i++ ){
        found_left = foundInHT(ht, sA, BUCKETSIZE, &entryNum1, &bucketFound1 );
        if(found_left) break;
    }
    for( unsigned int i=0; i<HTSIZE; i++ ){
        found_right = foundInHT(ht, sC, BUCKETSIZE, &entryNum2, &bucketFound2 );
        if(found_right) break;
    }
    adjustPointers(ht, BUCKETSIZE,&bucketFound1, entryNum1, &bucketFound2, entryNum2 );

    // now make the array of absolute differences that we will use for training and validation
    float** x_array = calloc(1,sizeof(float*));
	unsigned int* y_array = calloc(1,sizeof(unsigned int));
	unsigned int n;	 // number of differences (size of x and y array)
	create_x_y_array(&x_array,& y_array,ht,BUCKETSIZE, vocSize,&n);

	/* we will be expecting 3 pairs: (A B 1),(A C 0),(B C 0) */
	TEST_ASSERT(n==3);

	for( unsigned int i=0; i<n; i++ ){
		if(i==0){
			// A B 1 -- BoW : |A-B|
			TEST_ASSERT(x_array[i][0]==0.0);
			TEST_ASSERT(x_array[i][1]==1.0);
			TEST_ASSERT(x_array[i][2]==1.0);
			TEST_ASSERT(x_array[i][3]==1.0);
			TEST_ASSERT(x_array[i][4]==1.0);
			// same clique
			TEST_ASSERT(y_array[i]==1);
		}else if(i==1){
			// A C 0 -- BoW : |A-C|
			TEST_ASSERT(x_array[i][0]==1.0);
			TEST_ASSERT(x_array[i][1]==0.0);
			TEST_ASSERT(x_array[i][2]==0.0);
			TEST_ASSERT(x_array[i][3]==0.0);
			TEST_ASSERT(x_array[i][4]==1.0);
			// anti-clique
			TEST_ASSERT(y_array[i]==0);
			// B C 0 -- BoW : |B-C|
		}else if(i==2){
			TEST_ASSERT(x_array[i][0]==1.0);
			TEST_ASSERT(x_array[i][1]==1.0);
			TEST_ASSERT(x_array[i][2]==1.0);
			TEST_ASSERT(x_array[i][3]==1.0);
			TEST_ASSERT(x_array[i][4]==0.0);
			// anti-clique
			TEST_ASSERT(y_array[i]==0);
		}else{
			// there should only be 3 rows
			TEST_ASSERT(false);
		}	
	}

	//________________________________________________________________________________________________________________

    for(unsigned int i=0;i<n;i++){
		free(x_array[i]);
	}
	free(x_array); free(y_array);
	makeOutputFile(ht, BUCKETSIZE);
    destroyListOfWordInfo(vocabulary,(void*)wordInfoDeletion);
    destroyHTVOC(htVOC,BUCKETSIZEVOC);
    destroyHT(ht,BUCKETSIZE);
   
	return;
}

TEST_LIST = {
	{ "test_bow_vector", test_bow_vector },
	{ "test_tfidf_vectorDROPnRECOMPUTE", test_tfidf_vectorDROPnRECOMPUTE },
	{ "test_tfidf_vectorDROP", test_tfidf_vectorDROP },
	{ "test_create_x_y_array", test_create_x_y_array },
	{ NULL, NULL } // end of tests
};