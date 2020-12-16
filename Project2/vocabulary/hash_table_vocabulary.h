#ifndef _HASH_H_VOC_
#define _HASH_H_VOC_
#include <stdlib.h>
#include <string.h>
#include "../list/list.h"
#include "../../tuples/tuples.h"
#include "../word_info/word_info.h"
#include "bucket.h"


typedef struct hashTable{
    node** buckets;
    unsigned int size;
}hashTable;

unsigned int hash(char* string, hashTable* ht);
hashTable* createHT(unsigned int size);
void addtoHT(hashTable* ht, char* key, unsigned int bucketSize,node* _wordInfoList_ );
void destroyHT(hashTable* ht,unsigned int bucketSize);
void deleteBucketTable(bucketEntry** table,unsigned int* bucketSize);

bool foundInHT(hashTable* ht, char* _path_, unsigned int bucketSize, unsigned int* entryNum, bucket** bucketFound );

void printHT(hashTable* ht);
void printBucket(node* b);

bucket* getBucket(hashTable* ht, char* key, unsigned int* index);
bucketEntry* createEntry(char* key,node* _wordInfoList_);


#endif
