#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define STRSIZE 50

char* convertPath(char* _path_ ){

	char* adjustedPath = calloc(strlen(_path_)+2,sizeof(char)); assert( adjustedPath != NULL);

	unsigned int j=0;

	for( unsigned int i=0; _path_[i] != '\0'; i++ )

		if( _path_[i]=='/' ){
			adjustedPath[j++]='/';
			adjustedPath[j++]='/';
		}else
			adjustedPath[j++]=_path_[i];
		

	adjustedPath[j]='\0';

	return adjustedPath;
}


int main(void){

	char* strNew;
	char* str = calloc(STRSIZE,sizeof(char)); assert(str!=NULL);
	strcpy(str,"www.ebay.com/1");

	strNew=convertPath(str);

	printf("%s\n",strNew );

	free(strNew); strNew=NULL;
	free(str); str=NULL;

	return 0;
}