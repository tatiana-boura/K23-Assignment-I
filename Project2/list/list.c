#include "list.h"

node* appendList(node* n, void* data){

    // put at front for O(1) 
    node* newNode=calloc(1,sizeof(node)); assert(newNode!=NULL);
    newNode->data=data;
    newNode->next=n;

    return newNode;
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

void destroyListOfTuples(node* n, void(*deleteData)(void*)){
    
    node* oldhead = NULL;
    node* newhead = n;
    
    if(newhead==NULL) return;   //empty list
    //else remove current head and move on to the next one
    oldhead = newhead; 
    newhead = oldhead->next;
    
    (*deleteData)(oldhead->data);
    //free the actual node
    free(oldhead); oldhead = NULL;
    
    destroyListOfTuples(newhead,deleteData);

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