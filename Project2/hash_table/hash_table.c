#include "hash_table.h"

node* listOfCliques=NULL;
node* listOfNotCliques=NULL;

//__hash_function________________________________________________________________________________________

unsigned int hash(char* string, hashTable* ht){
    
    unsigned int value;
    value = 0;
    
    while(*string != '\0'){ //parse the whole string
        value = *string + 31 * value; //multiplying with a prime number to evenly spread data into buckets
        string++; //move the pointer over 1 char
    }
    return value % ht->size;
}

//__create_hash_table____________________________________________________________________________________

hashTable* createHT(unsigned int size){
    
    hashTable* ht = NULL; 
    ht = calloc(1,sizeof(hashTable)); assert(ht!=NULL);
    
    ht->size = size;

    ht->table = calloc(size,sizeof(bucket*)); assert(ht->table!=NULL); 
    //table of pointers to buckets
    for(unsigned int i=0;i<size;i++) ht->table[i]=NULL;
    
    return ht;
}
//_______________________________________________________________________________________________________


//__addToHashTable_______________________________________________________________________________________ 

void addtoHT(hashTable* ht, char* key, unsigned int bucketSize, node* _wordInfoList_){

    bucket* bucketPtr; unsigned int index; 
    
    unsigned int numOfEntries = (bucketSize-sizeof(bucket*))/sizeof(bucketEntry*);

    bucketPtr = getBucket(ht,key,&index);

    //if the hash table's index points to NULL -- no buckets, create bucket
    if(bucketPtr==NULL){

        //create entries table and add this entry
        bucketEntry* entry = createEntry(key,_wordInfoList_);
        bucketEntry** entryTable = calloc(numOfEntries,sizeof(bucketEntry*)); assert(entryTable!=NULL); 
        
        for(unsigned int i=0; i<numOfEntries; i++) entryTable[i]=NULL;
        
        entryTable[0]=entry;
        
        bucketPtr = appendList(bucketPtr,entryTable); //create bucket (list of buckets)
        
        ht->table[index] = bucketPtr; //add to hash table
    }
    else{
        //create bucketEntry
        bucketEntry* entry = createEntry(key,_wordInfoList_);
        bucketEntry** entryTable = bucketPtr->data;

        int position=-1;
        for(unsigned int i=0;i<numOfEntries;i++){
            if(entryTable[i]==NULL){
                position=i;    //j holds the position in bucket's entry table in which
                break;         // the new entry should be added
            }
        }
        if(position==-1){  //entry doesn't fit in block -> new list node

            bucketEntry** newEntryTable = calloc(numOfEntries,sizeof(bucketEntry)); assert(newEntryTable!=NULL);
            
            for(int i=0;i<numOfEntries;i++) newEntryTable[i]=NULL;
            newEntryTable[0]=entry;
            
            bucketPtr = appendList(bucketPtr,newEntryTable);
        }
        else{

            entryTable[position]=entry;
            bucketPtr->data = entryTable;
        }

        ht->table[index] = bucketPtr; 
    }

    return;
}

