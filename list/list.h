#ifndef _LIST_H_
#define _LIST_H_
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
    void* data;
    struct node* next;
}node;

#endif