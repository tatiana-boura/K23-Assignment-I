#ifndef _HASH_H_
#define _HASH_H_
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "../list/list.h"
#include "../../tuples/tuples.h"
#include "../word_info/word_info.h"
#include "../vocabulary/hash_table_vocabulary.h"
#include "bucket.h"

typedef node bucket;

typedef struct hashTable{
    bucket** table;
    unsigned int size;
}hashTable;

unsigned int hash(char* string, hashTable* ht);
hashTable* createHT(unsigned int size);
void addtoHT(hashTable* ht, char* key, unsigned int bucketSize,node* _wordInfoList_ );
void destroyHT(hashTable* ht,unsigned int bucketSize);
void deleteBucketTable(bucketEntry** table,unsigned int* bucketSize);

bool foundInHT(hashTable* ht, char* _path_, unsigned int bucketSize, unsigned int* entryNum, bucket** bucketFound );
void changePointers(hashTable* ht, unsigned int bucketSize, bucket** bucketFound1, unsigned int entryNum1, bucket** bucketFound2, unsigned int entryNum2 );
void adjustPointers(hashTable* ht, unsigned int bucketSize, bucket** bucketFound1, unsigned int entryNum1, bucket** bucketFound2, unsigned int entryNum2 );

void printHT(hashTable* ht);
void printBucket(node* b);

bucket* getBucket(hashTable* ht, char* key, unsigned int* index);
bucketEntry* createEntry(char* key,node* _wordInfoList_);

unsigned int  make_tfidf_vectorsDROP( hashTable* ht, unsigned int bucketSize, unsigned int vocabSize, node* vocabulary, unsigned int numOfJSON, hashTableVOC* htVOC, unsigned int bucketSizeVOC );
void make_BoW_vectors( hashTable* ht, unsigned int bucketSize, unsigned int vocabSize, node* vocabulary, unsigned int numOfJSON, hashTableVOC* htVOC, unsigned int bucketSizeVOC );
unsigned int make_tfidf_vectorsDROPnRECOMPUTE( hashTable* ht, unsigned int bucketSize, unsigned int vocabSize, node* vocabulary, unsigned int numOfJSON, hashTableVOC* htVOC, unsigned int bucketSizeVOC );



void makeOutputFile(hashTable* ht, unsigned int bucketSize);

#endif
