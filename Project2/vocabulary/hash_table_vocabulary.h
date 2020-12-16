#ifndef _HASH_H_VOC_
#define _HASH_H_VOC_
#include <stdlib.h>
#include <string.h>
#include "../list/list.h"
#include "../list/node.h"
#include "../word_info/word_info.h"

typedef node bucket;

typedef struct hashTableVOC{
    bucket** table;
    unsigned int size;
}hashTableVOC;

typedef struct bucketEntryVOC{
	char* key;
    wordInfo* w; 
}bucketEntryVOC;

unsigned int hashVOC(char* string, hashTableVOC* ht);

hashTableVOC* createHTVOC(unsigned int size);

bucket* getBucketVOC(hashTableVOC* ht, char* key, unsigned int* index);

bucketEntryVOC* createEntryVOC( char* _key_, wordInfo* _w_);

void addtoHTVOC(hashTableVOC* ht, char* key, unsigned int bucketSize, wordInfo* w);

void destroyHTVOC(hashTableVOC* ht,unsigned int bucketSize);
void deleteBucketTableVOC(bucketEntryVOC** table, unsigned int* bucketSize);

bool foundInHTVOC(hashTableVOC* ht, char* _path_, unsigned int bucketSize);

void printHTVOC(hashTableVOC* ht, unsigned int bucketSize);
void printBucketVOC(node* b, unsigned int bucketSize);




#endif
