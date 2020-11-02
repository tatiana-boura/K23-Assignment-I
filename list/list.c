#include "list.h"

node* appendList(node* n, void* data){
    if(n==NULL){    // after the last node of the list
        n = malloc(sizeof(node)+1);
        n->data=data;
        n->next=NULL;
    }
    else n->next = appendList(n->next,data);
    return n;
}

void destroyList(node* n,unsigned int* size,bool del, void(*deleteData)(void*,void*)){
    node* oldhead = NULL;
    node* newhead = n;
    if(newhead==NULL) { return; }   //empty list
    //else remove current head and move on to the next one
    oldhead = newhead; 
    newhead = oldhead->next;
    //if instructed to do so by call (del==true) free the node's data
    if(del) (*deleteData)(oldhead->data,size);
    //free the actual node
    free(oldhead);
    oldhead = NULL;
    destroyList(newhead,size,del,deleteData);
}