#include "../../unit_testing/acutest.h"

#include "../classification/hash_table_pairs.h"

#define STRSIZE 50
#define BUCKETSIZEPAIR 30
#define HTSIZEPAIR 2

//___test_createHT_________________________________________________________
void test_createHTPair(void){

    hashTablePair* htPair = createHTPair(HTSIZEPAIR);

    TEST_ASSERT(htPair!=NULL);
    TEST_ASSERT(htPair->table!=NULL);
    TEST_ASSERT(htPair->size==HTSIZEPAIR);

	destroyHTPair(htPair,BUCKETSIZEPAIR);

	return;
}

//___test_addtoHT_________________________________________________________
void test_addtoHTPair(void){
    unsigned int size = 0;
    hashTablePair* htPair = createHTPair(HTSIZEPAIR);

    //create path keys
    char* key1 = calloc(STRSIZE,sizeof(char)); strcpy(key1,"buy.net//12");
    char* key2 = calloc(STRSIZE,sizeof(char)); strcpy(key2,"www.ebay.com//253");
    char* key3 = calloc(STRSIZE,sizeof(char)); strcpy(key3,"www.walmart.com//697");
    char* key4 = calloc(STRSIZE,sizeof(char)); strcpy(key4,"www.camerafarm.com.au//1226");

    //insert pairs 
    addtoHTPair(htPair, key1, key2, BUCKETSIZEPAIR );  //key1 - key2
    addtoHTPair(htPair, key3, key4, BUCKETSIZEPAIR );  //key3 - key4

    
    // START TESTING

    // go through the hash and get the values
    unsigned int numOfEntries =(BUCKETSIZEPAIR-sizeof(bucket*))/sizeof(bucketEntryPair*);

    for( unsigned int i=0; i<htPair->size; i++ ){

        if(htPair->table[i] != NULL){
            node* temp = htPair->table[i];
            while(temp!=NULL){
                bucketEntryPair** entryTable = temp->data;
                for(int i = 0;i<numOfEntries;i++){
                    if((entryTable[i]!=NULL)){
                        // check that inserted values are the expected
                        TEST_ASSERT( (!strcmp(entryTable[i]->key1,key1) && !strcmp(entryTable[i]->key2,key2) ) || (!strcmp(entryTable[i]->key1,key3) && !strcmp(entryTable[i]->key2,key4) ) );
                    }
                }
                temp=temp->next;
            }
        }
    }
    //free not needed memory
    destroyHTPair(htPair,BUCKETSIZEPAIR);

    //keys are not freed by destroyHTPair() 
    //they would have been freed by the array deletion tin the program 
    free(key1);  
    free(key2);
    free(key3);
    free(key4);

    return;
}

//___test_foundInHT______________________________________________________________
void test_foundInHTPair(void){

    hashTablePair* htPair = createHTPair(HTSIZEPAIR);

    //create path keys
    char* key1 = calloc(STRSIZE,sizeof(char)); strcpy(key1,"buy.net//12");
    char* key2 = calloc(STRSIZE,sizeof(char)); strcpy(key2,"www.ebay.com//253");
    char* key3 = calloc(STRSIZE,sizeof(char)); strcpy(key3,"www.walmart.com//697");
    char* key4 = calloc(STRSIZE,sizeof(char)); strcpy(key4,"www.camerafarm.com.au//1226");
    
    //insert pairs 
    addtoHTPair(htPair, key1, key2, BUCKETSIZEPAIR );  //key1 - key2
    addtoHTPair(htPair, key3, key4, BUCKETSIZEPAIR );  //key3 - key4
    
    // START TESTING
    
    //check if the inserted pairs are found
    TEST_ASSERT(foundInHTPair(htPair,key1, key2, BUCKETSIZEPAIR) == true);
    //an inserted pair is not defined by the key order
    //thus by adding (key3,key4), (key4,key3) should be considered found too in the ht
    TEST_ASSERT(foundInHTPair(htPair,key3, key4, BUCKETSIZEPAIR) == true);
    TEST_ASSERT(foundInHTPair(htPair,key4, key3, BUCKETSIZEPAIR) == true);

    // test if (key1,key3) is found in hash table [should not be found]
    TEST_ASSERT(foundInHTPair(htPair,key1, key3, BUCKETSIZEPAIR) != true);

    // free not needed memo
    destroyHTPair(htPair,BUCKETSIZEPAIR);

    //keys are not freed by destroyHTPair() 
    //they would have been freed by the array deletion tin the program 
    free(key1);  
    free(key2);
    free(key3);
    free(key4);

    return;
}

TEST_LIST = {
	{ "createHTPair", test_createHTPair },
    { "addtoHTPair", test_addtoHTPair },
    { "foundInHTPair", test_foundInHTPair },
	{ NULL, NULL } // end of tests
};