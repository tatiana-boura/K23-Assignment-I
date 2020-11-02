#ifndef __TUPLE_OF_CHARS__
#define __TUPLE_OF_CHARS__

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct Tuple{
	char* propertyName;
	char* propertyValue;
}Tuple;

typedef Tuple* TuplePtr;

void tupleInitialization(TuplePtr,char*,char*);
void tupleDeletion(TuplePtr);
void printTuple(TuplePtr);


#endif