#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../hash_table/bucket.h"
#include "node.h"
#include "../../Project2/word_info/word_info.h" 

node* appendList(node* n, void* data);
node* appendListEnd(node* n, void* data);

void destroyList(node* n, unsigned int* size, bool del, void(*deleteData)(void*,void*));
node* mergeTwoLists(node* n1, node* n2);

void destroyListOfWordInfo(node* n, void(*deleteData)(void*));
void destroyListOfStrings(node* n, bool destroyDataAsWell);

void printList(node* n, void(*printData)(void*));

bool addrFoundinList(node* n,  void* addr );

void makeOutputFileList(node* n,FILE* outputFile);

node* removeNode(node** n, void* addr);
node* pop(node** n);

bool notInlist(node* n, char* w);

void sortedInsertStr(node** n, wordInfo* w);

bool foundInSortedListStr(node* n, char* w, bool increaseCounter);

void deleteWords( node** n, bool should_be_dropped[], unsigned int vocabSize );

#endif
