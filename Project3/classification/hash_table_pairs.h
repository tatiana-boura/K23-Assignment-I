#ifndef _HASH_H_PAIR_
#define _HASH_H_PAIR_
#include <stdlib.h>
#include <string.h>
#include "../../Project2/list/list.h"
#include "../../Project2/list/node.h"

typedef node bucket;

typedef struct hashTablePair{
    bucket** table;
    unsigned int size;
}hashTablePair;

typedef struct bucketEntryPair{
	char* key1;
	char* key2; 
}bucketEntryPair;

unsigned int hashPair(char* string1, char* string2, hashTablePair* ht);

hashTablePair* createHTPair(unsigned int size);

bucket* getBucketPair(hashTablePair* ht, char* key1, char* key2, unsigned int* index);

bucketEntryPair* createEntryPair( char* _key1_,  char* _key2_);

void addtoHTPair(hashTablePair* ht, char* key1, char* key2, unsigned int bucketSize );

void destroyHTPair(hashTablePair* ht,unsigned int bucketSize);
void deleteBucketTablePair(bucketEntryPair** table, unsigned int* bucketSize);

bool foundInHTPair( hashTablePair* ht, char* _key1_, char* _key2_, unsigned int bucketSize );

void printHTPair(hashTablePair* ht, unsigned int bucketSize);
void printBucketPair(node* b, unsigned int bucketSize);

#endif
