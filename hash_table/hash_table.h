#ifndef _HASH_H_
#define _HASH_H_
#include <stdlib.h>
#include <string.h>
#include "../list/list.h"

typedef struct bucketEntry{
    char* path;
}bucketEntry;

typedef node bucket;

typedef struct hashTable{
    bucket** table;
    unsigned int size;
}hashTable;

unsigned int hash(char* string, hashTable* ht);
hashTable* createHT(unsigned int size);
bucketEntry* createEntry(char* key);
bucket* getBucket(hashTable* ht, char* key, int* index);
bool foundInHT(hashTable* ht,bucket** bp,char* key,unsigned int numOfEntries,int* index);
void addtoHT(hashTable* ht, char* key, unsigned int bucketSize);
void destroyHT(hashTable* ht,unsigned int bucketSize);
void deleteBucketTable(bucketEntry** table,unsigned int* bucketSize);
void printBucket(node* buck);

#endif