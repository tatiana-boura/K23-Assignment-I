#ifndef __TUPLE_OF_CHARS__
#define __TUPLE_OF_CHARS__

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../list/list.h"

typedef struct Tuple{
	char* propertyName;
	node* propertyValueList;
}Tuple;

typedef Tuple* TuplePtr;

void tupleInitialization(TuplePtr,char*,char*,int);
void insertAtValueList( TuplePtr, char*);
void tupleDeletion(TuplePtr);
void printTuple(TuplePtr);


#endif