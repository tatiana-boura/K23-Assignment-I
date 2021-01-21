#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>  

#include "../../Project2/list/list.h"
#include "../../Project2/vocabulary/vocabulary.h"
#include "../classification/data_for_training.h"

char* convertPath(char* _path_ );
int count_json(char* given_directory);

void json_to_wordInfo_list(char* file,  node** l, node* stopwords,node** vocabulary,unsigned int* vocabSize, hashTableVOC* htVOC, unsigned int bucketSize);
char* str_preprocess_symbols(char* str);

#endif
