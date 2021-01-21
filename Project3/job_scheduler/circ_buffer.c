#include "circ_buffer.h"


//__BUFFER_FUNCTIONS______________________________________________________

// buffer initialization
circBufferPtr circBufferInit( unsigned int _size_){

	circBufferPtr cbuf = calloc(1,sizeof(circBuffer)); assert( cbuf != NULL );
	cbuf->buffer = calloc(_size_,sizeof(Batch)); assert( cbuf->buffer!= NULL );
	//cbuf->buffer = buffer;
	cbuf->size = _size_;
	cbuf->head = 0;
    cbuf->tail = 0;
    cbuf->full = false;

	return cbuf;
}

// check if buffer is empty
bool circBufferEmpty(circBufferPtr cbuf){
    // a buffer is empty if it is not full and beginning equals to end
    return ((cbuf->full==false) && (cbuf->head == cbuf->tail));
}

// get buffers size
unsigned int circBufferCurrSize(circBufferPtr cbuf){

	// if buffer is not full then it's size is:
	//		(i) if beginning is after ending then size = beggining-end
	//		(ii) else 							  size = size+beginning-end 
	if( cbuf->full == false )	
		return ( cbuf->head >= cbuf->tail ) ? (cbuf->head - cbuf->tail) : (cbuf->size + cbuf->head - cbuf->tail);

	// else it is the size
	return cbuf->size;
}

// insert element at circular buffer
bool circBufferInsert(circBufferPtr cbuf, Batch _data_){
    
    // if buffer isn't full
    if(cbuf->full==false){
    	// put data where head is
        cbuf->buffer[cbuf->head] = _data_;
        //_______________________________________________________________________
		// if the buffer is full, update tail
		if(cbuf->full) cbuf->tail = (cbuf->tail + 1) % cbuf->size;
		cbuf->head = (cbuf->head + 1) % cbuf->size; // new head
		cbuf->full = (cbuf->head == cbuf->tail); // buffer->full if begin == end
        //_______________________________________________________________________
        return true;
    }
    // if it is full-> operation unsuccessful
    return false;
}

// get element from circular buffer
bool circBufferPop(circBufferPtr cbuf, Batch* _data_){

	// if there is something to pop
    if(circBufferEmpty(cbuf)==false){
    	// get data from the tail
        *_data_ = cbuf->buffer[cbuf->tail];
        //_______________________________________________________________________
		cbuf->full = false; // got one out -> if the buffer was full, it now isn't 
		// new tail
		cbuf->tail = (cbuf->tail + 1) % cbuf->size; // new tail
        //_______________________________________________________________________
        return true;
    }
    // if it is empty-> operation unsuccessful
    return false;
}

// free all the memo that the buffer has used
void circBufferDealloc(circBufferPtr cbuf){
	free(cbuf->buffer); cbuf->buffer=NULL;
	free(cbuf); cbuf=NULL;
}


