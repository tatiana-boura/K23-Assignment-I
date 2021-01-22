#include "list.h"

node* appendList(node* n, void* data){
    // put at front for O(1) 
    node* newNode=calloc(1,sizeof(node)); assert(newNode!=NULL);
    newNode->data=data;
    newNode->next=n;

    return newNode;
}

node* appendListEnd(node* n, void* data){
    if(n==NULL){ // after the last node of the list
        n = malloc(sizeof(node)+1);
        n->data=data;
        n->next=NULL;
    }else n->next = appendList(n->next,data);

    return n;
}

node* mergeTwoLists(node* n1, node* n2){

    if (n1==NULL) return n2; 
    if (n2==NULL) return n1; 
   
    n1->next = mergeTwoLists(n1->next, n2); 
    return n1; 
    
}

node* removeNode(node** n, void* addr){ 
      
    node* temp = *n; 
    node* prev = NULL; 
      
    // is head  
    if (temp != NULL && temp->data == addr) { 
        *n = temp->next;
        return temp; 
    } 
  
    while (temp != NULL && temp->data != addr){ 
        prev = temp; 
        temp = temp->next; 
    } 
  
    // not found
    if (temp == NULL) return NULL; 
  
    // find and remove
    prev->next = temp->next; 
    return temp; 
} 

node* pop(node** n){ 
      
    node* temp = *n; 
      
    // pop first element  
    if (temp != NULL) { 
        *n = temp->next;
        return temp; 
    } 
    return NULL; 
}
 

void destroyList(node* n, unsigned int* size, bool del, void(*deleteData)(void*,void*)){
    
    node* oldhead = NULL;
    node* newhead = n;
    
    if(newhead==NULL) return;   //empty list
    //else remove current head and move on to the next one
    oldhead = newhead; 
    newhead = oldhead->next;
    
    //if instructed to do so by call (del==true) free the node's data
    if(del) (*deleteData)(oldhead->data,size);
    //free the actual node
    free(oldhead); oldhead = NULL;
    
    destroyList(newhead,size,del,deleteData);

    return;
}


void destroyListOfWordInfo(node* n, void(*deleteData)(void*)){
    
    node* oldhead = NULL;
    node* newhead = n;
    
    if(newhead==NULL) return;   //empty list
    //else remove current head and move on to the next one
    oldhead = newhead; 
    newhead = oldhead->next;
    
    (*deleteData)(oldhead->data);
    //free the actual node
    free(oldhead); oldhead = NULL;

    destroyListOfWordInfo(newhead,deleteData);

    return;
}

void destroyListOfStrings(node* n, bool destroyDataAsWell ){

    if (n == NULL) return;

    // if destroyDataAsWell==false it means that nodes data stored in 
    // the same address as path so we don't have to destroy it here
    // else, destroy data as well
    if(destroyDataAsWell){free(n->data); n->data=NULL; }

    destroyListOfStrings(n->next,destroyDataAsWell);    
    free(n); n=NULL; 

    return;
}

// function to help identify the clique
bool addrFoundinList(node* n,  void* addr){

    if (n == NULL) return false; 
    // this list is a list of visited address spaces
    if (n->data == addr) return true; 
  
    return addrFoundinList(n->next, addr); 
} 


void printList(node* n, void(*printData)(void*)){

    node* tempNode=n;

    while(tempNode!=NULL){
        
        // printData == void for printing the clique
        if(printData==NULL) printf("%s\n", (char*)tempNode->data);
        else
            (*printData)(tempNode->data);
        
        tempNode=tempNode->next;
    }

    return;
}

void makeOutputFileList(node* n, FILE* outputFile){

    /* This function works as follows: let's say we have the example,
    clique is a b c d. The function does the printing:
    takes a and pairs it with what follows-- ab, ac, ad
    then takes b and pairs it with what follows-- bc, bd (it has already been paired
    with it's previous ones), then takes c-- cd and at last reaches d that has no 
    followed, so it returns */

    node* outterTemp=n;
    node* innerTemp;

    // takes an item
    while( outterTemp!=NULL ){
        // and begins printing it with the item that follow
        innerTemp=outterTemp->next;

        while( innerTemp!= NULL ){ 
            // clique is a list of pointers to bucketEntries
            bucketEntry* inner = innerTemp->data;
            bucketEntry* outter = outterTemp->data;
            fprintf(outputFile,"%s \t %s\n",(char*)outter->path, (char*)inner->path );
            innerTemp=innerTemp->next;
        }
        outterTemp=outterTemp->next;
    }

    return;
}

//if word w is part of list l return false, else true
bool notInlist(node* n, char* w){
    node* tempNode=n;

    while(tempNode!=NULL){
        if(strcmp((char*)tempNode->data,w) == 0){
            return false; //in list
        }
        tempNode=tempNode->next;
    }
    return true; //NOT in list
}

void sortedInsertStr(node** n, wordInfo* w){

    // create node
    node* newNode = calloc(1,sizeof(node));
    newNode->data = w; 
    newNode->next = NULL;

    // head 
    node* _n_ =  *n;
    if (*n == NULL || strcmp(((wordInfo*)(_n_->data))->word,((wordInfo*)newNode->data)->word) > 0) { 
        newNode->next = *n; 
        *n = newNode;
    }else{ 
        // find the node and insert
        node* tempNode = *n; 
        while (tempNode->next != NULL && strcmp(((wordInfo*)tempNode->next->data)->word,((wordInfo*)newNode->data)->word) < 0) { 
            tempNode = tempNode->next; 
        } 
        newNode->next = tempNode->next; 
        tempNode->next = newNode; 
    }
    return; 
} 

bool foundInSortedListStr(node* n, char* w, bool increaseCounter){

    node* tempNode=n;

    while( tempNode != NULL && strcmp(((wordInfo*)tempNode->data)->word,w) < 0 )
        tempNode = tempNode->next;

    if( tempNode == NULL ) return false;

    if( strcmp(((wordInfo*)tempNode->data)->word,w) == 0){
        if( increaseCounter ) (((wordInfo*)tempNode->data)->count)++;
        return true;
    } 
    return false;
}

void deleteWords( node** n, bool should_be_dropped[], unsigned int vocabSize ){

    node* tempNode = *n;
    node* prev = NULL;
    // for all vocabulary
    for( unsigned int i=0; i<vocabSize; i++ ){
        /* if there is a deletion of a column 
        tempNode and prev will be updated internally */
        bool update = true;
        // if word should be removed from the vocabulary
        if( should_be_dropped[i] == true ){
            // if the first removed is the first in general
            if( i==0 ){
                // make the second node the head
                *n = tempNode->next; 
                wordInfoDeletion((wordInfo*)tempNode->data);
                free(tempNode); tempNode=NULL;
                tempNode = *n; prev = NULL;
                // do not update tempNode and prev
                update = false;
            }else{
                if(prev==NULL){ // if the previous deleted is the first one
                    *n = tempNode->next;
                    //free((wordInfo*)tempNode->data);
                    wordInfoDeletion((wordInfo*)tempNode->data);
                    free(tempNode); tempNode=NULL;
                    tempNode = *n; prev = NULL;
                }else{ // else do the normal deletion
                    prev->next = tempNode->next; 
                    wordInfoDeletion((wordInfo*)tempNode->data);
                    free(tempNode); tempNode=NULL;
                    tempNode = prev->next; 
                }
                // do not update tempNode and prev
                update = false;
            }
        }
        // if the previous column has not been deleted -- update
        if( update ){
            prev = tempNode;
            tempNode = tempNode->next;
        }
    }

    return;
}
