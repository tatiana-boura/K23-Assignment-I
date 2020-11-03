#include "hash_table.h"

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

//__create_hash_function__________________________________________________________________________________
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
void addtoHT(hashTable* ht, char* key, unsigned int bucketSize){

    bucket* bucketPtr; int index; 
    
    unsigned int numOfEntries = (bucketSize-sizeof(bucket*))/sizeof(bucketEntry*);

    bucketPtr = getBucket(ht,key,&index);

    //if the hash table's index points to NULL -- no buckets, create bucket
    if(bucketPtr==NULL){

        //create entries table and add this entry
        bucketEntry* entry = createEntry(key);
        bucketEntry** entryTable = calloc(numOfEntries,sizeof(bucketEntry*)); assert(entryTable!=NULL); 
        
        for(unsigned int i=0; i<numOfEntries; i++) entryTable[i]=NULL;
        
        entryTable[0]=entry;
        
        bucketPtr = appendList(bucketPtr,entryTable); //create bucket (list of buckets)
        
        ht->table[index] = bucketPtr; //add to hash table
    }
    else{
        //create bucketEntry
        bucketEntry* entry = createEntry(key);
        bucketEntry** entryTable = bucketPtr->data;
        int j=-1;
        for(unsigned int i=0;i<numOfEntries;i++){
            if(entryTable[i]==NULL){
                j=i;    //j holds the position in bucket's entry table in which
                break;  // the new entry should be added
            }
        }
        if(j==-1){  //entry doesn't fit in block -> new list node
            bucketEntry** newEntryTable = malloc(numOfEntries*sizeof(bucketEntry)); 
            
            for(int i=0;i<numOfEntries;i++) newEntryTable[i]=NULL;
            newEntryTable[0]=entry;
            bucketPtr = appendList(bucketPtr,newEntryTable);
        }
        else{
            entryTable[j]=entry;
            bucketPtr->data = entryTable;
        }
        ht->table[index] = bucketPtr; 
    }
    printBucket(bucketPtr);
    return;
}
//_______________________________________________________________________________________________________


//__destroyHashTable______________________________________________________________________________________
void destroyHT(hashTable* ht,unsigned int bucketSize){

    for(unsigned int i=0; i<ht->size; i++)
        destroyList(ht->table[i],&bucketSize,true,(void*)deleteBucketTable);
    
    free(ht->table); ht->table=NULL;
    free(ht); ht=NULL;

    return;
}

void deleteBucketTable(bucketEntry** table,unsigned int* bucketSize){

    unsigned int numOfEntries =(*bucketSize-sizeof(bucket*))/sizeof(bucketEntry*);

    for(int i=0;i<numOfEntries;i++){
        if(table[i])
            //free bucketEntry's fields
            free(table[i]->path); table[i]->path=NULL;
        
        free(table[i]); table[i]=NULL;
    }

    free(table); table=NULL;

    return;
}
//_______________________________________________________________________________________________________

//__getBucket____________________________________________________________________________________________
bucket* getBucket(hashTable* ht, char* key, int* index){

    *index = hash(key,ht);
    bucket* bucketPtr = ht->table[*index];
    return bucketPtr;
}
//_______________________________________________________________________________________________________


bucketEntry* createEntry(char* key){
    //create entry of the bucket with name key
    bucketEntry* entry = calloc(1,sizeof(bucketEntry)); assert(entry!=NULL);

    entry->path = calloc(1,strlen(key)+1); assert(entry->path!=NULL);
    strcpy(entry->path,key);

    return entry;
}

/*Used for patientHashTable returns true if record with key (recordID) is found*/
/*bool foundInHT(hashTable* ht,bucket** bp,char* key,unsigned int numOfEntries,int* index){
    *bp = getBucket(ht,key,index);
    bucket* temp = *bp;
    while(temp!=NULL){
        bucketEntry** entryTable = temp->data;
        for(int i=0;i<numOfEntries;i++){
            if(entryTable[i]!=NULL){
                if(!strcmp(entryTable[i]->name,key)) return true;
            }
        }
        temp = temp->next;
    }
    return false;
}
*/



// Extra -- print bucket function 
void printBucket(node* b){
    if(b == NULL){ printf("Empty Bucket\n"); return; }
    node* temp = b;

    while(temp!=NULL){

        bucketEntry** entryTable = temp->data;
        for(int i = 0;i<12;i++){
            if((entryTable[i]!=NULL)){
                 printf("%s\n",entryTable[i]->path);

            }
        }
        temp=temp->next;
    }
}
