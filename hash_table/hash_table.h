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


#endif