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

#include "../list/list.h"
#include "../vocabulary/vocabulary.h"


char* convertPath(char* _path_ );
int count_json(char* given_directory);

void json_to_word_list(char* str, node** l, node* stopwords, node** vocabulary,unsigned int* vocabSize);  
void json_to_word_list_value_array_edition(char* str,  node** l, node* stopwords, node** vocabulary,unsigned int* vocabSize);
char* no_symbols(char* str);
void createTFIDFarray(float*** array,hashTable* ht,unsigned int bucketSize, unsigned int vocabSize,unsigned int* n);


#endif
