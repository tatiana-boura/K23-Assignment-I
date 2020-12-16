#include "./data_for_training.h"

node* visitedList=NULL;

void createTFIDFarray(float*** x_array,unsigned int** y_array,hashTable* ht,unsigned int bucketSize, unsigned int vocabSize,unsigned int* n){
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
                for(unsigned int j = 0;j<numOfEntries && entryTable[j]!=NULL;j++){
                    if( !addrFoundinList(visitedList,entryTable[j]->clique)){
                        visitedList=appendList(visitedList,entryTable[j]->clique);
                        storeAbsDifference(entryTable[j], x_array, y_array, vocabSize, n);
                    }else entryTable[j]->clique=NULL;
                }
                temp=temp->next;
            }
        } 
	}
    // we don't need anymore to keep list of visited cliques
    destroyListOfStrings(visitedList,false); visitedList=NULL;
}


void storeAbsDifference(bucketEntry* entryTable_j,float*** x_array,unsigned int** y_array,unsigned int vocabSize, unsigned int* n){
    /* This function works as follows: let's say we have the example,
    clique is a b c d and its notClique is f e. The function stores the differences in x_array 
    and their labels in y_array:
    first it check if the notClique has been previously visited as a clique (if so the notClique
    elements will not be paired), then the function
    takes a and pairs it with the rest of the clique-- |a-b|, |a-c|, |a-d| (y_array: 1) and the 
    notClique -- |a-f|, |a-e| (y_array: 0)
    then takes b and pairs it with what follows of the clique-- |b-c|, |b-d| (y_array: 1) and all 
    of the notClique-- |b-f|, |b-e| (y_array: 0)
    then takes c-- |c-d| and |c-f|, |c-e| and at last reaches d that has no followed element in clique
    and just pairs with |d-f|, |d-e|
    */
   printf("n is %d\n",*n);
    node* outterTemp=entryTable_j->clique;
    node* innerTemp; node* otherTemp;
    bool revisitedNotClique = false;
    if(addrFoundinList(visitedList,entryTable_j->notClique))
        {printf("visited before\n"); revisitedNotClique = true;}

    // takes an item
    int times=0;
    while( outterTemp!=NULL ){
        printf("Outter %d\n",times);
        // and begins printing it with the item that follow
        innerTemp=outterTemp->next;
        otherTemp=entryTable_j->notClique;

        while( innerTemp!= NULL ){ 
            // clique is a list of pointers to bucketEntries
            bucketEntry* inner = innerTemp->data;
            bucketEntry* outter = outterTemp->data;
            // Difference between current (outter) and the rest of the clique
            //allocate memory in arrays for difference
            (*x_array)[(*n)] = calloc(vocabSize,sizeof(float));//n:= number of absolute differences
            for( unsigned int k = 0; k<vocabSize; k++ ){
                //calculate difference |outter->tfidf-inner->tfidf| 
                float abs_dif = fabs(outter->tfidf[k]-inner->tfidf[k]);
                //add to x_array
                (*x_array)[(*n)][k] = abs_dif;
                //store 1 to y_array
                (*y_array)[(*n)]=1;
                printf("|%s[%d]-%s[%d]| = |%f-%f| = %f  %d\n",outter->path,k,inner->path,k,outter->tfidf[k],inner->tfidf[k],(*x_array)[(*n)][k],(*y_array)[(*n)]);
            }
            (*n)++;
            //reallocate memory in arrays for next difference
            (*x_array) = realloc((*x_array),((*n)+1)*sizeof(float*));
            (*y_array) = realloc((*y_array),((*n)+1)*sizeof(unsigned int)); 
            
           
            // Difference between current (outter) and all of the notClique
           /* if(!revisitedNotClique){
                printf("not visited\n");
                while(otherTemp!=NULL){
                    bucketEntry* other = otherTemp->data; 
                    //allocate memory in arrays for difference
                    (*x_array)[(*n)] = calloc(vocabSize,sizeof(float)); //n:= number of absolute differences
                    for( unsigned int k = 0; k<vocabSize; k++ ){
                        //calculate difference |outter->tfidf-other->tfidf| 
                        printf("Outter tfidf: %f \t\t other tfidf: %f\n",outter->tfidf[k],other->tfidf[k]);
                        float abs_dif = fabs(outter->tfidf[k]-other->tfidf[k]);
                        //add to x_array
                        (*x_array)[(*n)][k] = abs_dif;
                        //store 0 to y_array
                        (*y_array)[(*n)]=0;
                        printf("|%f-%f| = %f  %d\n",outter->tfidf[k],other->tfidf[k],(*x_array)[(*n)][k],(*y_array)[(*n)]);
                    }
                    //reallocate memory in arrays for next difference
                    (*n)++;
                    (*x_array) = realloc((*x_array),((*n)+1)*sizeof(float*));
                    (*y_array) = realloc((*y_array),((*n)+1)*sizeof(unsigned int));
                    otherTemp = otherTemp->next;
                }
            }*/
            innerTemp=innerTemp->next;
        }
        outterTemp=outterTemp->next;
        times++;
    }
}

void createSets( float** total_set, unsigned int* total_y, unsigned int total_size ,float*** train_set, unsigned int* n, float*** valid_set, unsigned int* m, unsigned int** y_train, unsigned int** y_valid ){
    /* total_set  : whole set that will be split
       total_y    : labels for whole set
       total_size : size of whole set
       train_set  : training set to be returned
       n          : size of training set
       valid_set   : test set to be retuned
       m          : size of test set            
       y_train    : labels for training set to be returned
       y_valid     : labels for test set to be returned */  
        
    // the training size will be the 80% of the initial 
    *n = 0.8 * total_size;
    // the test size is the rest    
    *m = total_size - *n;
    // initialize memo for train and test
    (*train_set) = calloc(*n, sizeof(float*));
    (*y_train) = calloc(*n, sizeof(float*));
    (*valid_set) = calloc(*m, sizeof(float*));
    (*y_valid) = calloc(*m, sizeof(float*));

    srand(time(NULL));

    /* we will use a boolean array of size as total lines 
       -- true: will be in train_set 
       -- false: will be in test set */
    bool visitedNumbers[total_size];
    for( unsigned int i=0; i <total_size; i++)
        visitedNumbers[i] = true;

    unsigned int _n_=0, _m_=0;
    // until the number needed is succeded
    while( _m_!= *m ){
        // choose randomly and number
        unsigned int randomNumber = rand()%total_size;
        // if this number has not been discovered yet
        if(visitedNumbers[randomNumber]){
            visitedNumbers[randomNumber] = false;
            _m_++;
        }
    }
    /* not new memory for line -- use pointer from initial array*/
    _n_=0, _m_=0;
    for( unsigned int j = 0; j <total_size; j++ ){
        if( visitedNumbers[j] ){ 
            (*train_set)[_n_] = total_set[j];
            (*y_train)[_n_++] = total_y[j];
        }else{
            (*valid_set)[_m_] = total_set[j];
            (*y_valid)[_m_++] = total_y[j];
        }
    }

    return;
}


