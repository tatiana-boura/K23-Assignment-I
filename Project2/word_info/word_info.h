#ifndef __WORD_INFO__
#define __WORD_INFO__

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct wordInfo{
	char* word;
	unsigned int count;	/*wordInfoList in HT: # of times word appeared in file | vocabulary: # of files that containted the word */
}wordInfo;

void wordInfoInitialization(wordInfo* w, char* _word_);
void wordInfoDeletion(wordInfo* w);
void printWordInfo(wordInfo* w);

#endif