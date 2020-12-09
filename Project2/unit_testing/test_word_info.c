#include "../../unit_testing/acutest.h"

#include "../word_info/word_info.h"
#include "../list/list.h"

#define STRSIZE 100

//___test_wordInfoInitialization_________________________________________________________
void test_wordInfoInitialization(void){
	/* this is a test function for testing the initialization of a wordList */

	// make two strings
	char* _word_=calloc(STRSIZE,sizeof(char)); strcpy(_word_,"word");

	// make wordInfo and initialize it
	wordInfo* w=calloc(1,sizeof(wordInfo));
	wordInfoInitialization(w,_word_);

	// check if values are inserted as expected
	TEST_ASSERT(!strcmp(w->word,_word_));

	wordInfoDeletion(w);	

	return;
}

TEST_LIST = {
	{ "wordInfoInit", test_wordInfoInitialization },
	{ NULL, NULL } // end of tests
};