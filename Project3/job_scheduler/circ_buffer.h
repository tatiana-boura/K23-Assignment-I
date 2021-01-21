#ifndef __CIRC_BUFF__
#define __CIRC_BUFF__

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "batch.h"

//__CIRC_BUFFER__________________________________________________
typedef struct circBuffer{
	
	Batch* buffer; // buffer
	unsigned int head; 		// first element
	unsigned int tail; 		// last element
	unsigned int size; 		// maximum of buffer(capacity)
	bool full;				// check if buffer is full

}circBuffer;
typedef circBuffer* circBufferPtr;

circBufferPtr circBufferInit( unsigned int size);
bool circBufferEmpty(circBufferPtr cbuf);
unsigned int circBufferCurrSize(circBufferPtr cbuf);
bool circBufferInsert(circBufferPtr cbuf, Batch data);
bool circBufferPop(circBufferPtr cbuf, Batch* data);
void circBufferDealloc(circBufferPtr cbuf);

#endif