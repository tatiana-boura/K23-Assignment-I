#include "vocabulary.h"

/* Returns NULL when there's duplicate - wordInfo* created to insert to vocabulary otherwise*/
void addToWordInfoList(node** word_info_list,char* _word_){
    bool found = false; 
    wordInfo* w = NULL;

    found = foundInSortedListStr(*word_info_list, _word_ , true);
    if(found){
        free(_word_);
    }

    if(!found){
        // Initialise wordInfo struct and add to wordInfoList
        w = calloc(1,sizeof(wordInfo));
        wordInfoInitialization(w,_word_);

        sortedInsertStr(word_info_list,w);
    } 
}

void addToVoc(node** voc, char* _word_, node* word_info_list, unsigned int* vocabSize, hashTableVOC* htVOC, unsigned int bucketSize){ 
    // If _word_ has been added in word_info_list then it 
    // already represents the .json in vocabulary count
    bool found = false; wordInfo* voc_w = NULL;

    found = foundInSortedListStr(word_info_list, _word_, false);
    if(found) return; 

    /* Make sure there is no duplicate of the word we try to add*/

    found = foundInHTVOC(htVOC, _word_, bucketSize);

    //found = foundInSortedListStr(*voc, _word_, true);
    if(!found){
        voc_w = calloc(1,sizeof(wordInfo));
        char* new_word = calloc(strlen(_word_)+1,sizeof(char*));
        strcpy(new_word,_word_);
        wordInfoInitialization(voc_w,new_word);

        addtoHTVOC(htVOC, _word_, bucketSize, voc_w);

        //sortedInsertStr(voc,voc_w);
        *voc = appendList(*voc,voc_w);  
        (*vocabSize)++;    
    } 
}
