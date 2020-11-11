#include "hash_table.h"

node* listOfCliques=NULL;

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

void addtoHT(hashTable* ht, char* key, unsigned int bucketSize, node* _listOfTuples_){

    bucket* bucketPtr; unsigned int index; 
    
    unsigned int numOfEntries = (bucketSize-sizeof(bucket*))/sizeof(bucketEntry*);

    bucketPtr = getBucket(ht,key,&index);

    //if the hash table's index points to NULL -- no buckets, create bucket
    if(bucketPtr==NULL){

        //create entries table and add this entry
        bucketEntry* entry = createEntry(key,_listOfTuples_);
        bucketEntry** entryTable = calloc(numOfEntries,sizeof(bucketEntry*)); assert(entryTable!=NULL); 
        
        for(unsigned int i=0; i<numOfEntries; i++) entryTable[i]=NULL;
        
        entryTable[0]=entry;
        
        bucketPtr = appendList(bucketPtr,entryTable); //create bucket (list of buckets)
        
        ht->table[index] = bucketPtr; //add to hash table
    }
    else{
        //create bucketEntry
        bucketEntry* entry = createEntry(key,_listOfTuples_);
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

    //printBucket(bucketPtr);
    return;
}
//_______________________________________________________________________________________________________


//__destroyHashTable______________________________________________________________________________________

void destroyHT(hashTable* ht,unsigned int bucketSize){

    for(unsigned int i=0; i<ht->size; i++)
        destroyList(ht->table[i],&bucketSize,true,(void*)deleteBucketTable);
    
    free(ht->table); ht->table=NULL;
    free(ht); ht=NULL;

    destroyListOfStrings(listOfCliques,false);


    return;
}

void deleteBucketTable(bucketEntry** table, unsigned int* bucketSize){

    unsigned int numOfEntries =(*bucketSize-sizeof(bucket*))/sizeof(bucketEntry*);

    for(unsigned int i=0;i<numOfEntries;i++){
        if(table[i]){
            //free bucketEntry's fields
            
            // if we haven't come across this clique yet, mark it
            // as visited and then destroy it 
            if( !addrFoundinList(listOfCliques,table[i]->clique)){
                listOfCliques=appendList(listOfCliques,table[i]->clique);
                //printf("%p\n",table[i]->clique);
                destroyListOfStrings(table[i]->clique,false);
            }
            
            destroyListOfTuples(table[i]->listOfTuples,(void*)tupleDeletion); 
            free(table[i]->path); table[i]->path=NULL;
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



bucketEntry* createEntry(char* _path_, node* _listOfTuples_){
    // create entry of the bucket with name key(path)
    bucketEntry* entry = calloc(1,sizeof(bucketEntry)); assert(entry!=NULL);

    /*entry->path = calloc(strlen(_path_)+1,sizeof(char)); assert(entry->path!=NULL);
    strcpy(entry->path,_path_);*/

    entry->path = _path_;

    // create list of tuples <char*,char*>
    entry->listOfTuples = _listOfTuples_;

    // create clique -- list of paths
    entry->clique = NULL; entry->clique = appendList(entry->clique,entry->path);

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

void changePointers(hashTable* ht, unsigned int bucketSize, bucket** bucketFound1, unsigned int entryNum1, bucket** bucketFound2, unsigned int entryNum2 ){

    // first go to the first one and get the clique(list)
    bucketEntry**  entryTable1 = (*bucketFound1)->data;
    node* clique1 = entryTable1[entryNum1]->clique;

    // then go to second one and do the same
    bucketEntry** entryTable2 = (*bucketFound2)->data;
    node* clique2 = entryTable2[entryNum2]->clique;

    // if cliques aren't already the same
    if (clique1!=clique2){
        // merge the two lists
        clique1=mergeTwoLists(clique1,clique2);
    
        // now adjust the pointers of all items in clique to show to the same list(clique)
        unsigned int entryNum; 
        bucket* bucketFound;
        bucketEntry**  entryTable;

        // go through the clique
        node* tempNode=clique1;
        while(tempNode != NULL){
        // for each path
            // find path in hash table 
            foundInHT(ht,(char*)tempNode->data,bucketSize, &entryNum, &bucketFound );
            // make it point to the new clique
            entryTable = bucketFound->data;
            entryTable[entryNum]->clique=clique1;
            tempNode=tempNode->next;
        }
    }
    return;
}


//__PRINTS__________________________________________________________________________________
void printBucket(node* b){
    
    if(b == NULL){ printf("Empty Bucket\n"); return; }
    node* temp = b;

    while(temp!=NULL){

        bucketEntry** entryTable = temp->data;
        for(int i = 0;i<5;i++){
            if((entryTable[i]!=NULL)){
                printList(entryTable[i]->listOfTuples,(void*)printTuple);
                //printf("%s is equal to\n\n",entryTable[i]->path );
                //printList(entryTable[i]->clique,NULL);
                printf("\n");
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
                }
            }

            temp=temp->next;
        }
    }

    // close filw for reading
    fclose(outputFile);
    // we don't need anymore to keep list of visited cliques
    destroyListOfStrings(listOfCliques,false); listOfCliques=NULL;

    return;
}