//___make_tfidf_vectors________________________________________________________________________________________________________________________________________________________________________________
unsigned int make_tfidf_vectors( hashTable* ht, unsigned int bucketSize, unsigned int vocabSize, node* vocabulary, unsigned int numOfJSON, hashTableVOC* htVOC, unsigned int bucketSizeVOC ){

     // go through every entry in the hash and make the bow vector
    unsigned int numOfEntries =(bucketSize-sizeof(bucket*))/sizeof(bucketEntry*);

 //__COMPUTE_TFIDF_FOR_THE_FIRST_TIME_BEFORE_DROPPING______________________________   
    
    float* tfidf_average = calloc(vocabSize,sizeof(float));
   
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

                        // initialize the needed size for the tf
                        entryTable[j]->vector = calloc(vocabSize,sizeof(float)); 
                        assert(entryTable[j]->vector!=NULL);
                       
                        /* count := how many times the current word is spotted in current JSON.
                        position := what is the position of the word in the vocabulary*/

                        // counts total number of words in current JSON
                        unsigned int totalWordsJSON = 0;

                        // get vocabulary of the current JSON

                        node* wordJSON = entryTable[j]->wordInfoList;
                        while( wordJSON != NULL ){
                            // update total words of this JSON
                            wordInfo* infoJSON = (wordInfo*)(wordJSON->data);
                            totalWordsJSON+=infoJSON->count;
                            // go to next word of this JSON
                            wordJSON = wordJSON->next;
                        }

                        wordJSON = entryTable[j]->wordInfoList;
                        while( wordJSON != NULL ){

                            wordInfo* infoJSON = (wordInfo*)(wordJSON->data);
                            char* w = infoJSON->word;

                            /* get words relative position in the vocabulary, so that words 
                            of all vectors are in the same order */ 
                            unsigned int info_count;
                            int position = getPositionInAndCountVOC( htVOC, w, bucketSizeVOC, &info_count );
                            position = vocabSize - position -1;

                            /* compute the tf vector --:
                            -- num_of_times_word_is_found_in_curr_JSON / total_num_of_words_in_curr_JSON */
                            entryTable[j]->vector[position] = (float)infoJSON->count/totalWordsJSON;
                            /* compute the tfidf vector --:
                            -- use the type tf*log(n/nt) */
                            entryTable[j]->vector[position] *= (float)log10((double)(numOfJSON/info_count));

                            tfidf_average[position] += entryTable[j]->vector[position];

                            //printf("word found -- %s\t", w);
                            //printf("tf value is %f\t\t", entryTable[j]->vector[position] );
                            //printf("tfidtf value is %f\n", entryTable[j]->vector[position]);    

                            // go to next word of this JSON
                            wordJSON = wordJSON->next;
                        }  
                        //for(int d=0; d<vocabSize; d++) printf("%f\t",(entryTable[j]->vector)[d] ); printf("\n");
                    }
                }
                temp=temp->next;
            }
        }    
    }

//__GET_AVERAGE_AND_DROP_TFIDF_WITH_COLUMNS_________________________________________________________________

    float floor_tf_idf = 0.00155*(float)numOfJSON;

    // store the columns that should be dropped
    bool should_be_dropped[vocabSize];
    unsigned int new_vocabSize = 0;

    for( unsigned int k = 0; k<vocabSize; k++ ){
        // if average is less than a certain limit, the column should be dropped
        if( tfidf_average[k] < floor_tf_idf ){
            should_be_dropped[k] = true;
        }else{
            new_vocabSize++;
            should_be_dropped[k] = false;
        } 
    }

    //printf("\n"); for(int d=0; d<vocabSize; d++) printf("%f\t",tfidf_average[d] ); printf("\n");
    
    printf("new_vocabSize [%d]\n", new_vocabSize);
    printf("vocabSize [%d]\n",vocabSize );

    // now drop tf-idf columns
    
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

                        // initialize the needed size for the new tf
                        float* new_tf_idf = calloc(new_vocabSize,sizeof(float));
                        unsigned int _k_ = 0;

                        for( unsigned int k = 0; k < vocabSize; k++ ){
                            if( should_be_dropped[k] == false ){
                                new_tf_idf[_k_++] = entryTable[j]->vector[k];
                            }
                        }

                       free(entryTable[j]->vector); entryTable[j]->vector=NULL;
                       entryTable[j]->vector = new_tf_idf;

                       //for(int d=0; d<new_vocabSize; d++) printf("%f\t",(entryTable[j]->vector)[d] ); printf("\n");
                    }
                }
                temp=temp->next;
            }
        }    
    }
    
    free(tfidf_average); tfidf_average=NULL;
    return new_vocabSize;
}    

//___make_BoW_vectors________________________________________________________________________________________________________________________________________________________________________________
void make_BoW_vectors( hashTable* ht, unsigned int bucketSize, unsigned int vocabSize, node* vocabulary, unsigned int numOfJSON, hashTableVOC* htVOC, unsigned int bucketSizeVOC ){

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

                        // initialize the needed size for the tf
                        entryTable[j]->vector = calloc(vocabSize,sizeof(float)); 
                        assert(entryTable[j]->vector!=NULL);
                       
                        /* count := how many times the current word is spotted in current JSON.
                        position := what is the position of the word in the vocabulary*/

                         node* wordJSON = entryTable[j]->wordInfoList;
                        while( wordJSON != NULL ){

                            wordInfo* infoJSON = (wordInfo*)(wordJSON->data);
                            char* w = infoJSON->word;

                            /* get words relative position in the vocabulary, so that words 
                            of all vectors are in the same order */ 
                            unsigned int info_count;
                            int position = getPositionInAndCountVOC( htVOC, w, bucketSizeVOC, &info_count );
                            position = vocabSize - position -1;

                            /* compute the bow vector --:
                            -- num_of_times_word_is_found_in_curr_JSON */
                            entryTable[j]->vector[position] = (float)infoJSON->count;

                            // go to next word of this JSON
                            wordJSON = wordJSON->next;
                        }  
                        //for(int d=0; d<vocabSize; d++) printf("%f\t",(entryTable[j]->vector)[d] ); printf("\n");
                    }
                }
                temp=temp->next;
            }
        }    
    }


    return;
}    


