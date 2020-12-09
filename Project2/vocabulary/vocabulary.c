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
        printf("made %s, %d \n",w->word,w->count);
        *word_info_list = appendList(*word_info_list,w);      
    } 
}

void addToVoc(node** voc,char* _word_){
    /* make sure there is no duplicate of the word we try to add*/
    /*Maybe make a foundinlist function?*/
    bool found = false; node* temp = *voc; wordInfo* voc_w = NULL;
    while(temp!=NULL){
        if(!strcmp(((wordInfo*)temp->data)->word,_word_)) { // word is already in list
            (((wordInfo*)temp->data)->count) ++;
            found = true;
            break;
        }
        temp = temp->next;
    }
    if(!found){
        voc_w = calloc(1,sizeof(wordInfo));
        char* new_word = calloc(strlen(_word_)+1,sizeof(char*));
        strcpy(new_word,_word_);
        wordInfoInitialization(voc_w,new_word);
        *voc = appendList(*voc,voc_w);      
    } 
}

/* Made this one for both of the above -> frees and init got messed up
void addWordInfoToList(node** list,wordInfo* _word_info_){
    bool found = false; node* temp = *list;
    while(temp!=NULL){ //printf("\nIs %s the same as %s \n",((wordInfo*)temp->data)->word,_word_info_->word);
        if(!strcmp(((wordInfo*)temp->data)->word,_word_info_->word)) { // word is already in list
            (((wordInfo*)temp->data)->count)++;
            wordInfoDeletion(_word_info_);
            found = true;
            break;
        }
        temp = temp->next;
    }
    if(!found){
        *list = appendList(*list,_word_info_);      
    } 
}*/