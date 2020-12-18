#include "hash_table_pairs.h"

//__hash_function________________________________________________________________________________________
unsigned int hashPair(char* string1, char* string2, hashTablePair* ht){
    
    unsigned long int value = 0;
    
    while(*string1 != '\0'){ //parse the whole string
        value += *string1; //multiplying with a prime number to evenly spread data into buckets
        string1++; //move the pointer over 1 char
    }

    while(*string2 != '\0'){ //parse the whole string
        value += *string2; //multiplying with a prime number to evenly spread data into buckets
        string2++; //move the pointer over 1 char
    }
    //printf("%d\t",value % ht->size );
    return value % ht->size;
}

//__create_hash_table____________________________________________________________________________________
hashTablePair* createHTPair(unsigned int size){
    
    hashTablePair* ht = NULL; 
    ht = calloc(1,sizeof(hashTablePair)); assert(ht!=NULL);
    
    ht->size = size;

    ht->table = calloc(size,sizeof(bucket*)); assert(ht->table!=NULL); 
    //table of pointers to buckets
    for(unsigned int i=0;i<size;i++) ht->table[i]=NULL;
    
    return ht;
}

//__addToHashTable_______________________________________________________________________________________ 
void addtoHTPair(hashTablePair* ht, char* key1, char* key2, unsigned int bucketSize ){

    bucket* bucketPtr; unsigned int index; 
    
    unsigned int numOfEntries = (bucketSize-sizeof(bucket*))/sizeof(bucketEntryPair*);

    bucketPtr = getBucketPair(ht,key1,key2,&index);

    //if the hash table's index points to NULL -- no buckets, create bucket
    if(bucketPtr==NULL){
        //create entries table and add this entry
        bucketEntryPair* entry = createEntryPair(key1,key2);
        bucketEntryPair** entryTable = calloc(numOfEntries,sizeof(bucketEntryPair*)); assert(entryTable!=NULL); 
        
        for(unsigned int i=0; i<numOfEntries; i++) entryTable[i]=NULL;
        
        entryTable[0]=entry;
        
        bucketPtr = appendList(bucketPtr,entryTable); //create bucket (list of buckets)
        
        ht->table[index] = bucketPtr; //add to hash table

    }else{
        //create bucketEntryPair
        bucketEntryPair* entry = createEntryPair(key1,key2);
        bucketEntryPair** entryTable = bucketPtr->data;

        int position=-1;
        for(unsigned int i=0;i<numOfEntries;i++){
            if(entryTable[i]==NULL){
                position=i;    //j holds the position in bucket's entry table in which
                break;         // the new entry should be added
            }
        }
        if(position==-1){  //entry doesn't fit in block -> new list node

            bucketEntryPair** newEntryTable = calloc(numOfEntries,sizeof(bucketEntryPair)); assert(newEntryTable!=NULL);
            
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

bucket* getBucketPair(hashTablePair* ht, char* key1, char* key2, unsigned int* index){

    *index = hashPair(key1,key2,ht);
    bucket* bucketPtr = ht->table[*index];
    return bucketPtr;
}

bucketEntryPair* createEntryPair( char* _key1_, char* _key2_){
    
    bucketEntryPair* entry = calloc(1,sizeof(bucketEntryPair)); assert(entry!=NULL);

    entry->key1 = _key1_;
    entry->key2 = _key2_;

    return entry;
}

//__destroyHashTable______________________________________________________________________________________
void destroyHTPair(hashTablePair* ht,unsigned int bucketSize){

    for(unsigned int i=0; i<ht->size; i++)
        destroyList(ht->table[i],&bucketSize,true,(void*)deleteBucketTablePair);
    
    free(ht->table); ht->table=NULL;
    free(ht); ht=NULL;

    return;
}

void deleteBucketTablePair(bucketEntryPair** table, unsigned int* bucketSize){

    unsigned int numOfEntries =(*bucketSize-sizeof(bucket*))/sizeof(bucketEntryPair*);

    for(unsigned int i=0;i<numOfEntries;i++){
        if(table[i]){ free(table[i]); table[i]=NULL; }
    }
    free(table); table=NULL;

    return;
}
//_______________________________________________________________________________________________________

bool foundInHTPair( hashTablePair* ht, char* _key1_, char* _key2_, unsigned int bucketSize ){
    // if function returns true(item is found), 
    // adjust counter

    unsigned int numOfEntries =(bucketSize-sizeof(bucket*))/sizeof(bucketEntryPair*);

    unsigned int index;

    // get first node of bucket list
    bucket* bp = getBucketPair(ht,_key1_,_key2_,&index);
    bucket* temp = bp;

    bucketEntryPair** entryTable;
 
    // iterate while temp != NULL and item hasn't been found
    while(temp!=NULL){
        
        entryTable = temp->data;
        
        for(unsigned int i=0;i<numOfEntries;i++){

            if(entryTable[i]!=NULL){
                // if key path has been found, adjust counter
                if( strcmp(entryTable[i]->key1,_key1_)==0 && strcmp(entryTable[i]->key2,_key2_)==0 ){
                    return true;
                }

                if( strcmp(entryTable[i]->key1,_key2_)==0 && strcmp(entryTable[i]->key2,_key1_)==0 ){
                    return true;
                }
            }
        }
        temp = temp->next;
    }

    return false;
}

//__PRINTS__________________________________________________________________________________
void printBucketPair(node* b, unsigned int bucketSize){

    unsigned int numOfEntries =(bucketSize-sizeof(bucket*))/sizeof(bucketEntryPair*);
    
    if(b == NULL){ /*printf("Empty Bucket\n");*/ return; }
    node* temp = b;

    while(temp!=NULL){

        bucketEntryPair** entryTable = temp->data;

        for(int i = 0;i<numOfEntries;i++){

            if((entryTable[i]!=NULL)){

                printf("path1: %s \t ",entryTable[i]->key1);
                printf("path2: %s \t ",entryTable[i]->key2);
                printf("\n");
            }
        }
        temp=temp->next;
    }

    return;
}

void printHTPair(hashTablePair* ht, unsigned int bucketSize){

    for( unsigned int i=0; i<ht->size; i++ )
        printBucketPair(ht->table[i],bucketSize);
        
    return;
}
