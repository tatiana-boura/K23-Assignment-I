#ifndef __WORD_INFO_HEADER__
#define __WORD_INFO_HEADER__

typedef struct wordInfo{
	char* word;
	unsigned int count;	/*wordInfoList in HT: # of times word appeared in file | vocabulary: # of files that containted the word */
}wordInfo;

#endif