//__destroyHashTable______________________________________________________________________________________

void destroyHT(hashTable* ht,unsigned int bucketSize){

    for(unsigned int i=0; i<ht->size; i++)
        destroyList(ht->table[i],&bucketSize,true,(void*)deleteBucketTable);
    
    free(ht->table); ht->table=NULL;
    free(ht); ht=NULL;

    destroyListOfStrings(listOfNotCliques,false);

    return;
}

void deleteBucketTable(bucketEntry** table, unsigned int* bucketSize){

    unsigned int numOfEntries =(*bucketSize-sizeof(bucket*))/sizeof(bucketEntry*);

    for(unsigned int i=0;i<numOfEntries;i++){
        if(table[i]){
            //free bucketEntry's fields
            
            // if we haven't come across this clique yet, destroy it 
            if( table[i]->clique != NULL ){
                destroyListOfStrings(table[i]->clique,false);
            	//destroyListOfStrings(table[i]->notClique,false);
            	
            }

            if(table[i]->notClique != NULL){
                if( !addrFoundinList(listOfNotCliques,table[i]->notClique)){
            	   listOfNotCliques = appendList(listOfNotCliques,table[i]->notClique);
            	   destroyListOfStrings(table[i]->notClique,false);
            	   table[i]->notClique=NULL;
                }
            }        
            
            //destroyListOfStrings(table[i]->notClique,false);

            destroyListOfWordInfo(table[i]->wordInfoList,(void*)wordInfoDeletion); 
            free(table[i]->path); table[i]->path=NULL;
            free(table[i]->vector); table[i]->vector=NULL;
            free(table[i]); table[i]=NULL;
        }
    }
    free(table); table=NULL;

    return;
}
//_______________________________________________________________________________________________________

bucket* getBucket(hashTable* ht, char* key, unsigned int* index){

    *index = hash(key,ht);
    bucket* bucketPtr = ht->table[*index];
    return bucketPtr;
}



bucketEntry* createEntry(char* _path_, node* _wordInfoList_){
    // create entry of the bucket with name key(path)
    bucketEntry* entry = calloc(1,sizeof(bucketEntry)); assert(entry!=NULL);

    entry->path = _path_;
    // create list of wordInfo <char*,unsigned int>
    entry->wordInfoList = _wordInfoList_;
    // initialize vector
    entry->vector = NULL;
    // create clique -- list of paths
    entry->clique = NULL; entry->clique = appendList(entry->clique,entry);
    // create not clique -- list of pointers to not cliques
    entry->notClique = NULL; 

    return entry;
}


bool foundInHT(hashTable* ht, char* _path_, unsigned int bucketSize, unsigned int* entryNum, bucket** bucketFound ){
    // if function returns true(item is found), 
    // bucketFound := in which bucket entry has been found, entryNum := where in the array
    
    unsigned int index;

    // get first node of bucket list
    bucket* bp = getBucket(ht,_path_,&index);
    bucket* temp = bp;

    bucketEntry** entryTable;

    unsigned int numOfEntries =(bucketSize-sizeof(bucket*))/sizeof(bucketEntry*);
    
    // iterate while temp != NULL and item hasn't been found
    while(temp!=NULL){
        
        entryTable = temp->data;
        
        for(unsigned int i=0;i<numOfEntries;i++){

            if(entryTable[i]!=NULL){
                // if key path has been found, return what must be returned
                if(strcmp(entryTable[i]->path,_path_)==0){ 
                    *entryNum=i;
                    *bucketFound=temp;
                    return true;
                }    
            }
        }
        temp = temp->next;
    }
    return false;
}

