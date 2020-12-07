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

#include "../../tuples/tuples.h"
#include "../list/list.h"


char* convertPath(char* _path_ );
int count_json(char* given_directory);
void json_separator(char* str, TuplePtr t);
void json_array_handler(char* string, TuplePtr t);

void json_to_word_list(char* str, node** l);  
void json_to_word_list_value_array_edition(char* str,  node** l);

#endif
