#include "word_info.h"

#define NAME_SIZE 100
#define VALUE_SIZE 300

void wordInfoInitialization(wordInfo* w, char* _word_){ 

	/*w = calloc(1,sizeof(wordInfo));
	assert(w!=NULL);*/

	w->word=_word_;
	w->count = 0;

	return;
}

void wordInfoDeletion(wordInfo* w){ 

	free(w->word); w->word=NULL;
	free(w); w=NULL;

	return;
}

void printWordInfo(wordInfo* w){

	if(w==NULL){ printf("NULL wordInfo\n"); return;}

	printf("[word] = [%s],[count] = %d\n",w->word,w->count);

	return;
}