//void updateWordInfoList(hashTable* ht, )

void changePointers(hashTable* ht, unsigned int bucketSize, bucket** bucketFound1, unsigned int entryNum1, bucket** bucketFound2, unsigned int entryNum2 ){

    // first go to the first one and get the clique(list)
    bucketEntry**  entryTable1 = (*bucketFound1)->data;
    node* clique1 = entryTable1[entryNum1]->clique;
    node* notClique1 = entryTable1[entryNum1]->notClique;

    // then go to second one and do the same
    bucketEntry** entryTable2 = (*bucketFound2)->data;
    node* clique2 = entryTable2[entryNum2]->clique;
    node* notClique2 = entryTable2[entryNum2]->notClique;

    // if cliques aren't already the same
    if (clique1!=clique2){
        
        // merge the two lists
        clique1=mergeTwoLists(clique1,clique2);

        node* t=notClique2;
        if( notClique1!=notClique2 ){
            
	        while( t!=NULL ){
	        	// make a compact notClique list
	        	if( !addrFoundinList(notClique1,t->data) ){
                    
	        		// if notCliques have not that element in common 
	        		notClique1 = appendList(notClique1,t->data);
	        	}else{
                    // the address is found a second time which means that 
                    // the two notCliques have the same element. Remove that
                    // element for each element of the notClique of the clique

                    node* _n_;
                    node* toDel=NULL;

	        		node* n_ = t->data; bucketEntry* e_ = n_->data;
                    // find clique that if found duplicate in the no list
                    unsigned int _entryNum_; bucket* _bucketFound_;
                    foundInHT(ht, e_->path, bucketSize, &_entryNum_, &_bucketFound_ );
                    // access the data
                    bucketEntry**  _entryTable_ = (_bucketFound_)->data;
                    node* _clique_ = _entryTable_[_entryNum_]->clique;

                    node* _tempNode_; 
                    _tempNode_=_clique_;
                    // go through each member of the clique
                    while(_tempNode_ != NULL){
                        e_ = (bucketEntry*)_tempNode_->data;
                        // remove it but not delete it yet 
                        _n_ = removeNode(&(e_->notClique),clique2);
                        if( _n_ != NULL ) toDel = _n_;
                        // go to next one
                        _tempNode_=_tempNode_->next;
                    }
                    // now delete it
                    if(toDel!=NULL){ free(toDel); toDel=NULL; }
	        	}

	        	t = t->next;
	        }
			// destroy the previous one      
	        destroyListOfStrings(notClique2,false);
	    }

	    /*
	    now nodes:
	    clique1 = merged lists of given cliques
	    notClique1 = merged lists of given no cliques
	    */

        // now adjust the pointers of all items in clique to show to the same list(clique)
        bucketEntry* entry;
        // go through the clique (usually only the elements of the second)
	    node* tempNode; 
	    tempNode=clique1;
        // adjust -1 clique to the rest of the clique
        while(tempNode != NULL){
        	entry = (bucketEntry*)tempNode->data;
            entry->clique=clique1;
            entry->notClique=notClique1;
            tempNode=tempNode->next;
        }

    }

    return;
}

void adjustPointers(hashTable* ht, unsigned int bucketSize, bucket** bucketFound1, unsigned int entryNum1, bucket** bucketFound2, unsigned int entryNum2 ){

	// get the cliques
    bucketEntry**  entryTable1 = (*bucketFound1)->data;
    node* clique1 = entryTable1[entryNum1]->clique;

    bucketEntry** entryTable2 = (*bucketFound2)->data;
    node* clique2 = entryTable2[entryNum2]->clique;

    // now add to list of noCliques each other 
    bucketEntry* entry;

    // for each clique mark the other as -1
    node* n;
    // go through the 1st clique
    node* tempNode=clique1;
    bool firstIter=true;

    while(tempNode != NULL){
        entry = (bucketEntry*)tempNode->data;
        // the point is that the members of the same clique should 
        // have the same notClique (pointer is the same)
        if( !addrFoundinList(entry->notClique,clique2) ){
        	if(firstIter){
        		entry->notClique = appendList(entry->notClique,clique2);
        		n = entry->notClique;
        		firstIter=false;
        	
        	}else entry->notClique = n;
    	}else break;

        tempNode=tempNode->next;
    }

    // go through the 2nd clique
    tempNode=clique2;
    firstIter=true;

    while(tempNode != NULL){
        entry = (bucketEntry*)tempNode->data;
        if( !addrFoundinList(entry->notClique,clique1) ){
            if(firstIter){
        		entry->notClique = appendList(entry->notClique,clique1);
        		n = entry->notClique;
        		firstIter=false;
        	}else entry->notClique = n;
    	}else break;

        tempNode=tempNode->next;
    }

    return;
}


