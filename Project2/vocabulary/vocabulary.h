#include "../hash_table/hash_table.h"

void addWordInfoToList(node** list,wordInfo* _word_info_);
void addToWordInfoList(node** word_info_list,char* _word_);
void addToVoc(node** voc,char* _word_, node* word_info_list,unsigned int* vocabSize);
