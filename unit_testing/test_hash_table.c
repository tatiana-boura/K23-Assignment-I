#include "acutest.h"

#include "../hash_table/hash_table.h"

#define STRSIZE 100
#define BUCK_SIZE 20

//___test_createHT_________________________________________________________
void test_createHT(void){
    unsigned int size = 755;

    hashTable* ht = createHT(size);

    TEST_ASSERT(ht!=NULL);
    TEST_ASSERT(ht->table!=NULL);
    TEST_ASSERT(ht->size==size);

	destroyHT(ht,BUCK_SIZE);

	return;
}

//___test_addtoHT_________________________________________________________
void test_addtoHT(void){
    unsigned int size = 755;
    hashTable* ht = createHT(size);

    //create listOfTuples just like in test_list.c
    unsigned int Nentries=1000; unsigned int Ntuples=100;
	char** arrayOfNames = calloc(Ntuples,sizeof(char*));

	char* path; char* str;

    // for example if k=2 then the entry must be 2 | [0,[0]] , [2,[0,2]] , [4,[0,2,4]]
    for(unsigned int k=0;k<Nentries;k++){

        node* listOfTuples=NULL;
        path = calloc(STRSIZE,sizeof(char));
        sprintf(path,"%d",k);

        for( unsigned int j=0; j<Ntuples; j++ ){
            
            arrayOfNames[j]=calloc(STRSIZE,sizeof(char));
            sprintf(arrayOfNames[j],"%d",k*j);
            TuplePtr t=calloc(1,sizeof(Tuple));

            for( unsigned int i = 0; i<=j; i++ ){
                // in this loop we are making the listOfStrings
                str = calloc(STRSIZE,sizeof(char));
                sprintf(str,"%d",i*k);

                if(i==0) // first time
                    tupleInitialization(t,arrayOfNames[j],str);
                else insertAtValueList(t,str);
            }

            listOfTuples = appendList(listOfTuples,t);
        }

        addtoHT(ht,path,BUCK_SIZE,listOfTuples);

        unsigned int entryNum; bucket* bucketFound;
        // test if path string is found in hash table
        TEST_ASSERT(foundInHT(ht,path,BUCK_SIZE,&entryNum,&bucketFound)==true);
        // test if the path in the bucket is the same as expected
        bucketEntry**  entryTable = bucketFound->data;
        TEST_ASSERT(!strcmp(entryTable[entryNum]->path,path));

        node* tempList = listOfTuples;
        node* tempEntryList = entryTable[entryNum]->listOfTuples; 
        for( unsigned int j=0; j<Ntuples; j++ ){
            TEST_ASSERT(!strcmp(tempEntryList->data,tempList->data));
            tempEntryList = tempEntryList->next;
            tempList = tempList->next;
        }
        free(path); 
    }

    destroyHT(ht,BUCK_SIZE);
    free(arrayOfNames); arrayOfNames=NULL;
    return;
}



TEST_LIST = {
	{ "createHT", test_createHT },
    { "addtoHT", test_addtoHT },
	{ NULL, NULL } // end of tests
};