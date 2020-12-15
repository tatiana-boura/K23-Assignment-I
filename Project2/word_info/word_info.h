#ifndef __WORD_INFO__
#define __WORD_INFO__

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "word_info_header.h"

void wordInfoInitialization(wordInfo* w, char* _word_);
void wordInfoDeletion(wordInfo* w);
void printWordInfo(wordInfo* w);

#endif
