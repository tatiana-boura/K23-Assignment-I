#ifndef __BUCKET_H__
#define __BUCKET_H__

#include "../list/node.h"

typedef struct bucketEntry{
    char* path;
    node* listOfTuples;
    node* clique;
    node* notClique; 
}bucketEntry;

#endif