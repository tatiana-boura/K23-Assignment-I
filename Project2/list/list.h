#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../hash_table/bucket.h"
#include "node.h"

node* appendList(node* n, void* data);
void destroyList(node* n, unsigned int* size, bool del, void(*deleteData)(void*,void*));
node* mergeTwoLists(node* n1, node* n2);

void destroyListOfTuples(node* n, void(*deleteData)(void*));
void destroyListOfStrings(node* n, bool destroyDataAsWell);

void printList(node* n, void(*printData)(void*));

bool addrFoundinList(node* n,  void* addr, bool _print_ );

void makeOutputFileList(node* n,FILE* outputFile);

node* deleteNode(node** n, void* addr);



#endif