//__PRINTS__________________________________________________________________________________
void printBucket(node* b){
    
    if(b == NULL){ /*printf("Empty Bucket\n");*/ return; }
    node* temp = b;

    while(temp!=NULL){

        bucketEntry** entryTable = temp->data;

        for(int i = 0;i<15;i++){

            if((entryTable[i]!=NULL)){

            	node* n;
                

               // printf("\npath: %s wordInfoList: ",entryTable[i]->path);
               // for(int d=0; d<7; d++) printf("%f\t",entryTable[i]->vector[d] ); printf("\n");
                //printList(entryTable[i]->wordInfoList,(void*)printWordInfo); printf("\n");


                bucketEntry* entryIn;
                node* tempNode=entryTable[i]->clique;

                if(tempNode!=NULL && tempNode->next!=NULL){
            		printf("__________________________________________________________________________\n");
                	printf("\n%s is equal to\n\n",entryTable[i]->path );
                
				    while( tempNode !=  NULL ){

				        entryIn = (bucketEntry*)tempNode->data;
				        printf("%s\n", (char*)entryIn->path );

				        tempNode=tempNode->next;
				    }
				}

			    tempNode=entryTable[i]->notClique;
               
               if(tempNode!=NULL) printf("\n%s is not equal to\n\n",entryTable[i]->path );

			    while( tempNode !=  NULL ){

			    	n = tempNode->data;
			    	bucketEntry* entryOut = n->data;
			    	node* innerTemp = entryOut->clique;

			        while( innerTemp != NULL ){
			        	entryIn = (bucketEntry*)innerTemp->data;
			        	printf("%s\n", (char*)entryIn->path );
			        	innerTemp = innerTemp->next;
	        		}
	        		printf("\n");
			        tempNode=tempNode->next;
			    }
            }
        }
        temp=temp->next;
    }
}

void printHT(hashTable* ht){

    //bucket* bp;
    for( unsigned int i=0; i<ht->size; i++ )
        printBucket(ht->table[i]);
        
    return;
}

void makeOutputFile(hashTable* ht, unsigned int bucketSize){

    // if output file exists delete it and then remake it( it is append only so it needs to be destroyed)
   if( access( "output.txt", F_OK ) != -1 )
        // file exists
        if (remove("output.txt") != 0){
            //delete it 
            perror("File was not deleted successfully"); return;
        } 

    // make the output file (append only)
    FILE *outputFile;

    outputFile = fopen("output.txt", "a"); 
    if( outputFile==NULL ){ perror("unable to open file\n"); return; } 

    bucketEntry** entryTable;
    unsigned int numOfEntries =(bucketSize-sizeof(bucket*))/sizeof(bucketEntry*);

    /* go to every entry of the hash table, go to bucket and start exploring
    cliques. Keep track of what cliques you have already visited*/

    for(unsigned int i=0; i<ht->size; i++ ){
        // first entry of hashTable    
        node* temp = ht->table[i];

        while(temp!=NULL){

            entryTable = temp->data;
            
            for( unsigned int j=0; j<numOfEntries && entryTable[j]!=NULL; j++){
                // if this clique hasn't been visited before, go and print what it has inside
                if( !addrFoundinList(listOfCliques,entryTable[j]->clique)){
                    listOfCliques=appendList(listOfCliques,entryTable[j]->clique);
                    makeOutputFileList(entryTable[j]->clique, outputFile);
                }else entryTable[j]->clique=NULL;
            }

            temp=temp->next;
        }
    }

    // close file for reading
    fclose(outputFile);
    // we don't need anymore to keep list of visited cliques
    destroyListOfStrings(listOfCliques,false); listOfCliques=NULL;

    return;
}

