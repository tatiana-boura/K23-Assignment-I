#include "list.h"

node* appendList(node* n, void* data){

    if(n==NULL){    // after the last node of the list
        
        n = calloc(1,sizeof(node)+1);
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

/*
void destroyClique(node** n, char* _path_ ){ 
    
    node* tempNode = *n;
    node* prev; 
  
    if (tempNode != NULL && strcmp(_path_,(char*)tempNode->data) == 0){ 
        *n = tempNode->next;               
        free(tempNode);  tempNode=NULL; 
        return; 
    } 
   
    while (tempNode != NULL && strcmp(_path_,(char*)tempNode->data) != 0){ 
        prev = tempNode; 
        tempNode = tempNode->next; 
    } 
   
    prev->next = tempNode->next; 
    printf("%s\n", (char*)tempNode->data);

    free(tempNode); tempNode=NULL;


    return;
} */


// function to help identify the clique
bool addrFoundinList(node* n,  void* addr ){

    if (n == NULL) return false; 

    if (n->data == addr) return true; 
  
    return addrFoundinList(n->next, addr); 
} 


void printList(node* n, void(*printData)(void*)){

    node* tempNode=n;

    while(tempNode!=NULL){
        
        // printData == void for printing the clique
        if(printData==NULL) printf("%s\n", (char*)tempNode->data);
        else{
            (*printData)(tempNode->data);
        } 
        
        tempNode=tempNode->next;
    }

    return;
}