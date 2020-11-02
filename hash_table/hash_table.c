#include "hash_table.h"

unsigned int hash(char* string, hashTable* ht){
    unsigned int value;
    value = 0;
    while(*string != '\0'){ //parse the whole string
        value = *string + 31 * value; //multiplying with a prime num to evenly spread data into buckets
        string++; //move the pointer over 1 char
    }
    return value % ht->size;
}

hashTable* createHT(unsigned int size){
    hashTable* ht = NULL;
    ht = malloc(sizeof(hashTable));
    ht->size = size;
    ht->table = malloc(size*sizeof(bucket*));   //table of pointers to buckets
    for(int i=0;i<size;i++)
        ht->table[i]=NULL;
    return ht;
}


void destroyHT(hashTable* ht,unsigned int bucketSize){
    for(int i=0;i<ht->size;i++){
        destroyList(ht->table[i],&bucketSize,true,(void*)deleteBucketTable);
    }
    free(ht->table);
    free(ht);
}

void deleteBucketTable(bucketEntry** table,unsigned int* bucketSize){
    unsigned int numOfEntries =(*bucketSize-sizeof(bucket*))/sizeof(bucketEntry*);
    for(int i=0;i<numOfEntries;i++){
        if(table[i]){
            //free bucketEntry's fields
            free(table[i]->path);
        }
        free(table[i]);
    }
    free(table);
}

bucketEntry* createEntry(char* key){
    //create entry of the bucket with name key, and tree with root node (d,p)
    bucketEntry* entry = malloc(sizeof(bucketEntry));
    entry->path = malloc(strlen(key)+1);
    strcpy(entry->path,key);
    return entry;
}

bucket* getBucket(hashTable* ht, char* key, int* index){
    *index = hash(key,ht);
    bucket* bp = ht->table[*index];
    return bp;
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

void addtoHT(hashTable* ht, char* key, unsigned int bucketSize){
    bucket* bp; int index; 
    unsigned int numOfEntries =( bucketSize-sizeof(bucket*))/sizeof(bucketEntry*);

    bp = getBucket(ht,key,&index);

    //if the hash table's index points to NULL -- no buckets, add right away
    if(bp==NULL){
        //create bucketEntry
        bucketEntry* entry = createEntry(key);
        //create entries table and add this entry
        bucketEntry** entryTable = malloc(numOfEntries*sizeof(bucketEntry*)); 
        for(int i=0;i<numOfEntries;i++){
            entryTable[i]=NULL;
        }
        entryTable[0]=entry;
        //create bucket (list of buckets)
        bp = appendList(bp,entryTable);
        //add to hash table
        ht->table[index] = bp; 
    }
    else{
        //create bucketEntry
        bucketEntry* entry = createEntry(key);
        bucketEntry** entryTable = bp->data;
        int j=-1;
        for(int i=0;i<numOfEntries;i++){
            if(entryTable[i]==NULL){
                j=i;    //j holds the position in bucket's entry table in which
                break;  // the new entry should be added
            }
        }
        if(j==-1){  //entry doesn't fit in block -> new list node
            bucketEntry** newEntryTable = malloc(numOfEntries*sizeof(bucketEntry)); 
            for(int i=0;i<numOfEntries;i++)
                newEntryTable[i]=NULL;
            newEntryTable[0]=entry;
            bp = appendList(bp,newEntryTable);
        }
        else{
            entryTable[j]=entry;
            bp->data = entryTable;
        }
        ht->table[index] = bp; 
    }
    printBucket(bp);
}

// Extra -- print bucket function 
void printBucket(node* buck){
    if(buck == NULL) { printf("Oops! Empty Bucket.\n"); return; }
    node* temp = buck;
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
