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
#define STRSIZE 100

//Compile:  gcc -g -o test ./taniaTest.c ../list/list.c ../word_info/word_info.c ./hash_table.c ../vocabulary/vocabulary.c 
//Run:   valgrind -s -q --leak-check=full --show-leak-kinds=all ./test

node* vocabulary = NULL;


int main(int argc, char* argv[]){
    wordInfo* temp_word_info;

    /* First sentence / .json */
    char* w1 = calloc(100,sizeof(char)); strcpy(w1,"I");
    char* w2 = calloc(100,sizeof(char)); strcpy(w2,"read");
    char* w3 = calloc(100,sizeof(char)); strcpy(w3,"a");
    char* w4 = calloc(100,sizeof(char)); strcpy(w4,"very");
    char* w5 = calloc(100,sizeof(char)); strcpy(w5,"good");
    char* w6 = calloc(100,sizeof(char)); strcpy(w6,"read");

    /* WordInfoListA gets w1-w6  */
    node* wordInfoListA=NULL; 
    
    addToVoc(&vocabulary,w1);
    addToWordInfoList(&wordInfoListA,w1); 
   
    addToVoc(&vocabulary,w2);
    addToWordInfoList(&wordInfoListA,w2); 
   
    addToVoc(&vocabulary,w3);
    addToWordInfoList(&wordInfoListA,w3); 
  
    addToVoc(&vocabulary,w4);
    addToWordInfoList(&wordInfoListA,w4);
    
    addToVoc(&vocabulary,w5);
    addToWordInfoList(&wordInfoListA,w5); 
   
    addToVoc(&vocabulary,w6);
    addToWordInfoList(&wordInfoListA,w6); 

    /* Second sentence / .json */
    char* w7 = calloc(100,sizeof(char)); strcpy(w7,"Thank");
    char* w8 = calloc(100,sizeof(char)); strcpy(w8,"you");
    char* w9 = calloc(100,sizeof(char)); strcpy(w9,"very");
    char* w10 = calloc(100,sizeof(char)); strcpy(w10,"very");
    char* w11 = calloc(100,sizeof(char)); strcpy(w11,"much");

    // WordInfoListB gets w7-w11 
    node* wordInfoListB=NULL; 
    
    
    addToVoc(&vocabulary,w7);
    addToWordInfoList(&wordInfoListB,w7); 

    
    addToVoc(&vocabulary,w8);
    addToWordInfoList(&wordInfoListB,w8); 

   
    addToVoc(&vocabulary,w9);
    addToWordInfoList(&wordInfoListB,w9); 

    
    addToVoc(&vocabulary,w10);
    addToWordInfoList(&wordInfoListB,w10);

    
    addToVoc(&vocabulary,w11);
    addToWordInfoList(&wordInfoListB,w11); 

    printList(vocabulary,(void*)printWordInfo);printf("\n");printf("\n");

    char* sA = calloc(100,sizeof(char)); strcpy(sA,"book");
	char* sB = calloc(100,sizeof(char)); strcpy(sB,"thanks");

   

    //___INITIALIZING_HASH_TABLE_________________________________________________________________________________________

    hashTable* ht = createHT(HTSIZE);

	addtoHT(ht, sA, BUCKETSIZE, wordInfoListA);
    addtoHT(ht, sB, BUCKETSIZE, wordInfoListB);

    make_tfidf_vectors( ht, BUCKETSIZE, 8, vocabulary );

    //__END_INITIALIZING_HASH_TABLE____________________________________________________________________________________

    printHT(ht); 
    destroyListOfWordInfo(vocabulary,(void*)wordInfoDeletion);
	//________________________________________________________________________________________________________________

    destroyHT(ht,BUCKETSIZE);
}