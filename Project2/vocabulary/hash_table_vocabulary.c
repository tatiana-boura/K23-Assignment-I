#include "hash_table_vocabulary.h"

//__hash_function________________________________________________________________________________________
unsigned int hashVOC(char* string, hashTableVOC* ht){
    
    unsigned int value;
    value = 0;
    
    while(*string != '\0'){ //parse the whole string
        value = *string + 31 * value; //multiplying with a prime number to evenly spread data into buckets
        string++; //move the pointer over 1 char
    }
    return value % ht->size;
}

//__create_hash_table____________________________________________________________________________________
hashTableVOC* createHTVOC(unsigned int size){
    
    hashTableVOC* ht = NULL; 
    ht = calloc(1,sizeof(hashTableVOC)); assert(ht!=NULL);
    
    ht->size = size;

    ht->table = calloc(size,sizeof(bucket*)); assert(ht->table!=NULL); 
    //table of pointers to buckets
    for(unsigned int i=0;i<size;i++) ht->table[i]=NULL;
    
    return ht;
}

//__addToHashTable_______________________________________________________________________________________ 
void addtoHTVOC(hashTableVOC* ht, char* key, unsigned int bucketSize, wordInfo* w){

    bucket* bucketPtr; unsigned int index; 
    
    unsigned int numOfEntries = (bucketSize-sizeof(bucket*))/sizeof(bucketEntryVOC*);

    bucketPtr = getBucketVOC(ht,key,&index);

    //if the hash table's index points to NULL -- no buckets, create bucket
    if(bucketPtr==NULL){
        //create entries table and add this entry
        bucketEntryVOC* entry = createEntryVOC(key,w);
        bucketEntryVOC** entryTable = calloc(numOfEntries,sizeof(bucketEntryVOC*)); assert(entryTable!=NULL); 
        
        for(unsigned int i=0; i<numOfEntries; i++) entryTable[i]=NULL;
        
        entryTable[0]=entry;
        
        bucketPtr = appendList(bucketPtr,entryTable); //create bucket (list of buckets)
        
        ht->table[index] = bucketPtr; //add to hash table

    }else{
        //create bucketEntryVOC
        bucketEntryVOC* entry = createEntryVOC(key,w);
        bucketEntryVOC** entryTable = bucketPtr->data;

        int position=-1;
        for(unsigned int i=0;i<numOfEntries;i++){
            if(entryTable[i]==NULL){
                position=i;    //j holds the position in bucket's entry table in which
                break;         // the new entry should be added
            }
        }
        if(position==-1){  //entry doesn't fit in block -> new list node

            bucketEntryVOC** newEntryTable = calloc(numOfEntries,sizeof(bucketEntryVOC)); assert(newEntryTable!=NULL);
            
            for(int i=0;i<numOfEntries;i++) newEntryTable[i]=NULL;
            newEntryTable[0]=entry;
            
            bucketPtr = appendList(bucketPtr,newEntryTable);
        }else{

            entryTable[position]=entry;
            bucketPtr->data = entryTable;
        }

        ht->table[index] = bucketPtr; 
    }

    return;
}

bucket* getBucketVOC(hashTableVOC* ht, char* key, unsigned int* index){

    *index = hashVOC(key,ht);
    bucket* bucketPtr = ht->table[*index];
    return bucketPtr;
}

bucketEntryVOC* createEntryVOC( char* _key_, wordInfo* _w_){
    
    bucketEntryVOC* entry = calloc(1,sizeof(bucketEntryVOC)); assert(entry!=NULL);

    entry->key = _key_;
    entry->w = _w_;

    return entry;
}

//__destroyHashTable______________________________________________________________________________________

void destroyHTVOC(hashTableVOC* ht,unsigned int bucketSize){

    for(unsigned int i=0; i<ht->size; i++)
        destroyList(ht->table[i],&bucketSize,true,(void*)deleteBucketTableVOC);
    
    free(ht->table); ht->table=NULL;
    free(ht); ht=NULL;

    return;
}

void deleteBucketTableVOC(bucketEntryVOC** table, unsigned int* bucketSize){

    unsigned int numOfEntries =(*bucketSize-sizeof(bucket*))/sizeof(bucketEntryVOC*);

    for(unsigned int i=0;i<numOfEntries;i++){
        if(table[i]){ free(table[i]); table[i]=NULL; }
    }
    free(table); table=NULL;

    return;
}
//_______________________________________________________________________________________________________

bool foundInHTVOC( hashTableVOC* ht, char* _key_, unsigned int bucketSize ){
    // if function returns true(item is found), 
    // adjust counter

    unsigned int index;

    // get first node of bucket list
    bucket* bp = getBucketVOC(ht,_key_,&index);
    bucket* temp = bp;

    bucketEntryVOC** entryTable;

    unsigned int numOfEntries =(bucketSize-sizeof(bucket*))/sizeof(bucketEntryVOC*);
    
    // iterate while temp != NULL and item hasn't been found
    while(temp!=NULL){
        
        entryTable = temp->data;
        
        for(unsigned int i=0;i<numOfEntries;i++){

            if(entryTable[i]!=NULL){
                // if key path has been found, adjust counter
                if(strcmp(entryTable[i]->key,_key_)==0){ 
                    (((wordInfo*)entryTable[i]->w)->count)++;
                    return true;
                }    
            }
        }
        temp = temp->next;
    }
    return false;
}

//__PRINTS__________________________________________________________________________________
void printBucketVOC(node* b, unsigned int bucketSize){

    unsigned int numOfEntries =(bucketSize-sizeof(bucket*))/sizeof(bucketEntryVOC*);
    
    if(b == NULL){ /*printf("Empty Bucket\n");*/ return; }
    node* temp = b;

    while(temp!=NULL){

        bucketEntryVOC** entryTable = temp->data;

        for(int i = 0;i<numOfEntries;i++){

            if((entryTable[i]!=NULL)){

                printf("path: %s \t ",entryTable[i]->key);
                printWordInfo(entryTable[i]->w);
                printf("\n");
            }
        }
        temp=temp->next;
    }

    return;
}

void printHTVOC(hashTableVOC* ht, unsigned int bucketSize){

    for( unsigned int i=0; i<ht->size; i++ )
        printBucketVOC(ht->table[i],bucketSize);
        
    return;
}
