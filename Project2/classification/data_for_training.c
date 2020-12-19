#include "./data_for_training.h"

#define BUCKETSIZEPAIR 100
#define HTSIZEPAIR 9000

hashTablePair* htPair=NULL;
node* visitedList=NULL;

void create_x_y_array(float*** x_array,unsigned int** y_array,hashTable* ht,unsigned int bucketSize, unsigned int vocabSize,unsigned int* n){
	
    *n=0;	//n:= number of absolute differences
	// create hash table to store visited pairs
    htPair = createHTPair(HTSIZEPAIR);

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
                    // if this clique has not been visited before
                    if( !addrFoundinList(visitedList,entryTable[j]->clique )){
                        // mark it as visited
                        visitedList = appendList(visitedList,entryTable[j]->clique);
                        // start making the tfidf or bow array
                        storeAbsDifference(entryTable[j], x_array, y_array, vocabSize, n);
                    }
                }
                temp=temp->next;
            }
        } 
	}
    
    // we don't need anymore to keep list of visited cliques
    destroyHTPair(htPair,BUCKETSIZEPAIR);
    destroyListOfStrings(visitedList,false); visitedList=NULL;
    return;
}


void storeAbsDifference(bucketEntry* entryTable_j,float*** x_array,unsigned int** y_array,unsigned int vocabSize, unsigned int* n){
    /* This function works as follows: let's say we have the example,
    clique is a b c d and its notClique is f e. The function stores the differences in x_array 
    and their labels in y_array:
    first it check if the clique has been previously visited, then the function takes a and pairs 
    it with the rest of the clique-- |a-b|, |a-c|, |a-d| (y_array: 1) and the notClique 
    -- |a-f|, |a-e| (y_array: 0)
    then takes b and pairs it with what follows of the clique-- |b-c|, |b-d| (y_array: 1) and all 
    of the notClique-- |b-f|, |b-e| (y_array: 0)
    then takes c-- |c-d| and |c-f|, |c-e| and at last reaches d that has no followed element in clique
    and just pairs with |d-f|, |d-e|
    ------------------------------------------------------------------------------------------------
    This function uses a hash to look up already visited pairs in order not to revistt them. Note that 
    already visited cliques are not visited at all due to the look up in the list of already visited
    cliques in the function above 
    -----------------------------------------------------------------------------------------------*/

    //n:= number of absolute differences

    node* outter_clique_list=entryTable_j->clique;
    node* inner_clique_list; node* outter_notClique_list;

    // takes an item of the clique
    while( outter_clique_list!=NULL ){
        /* and begins pairing it with the items in clique that follow 
        (difference between current (outter) and the rest of the clique) */
        inner_clique_list=outter_clique_list->next;
        bucketEntry* outter_clique = outter_clique_list->data;
        while( inner_clique_list!= NULL ){ 
            // clique is a list of pointers to bucketEntries
            bucketEntry* inner_clique = inner_clique_list->data;
            // if this pair has not been visited before
            if( !foundInHTPair( htPair, outter_clique->path, inner_clique->path, BUCKETSIZEPAIR ) ){
                // mark it as visited
                addtoHTPair(htPair, outter_clique->path, inner_clique->path, BUCKETSIZEPAIR );
                //allocate memory in arrays for difference
                (*x_array)[(*n)] = calloc(vocabSize,sizeof(float));
                //printf("1 cliques : [%s] - [%s]\n",outter_clique->path,inner_clique->path );
                for( unsigned int k = 0; k<vocabSize; k++ ){
                    //calculate difference |outter->vector-inner_clique->vector| 
                    float abs_dif = fabs(outter_clique->vector[k]-inner_clique->vector[k]);
                    (*x_array)[(*n)][k] = abs_dif; //add to x_array 
                    (*y_array)[(*n)]=1; //store 1 to y_array -- same clique
                    //printf("|%s[%d]-%s[%d]| = |%f-%f| = %f  %d\n",outter_clique->path,k,inner_clique->path,k,outter_clique->vector[k],inner_clique->vector[k],(*x_array)[(*n)][k],(*y_array)[(*n)]);
                }
                (*n)++;
                //reallocate memory in arrays for next difference
                (*x_array) = realloc((*x_array),((*n)+1)*sizeof(float*));
                (*y_array) = realloc((*y_array),((*n)+1)*sizeof(unsigned int)); 
            }
            inner_clique_list=inner_clique_list->next;
        }
        outter_clique_list=outter_clique_list->next;
    }  

    // for each item of the clique
    outter_clique_list=entryTable_j->clique;
    while( outter_clique_list!=NULL ){
        /* and begins pairing it with the items in the anti-clique 
        (difference between current (outter) and inner_notClique) */
        outter_notClique_list=entryTable_j->notClique;
        bucketEntry* outter_clique = outter_clique_list->data;
        // start getting not cliques
        while(outter_notClique_list!=NULL){
            //access the clique that is pointed by current notClique
            node* temp = outter_notClique_list->data;
            bucketEntry* entryOut = temp->data;
            node* inner_notClique_list = entryOut->clique;
            // for each item in the anti-clique
            while( inner_notClique_list != NULL ){
                // get item (clique)
                bucketEntry* inner_notClique = (bucketEntry*)inner_notClique_list->data;
                // if this pair has not been visited before
                if( !foundInHTPair( htPair, outter_clique->path, inner_notClique->path, BUCKETSIZEPAIR ) ){
                    // mark it as visited
                    addtoHTPair(htPair, outter_clique->path, inner_notClique->path, BUCKETSIZEPAIR );
                    //allocate memory in arrays for difference
                    (*x_array)[(*n)] = calloc(vocabSize,sizeof(float));
                    //printf("2 not cliques : [%s] - [%s]\n",outter_clique->path, inner_notClique->path );
                    for( unsigned int k = 0; k<vocabSize; k++ ){
                        //calculate difference |outter_clique->vector-inner_notClique->vector| 
                        float abs_dif = fabs(outter_clique->vector[k]-inner_notClique->vector[k]);
                        (*x_array)[(*n)][k] = abs_dif; //add to x_array
                        (*y_array)[(*n)]=0; //store 0 to y_array -- anti-clique
                        //printf("|%s[%d]-%s[%d]| = |%f-%f| = %f  %d\n",inner_clique->path,k,inner_notClique->path,k,inner_clique->vector[k],inner_notClique->vector[k],(*x_array)[(*n)][k],(*y_array)[(*n)]);
                    }
                    (*n)++;
                    //reallocate memory in arrays for next difference
                    (*x_array) = realloc((*x_array),((*n)+1)*sizeof(float*));
                    (*y_array) = realloc((*y_array),((*n)+1)*sizeof(unsigned int)); 
                }
                inner_notClique_list = inner_notClique_list->next;
            }     
            outter_notClique_list = outter_notClique_list->next;
        }        
        outter_clique_list=outter_clique_list->next; 
    } 

    return;
}

void createSets( float** total_set, unsigned int* total_y, unsigned int total_size ,float*** train_set, unsigned int* n, float*** valid_set, unsigned int* m, unsigned int** y_train, unsigned int** y_valid ){
    /* total_set  : whole set that will be split
       total_y    : labels for whole set
       total_size : size of whole set
       train_set  : training set to be returned
       n          : size of training set
       valid_set  : test set to be retuned
       m          : size of test set            
       y_train    : labels for training set to be returned
       y_valid    : labels for test set to be returned */  
        
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


