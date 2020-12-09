#include "vocabulary.h"

/* Returns NULL when there's duplicate - wordInfo* created to insert to vocabulary otherwise*/
void addToWordInfoList(node** word_info_list,char* _word_){
    bool found = false; node* temp = *word_info_list; 
    wordInfo* w = NULL;
    while(temp!=NULL){ 
        if(!strcmp(((wordInfo*)temp->data)->word,_word_)) { // _word_ is already in list
            (((wordInfo*)temp->data)->count)++;
            free(_word_); //? Might not be needed - depends on reading
            found = true;
            break;
        }
        temp = temp->next;
    }
    if(!found){
        // Initialise wordInfo struct and add to wordInfoList
        w = calloc(1,sizeof(wordInfo));
        wordInfoInitialization(w,_word_);
        *word_info_list = appendList(*word_info_list,w);      
    } 
}

void addToVoc(node** voc,char* _word_, node* word_info_list){ 
    // If _word_ has been added in word_info_list then it 
    // already represents the .json in vocabulary count
    bool found = false; wordInfo* voc_w = NULL;
    node* temp_voc = *voc; node* temp = word_info_list;
    while(temp!=NULL){
        if(!strcmp(((wordInfo*)temp->data)->word,_word_)){
            found = true;
            break;
        }
        temp = temp->next;
    }
    if(found) return;   
    /* Make sure there is no duplicate of the word we try to add*/
    while(temp_voc!=NULL){
        if(!strcmp(((wordInfo*)temp_voc->data)->word,_word_)) { // word is already in list
            (((wordInfo*)temp_voc->data)->count) ++;
            found = true;
            break;
        }
        temp_voc = temp_voc->next;
    }
    if(!found){
        voc_w = calloc(1,sizeof(wordInfo));
        char* new_word = calloc(strlen(_word_)+1,sizeof(char*));
        strcpy(new_word,_word_);
        wordInfoInitialization(voc_w,new_word);
        *voc = appendList(*voc,voc_w);      
    } 
}
