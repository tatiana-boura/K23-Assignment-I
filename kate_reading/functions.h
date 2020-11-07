//functions.h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>
#include <stdlib.h>


char* convertPath(char* _path_ );
int count_json(char* given_directory);
void json_separator(char* str);
void json_array_handler(char* string);
