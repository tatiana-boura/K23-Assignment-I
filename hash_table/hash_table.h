#ifndef _HASH_H_
#define _HASH_H_
#include <stdlib.h>
#include <string.h>
#include "../list/list.h"
#include "../tuples/tuples.h"

typedef struct bucketEntry{
    char* path;
    node* listOfTuples;
    node* clique; 
}bucketEntry;

typedef node bucket;

typedef struct hashTable{
    bucket** table;
    unsigned int size;
}hashTable;

unsigned int hash(char* string, hashTable* ht);
hashTable* createHT(unsigned int size);
void addtoHT(hashTable* ht, char* key, unsigned int bucketSize,node* _listOfTuples_);
void destroyHT(hashTable* ht,unsigned int bucketSize);
void deleteBucketTable(bucketEntry** table,unsigned int* bucketSize);

bool foundInHT(hashTable* ht, char* _path_, unsigned int bucketSize, unsigned int* entryNum, bucket** bucketFound );
void changePointers(hashTable* ht, unsigned int bucketSize,bucket** bucketFound1, unsigned int entryNum1, bucket** bucketFound2, unsigned int entryNum2 );

void printHT(hashTable* ht);
void printBucket(node* b);

bucket* getBucket(hashTable* ht, char* key, int* index);
bucketEntry* createEntry(char* key,node* _listOfTuples_);

#endif