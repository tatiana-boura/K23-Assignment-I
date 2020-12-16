#include "../hash_table/hash_table.h"
#include <math.h>
#include <stdlib.h>

void createTFIDFarray(float*** x_array,unsigned int** y_array,hashTable* ht,unsigned int bucketSize, unsigned int vocabSize,unsigned int* n);
void storeAbsDifference(bucketEntry* entryTable_j,float*** x_array,unsigned int** y_array,unsigned int vocabSize,unsigned int* n);