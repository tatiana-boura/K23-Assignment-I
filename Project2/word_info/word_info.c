#include "word_info.h"

#define NAME_SIZE 100
#define VALUE_SIZE 300

/* WordInfoList: if word foundinlist then node->count++ else wordInfo* w; wordInfoInitialization(w,word); list=appendlist(list,w); */

void wordInfoInitialization(wordInfo* w, char* _word_){ 

	w->word=_word_;
	w->count = 1;

	return;
}

void wordInfoDeletion(wordInfo* w){ 

	free(w->word); w->word=NULL;
	free(w); w=NULL;

	return;
}

void printWordInfo(wordInfo* w){

	if(w==NULL){ printf("NULL wordInfo\n"); return;}

	printf("[word] = %s,[count] = %d ",w->word,w->count);

	return;
}