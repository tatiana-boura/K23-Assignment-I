#ifndef _DATA_FOR_TRAIN_
#define _DATA_FOR_TRAIN_

#include "../hash_table/hash_table.h"
#include "./hash_table_pairs.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void create_x_y_array(float*** x_array,unsigned int** y_array,hashTable* ht,unsigned int bucketSize, unsigned int vocabSize,unsigned int* n);
void storeAbsDifference(bucketEntry* entryTable_j,float*** x_array,unsigned int** y_array,unsigned int vocabSize,unsigned int* n);
void createSets( float** total_set, unsigned int* total_y, unsigned int total_size ,float*** train_set, unsigned int* n, float*** valid_set, unsigned int* m, unsigned int** y_train, unsigned int** y_valid );
void makeResultFile(float* w_array,node* vocabulary);
#endif