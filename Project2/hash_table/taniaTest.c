#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <dirent.h>
#include "../vocabulary/vocabulary.h"

#define BUFFER_SIZE 51000
#define BUCKETSIZE 200
#define HTSIZE 20
#define HTSIZEVOC 3
#define STRSIZE 100

//Compile:  gcc -g -o test ./taniaTest.c ../list/list.c ../word_info/word_info.c ./hash_table.c ../vocabulary/vocabulary.c -lm 
//Run:   valgrind -s -q --leak-check=full --show-leak-kinds=all ./test

node* vocabulary = NULL;


int main(int argc, char* argv[]){

    hashTableVOC* htVOC = createHTVOC(HTSIZEVOC);

     unsigned int size=0; 


    wordInfo* temp_word_info;

    /* First sentence / .json */
    char* w1 = calloc(100,sizeof(char)); strcpy(w1,"i");
    char* w2 = calloc(100,sizeof(char)); strcpy(w2,"read");
    char* w3 = calloc(100,sizeof(char)); strcpy(w3,"a");
    char* w4 = calloc(100,sizeof(char)); strcpy(w4,"very");
    char* w5 = calloc(100,sizeof(char)); strcpy(w5,"good");
    char* w6 = calloc(100,sizeof(char)); strcpy(w6,"read");

    /* WordInfoListA gets w1-w6  */
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
   
    addToVoc(&vocabulary,w6,wordInfoListA,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListA,w6); 

    /* Second sentence / .json */
    char* w7 = calloc(100,sizeof(char)); strcpy(w7,"thank");
    char* w8 = calloc(100,sizeof(char)); strcpy(w8,"you");
    char* w9 = calloc(100,sizeof(char)); strcpy(w9,"very");
    char* w10 = calloc(100,sizeof(char)); strcpy(w10,"very");
    char* w11 = calloc(100,sizeof(char)); strcpy(w11,"much");

    // WordInfoListB gets w7-w11 
    node* wordInfoListB=NULL; 


    
    addToVoc(&vocabulary,w7,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w7); 

    
    addToVoc(&vocabulary,w8,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w8); 

   
    addToVoc(&vocabulary,w9,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w9); 

    
    addToVoc(&vocabulary,w10,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w10);

    
    addToVoc(&vocabulary,w11,wordInfoListB,&size,htVOC, BUCKETSIZE);
    addToWordInfoList(&wordInfoListB,w11); 

    printList(vocabulary,(void*)printWordInfo);printf("\n");printf("\n");

    printf("%d\n",size );


    char* sA = calloc(100,sizeof(char)); strcpy(sA,"book");
    char* sB = calloc(100,sizeof(char)); strcpy(sB,"thanks");

   

    //___INITIALIZING_HASH_TABLE_________________________________________________________________________________________

    hashTable* ht = createHT(HTSIZE);

	addtoHT(ht, sA, BUCKETSIZE, wordInfoListA);
    addtoHT(ht, sB, BUCKETSIZE, wordInfoListB);

    //make_tfidf_vectors( ht, BUCKETSIZE, 8, vocabulary, 2, htVOC, BUCKETSIZE  );
    make_tfidf_vectors( ht, BUCKETSIZE, 8, vocabulary, 2 );

    printf("\n");

    printHTVOC(htVOC,BUCKETSIZE);

    //__END_INITIALIZING_HASH_TABLE____________________________________________________________________________________

    printHT(ht); 
    destroyListOfWordInfo(vocabulary,(void*)wordInfoDeletion);
	//________________________________________________________________________________________________________________

    destroyHT(ht,BUCKETSIZE);
    destroyHTVOC(htVOC,BUCKETSIZE);
}