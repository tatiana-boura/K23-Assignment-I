#ifndef __WORD_INFO__
#define __WORD_INFO__

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct wordInfo{
	char* word;
	unsigned int count;
}wordInfo;

void wordInfoInitialization(wordInfo* w, char* _word_);
void wordInfoDeletion(wordInfo* w);
void printWordInfo(wordInfo* w);

#endif