#ifndef __BUCKET_H__
#define __BUCKET_H__

#include "../list/node.h"

typedef struct bucketEntry{
    char* path;
    node* wordInfoList;
    node* clique;
    node* notClique;
    float* tfidf; 
}bucketEntry;

#endif