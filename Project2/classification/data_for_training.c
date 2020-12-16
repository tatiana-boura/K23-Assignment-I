#include "./data_for_training.h"

void createTFIDFarray(float*** array,hashTable* ht,unsigned int bucketSize, unsigned int vocabSize,unsigned int* n){
	*n=0;	//n:= number of absolute differences
	
	// go through every entry in the hash and make the bow vector
    unsigned int numOfEntries =(bucketSize-sizeof(bucket*))/sizeof(bucketEntry*);
    // for all hash table
    for( unsigned int i=0; i<ht->size; i++ ){
		// go to the bucket of ht
        if(ht->table[i] != NULL){
            node* temp = ht->table[i];

            while(temp!=NULL){
                bucketEntry** entryTable = temp->data;
                // and every entry of the bucket
                for(unsigned int j = 0;j<numOfEntries;j++){
                    if(entryTable[j]!=NULL){
						(*array)[(*n)] = calloc(vocabSize,sizeof(float));
						// FOR NOW pass every element of tfidf vector in hash to array
                        for( unsigned int k = 0; k<vocabSize; k++ ){
							(*array)[(*n)][k] = entryTable[j]->tfidf[k];
							printf("%f\n",(*array)[(*n)][k]);
                        }
						if((j+1)!=numOfEntries){
							(*n)++;
							(*array) = realloc((*array),((*n)+1)*sizeof(float*));
						}
                    }
                }
                temp=temp->next;
            }

        } 
	}

}
