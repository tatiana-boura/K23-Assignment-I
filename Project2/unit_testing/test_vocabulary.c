#include "../../unit_testing/acutest.h"

#include "../vocabulary/vocabulary.h"

#define HTSIZE 1
#define HTSIZEVOC 2
#define STRSIZE 20
#define BUCKETSIZE 24


node* vocabulary = NULL;

//___test_add_VOC_and_wordInfoList_______________________________________________
void test_add_VOC_and_wordInfoList(void){

    hashTableVOC* htVOC = createHTVOC(HTSIZEVOC);

    unsigned int size=0; 

    wordInfo* temp_word_info;

    /*_____________ First sentence / .json ______________________*/
    char* w1 = calloc(STRSIZE,sizeof(char)); strcpy(w1,"red");
    char* w2 = calloc(STRSIZE,sizeof(char)); strcpy(w2,"yellow");
    char* w3 = calloc(STRSIZE,sizeof(char)); strcpy(w3,"pink");
    char* w4 = calloc(STRSIZE,sizeof(char)); strcpy(w4,"blue");
    char* w5 = calloc(STRSIZE,sizeof(char)); strcpy(w5,"yellow");

    /* WordInfoListA gets w1-w5  */
    node* wordInfoListA=NULL; 
    
    addToVoc(&vocabulary,w1, wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w1); 
   
    addToVoc(&vocabulary,w2,wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w2); 
   
    addToVoc(&vocabulary,w3,wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w3); 
  
    addToVoc(&vocabulary,w4,wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w4);
    
    addToVoc(&vocabulary,w5,wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w5); 

    /*_____________ Second sentence / .json ____________________*/
    char* w6 = calloc(STRSIZE,sizeof(char)); strcpy(w6,"black");
    char* w7 = calloc(STRSIZE,sizeof(char)); strcpy(w7,"blue");
    char* w8 = calloc(STRSIZE,sizeof(char)); strcpy(w8,"blue");
    char* w9 = calloc(STRSIZE,sizeof(char)); strcpy(w9,"green");

    // WordInfoListB gets w6-w9 
    node* wordInfoListB=NULL; 
    
    addToVoc(&vocabulary,w6,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w6); 

    addToVoc(&vocabulary,w7,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w7); 

    addToVoc(&vocabulary,w8,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w8); 

    addToVoc(&vocabulary,w9,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w9); 

    // START TESTING

    // check if the vocabulary size is as expected
    TEST_ASSERT( size == 6 );

    /* here words are expected to be alphabeticaly sorted
       and yellow is seen two times */
    node* tempA = wordInfoListA;
    for( unsigned int i = 0; i < 4; i++ ){

    	wordInfo* w = (wordInfo*)tempA->data;
    	
    	if(i==0){
    		TEST_ASSERT(strcmp(w->word,"blue")==0);
    		TEST_ASSERT(w->count==1);
    	}else if(i==1){
    		TEST_ASSERT(strcmp(w->word,"pink")==0);
    		TEST_ASSERT(w->count==1);
    	}else if(i==2){
    		TEST_ASSERT(strcmp(w->word,"red")==0);
    		TEST_ASSERT(w->count==1);
    	}else{
    		TEST_ASSERT(strcmp(w->word,"yellow")==0);
    		TEST_ASSERT(w->count==2);
    	}

    	tempA = tempA->next;
    }

    /* here words are expected to be alphabeticaly sorted
       and blue is seen two times */
    node* tempB = wordInfoListB;
    for( unsigned int i = 0; i < 3; i++ ){

    	wordInfo* w = (wordInfo*)tempB->data;
    	// words should be sorted
    	if(i==0){
    		TEST_ASSERT(strcmp(w->word,"black")==0);
    		TEST_ASSERT(w->count==1);
    	}else if(i==1){
    		TEST_ASSERT(strcmp(w->word,"blue")==0);
    		TEST_ASSERT(w->count==2);
    	}else{
    		TEST_ASSERT(strcmp(w->word,"green")==0);
    		TEST_ASSERT(w->count==1);
    	}

    	tempB = tempB->next;
    }

    /* this list is append front 
       blue is seen in both 'json'*/
    node* temp = vocabulary;
    for( unsigned int i = 0; i < 6; i++ ){

    	wordInfo* w = (wordInfo*)temp->data;
    	// list is append front
    	if(i==0){
    		TEST_ASSERT(strcmp(w->word,"green")==0);
    		TEST_ASSERT(w->count==1);
    	}else if(i==1){
    		TEST_ASSERT(strcmp(w->word,"black")==0);
    		TEST_ASSERT(w->count==1);
    	}else if(i==2){
    		TEST_ASSERT(strcmp(w->word,"blue")==0);
    		TEST_ASSERT(w->count==2);
    	}else if(i==3){
    		TEST_ASSERT(strcmp(w->word,"pink")==0);
    		TEST_ASSERT(w->count==1);
    	}else if(i==4){
    		TEST_ASSERT(strcmp(w->word,"yellow")==0);
    		TEST_ASSERT(w->count==1);
    	}else{
    		TEST_ASSERT(strcmp(w->word,"red")==0);
    		TEST_ASSERT(w->count==1);
    	}

    	temp = temp->next;
    }

    // free not needed memory
    destroyListOfWordInfo(wordInfoListA,(void*)wordInfoDeletion);
    destroyListOfWordInfo(wordInfoListB,(void*)wordInfoDeletion);
    destroyListOfWordInfo(vocabulary,(void*)wordInfoDeletion);
    destroyHTVOC(htVOC,BUCKETSIZE);

    return;

}


TEST_LIST = {
	{ "test_vocab", test_add_VOC_and_wordInfoList },
	{ NULL, NULL } // end of tests
};