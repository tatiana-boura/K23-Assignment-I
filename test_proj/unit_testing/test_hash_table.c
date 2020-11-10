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
        // test if path string is found in hash table(foundInHT is tested below)
        TEST_ASSERT(foundInHT(ht,path,BUCK_SIZE,&entryNum,&bucketFound)==true);
        // test if the path in the bucket is the same as expected
        bucketEntry**  entryTable = bucketFound->data;
        TEST_ASSERT(!strcmp(entryTable[entryNum]->path,path));

        // test if the list contain the same elements as it should be
        node* tempList = listOfTuples;
        node* tempEntryList = entryTable[entryNum]->listOfTuples; 
        for( unsigned int j=0; j<Ntuples; j++ ){
            TEST_ASSERT(!strcmp(tempEntryList->data,tempList->data));
            tempEntryList = tempEntryList->next;
            tempList = tempList->next;
        }
    }

    destroyHT(ht,BUCK_SIZE);
    free(arrayOfNames); arrayOfNames=NULL;
    return;
}


//___test_foundInHT______________________________________________________________
void test_foundInHT(void){
    /* function to check if foundInHT find the path's bucket and entry position
       correctly in the hash table */
    /* bucketFound := in which bucket entry has been found, 
       entryNum := where in the array */

    // create a hash table of size 1 so we can find the path even when there is 
    // collision (chain)
    hashTable* ht = createHT(1);
    // by making bucket_size equal to 2 entries we make sure there's collisions
    // while testing more than 1 entry in each bucket
    unsigned int bucket_size = 2*sizeof(bucketEntry);

    char* str1 = calloc(STRSIZE,sizeof(char)); strcpy(str1,"string1");
    char* str2 = calloc(STRSIZE,sizeof(char)); strcpy(str2,"string2");
    char* str3 = calloc(STRSIZE,sizeof(char)); strcpy(str3,"string3");

    addtoHT(ht,str1,bucket_size,NULL);
    addtoHT(ht,str2,bucket_size,NULL);
    addtoHT(ht,str3,bucket_size,NULL);

    unsigned int entryNum; bucket* bucketFound;
    // test if each string is found in hash table (collisions are tested)
    TEST_ASSERT(foundInHT(ht,str1,bucket_size,&entryNum,&bucketFound)==true);
    TEST_ASSERT(foundInHT(ht,str2,bucket_size,&entryNum,&bucketFound)==true);
    TEST_ASSERT(foundInHT(ht,str3,bucket_size,&entryNum,&bucketFound)==true);

    char* str4 = calloc(STRSIZE,sizeof(char));
    strcpy(str4,"black sheep");
    // test if str4 is found in hash table [should not be found - not added to ht]
    TEST_ASSERT(foundInHT(ht,str4,bucket_size,&entryNum,&bucketFound)!=true);


	// free not needed memo
	destroyHT(ht, bucket_size );

	return;
}

//___test_changePointers______________________________________________________________
void test_changePointers(void){
    /* function to check if, after going through the (merged) clique, the pointers are 
       pointing to the correct clique */

    unsigned int size = 20;
    hashTable* ht = createHT(size);

    // create two entries
    char* str1 = calloc(STRSIZE,sizeof(char)); strcpy(str1,"string1");
    char* str2 = calloc(STRSIZE,sizeof(char)); strcpy(str2,"string2");
    char* str3 = calloc(STRSIZE,sizeof(char)); strcpy(str3,"string3");
    // add them to ht
    addtoHT(ht,str1,BUCK_SIZE,NULL);
    addtoHT(ht,str2,BUCK_SIZE,NULL);
    addtoHT(ht,str3,BUCK_SIZE,NULL);

    // locate these entries in the hash table (foundInHT has been checked above)
    unsigned int entryNum1; bucket* bucketFound1;
    foundInHT(ht,str1,BUCK_SIZE,&entryNum1,&bucketFound1);

    unsigned int entryNum2; bucket* bucketFound2;
    foundInHT(ht,str2,BUCK_SIZE,&entryNum2,&bucketFound2);
        
    unsigned int entryNum3; bucket* bucketFound3;
    foundInHT(ht,str3,BUCK_SIZE,&entryNum3,&bucketFound3);

    /* these three entries are in the same clique */
    /* NOTE: we change the pointers three times even though two should be enough
    because that is an action that was observed in our project. By making this 
    'cycling' changing there whould be a problem of a cyclic queue where every 
    item points to another. Our program avoids such problems and operates properly.*/
    changePointers(ht,BUCK_SIZE,&bucketFound1,entryNum1,&bucketFound2,entryNum2);
    changePointers(ht,BUCK_SIZE,&bucketFound3,entryNum3,&bucketFound1,entryNum1);
    changePointers(ht,BUCK_SIZE,&bucketFound3,entryNum3,&bucketFound2,entryNum2);

    bucketEntry**  entryTable1 = bucketFound1->data;
    bucketEntry** entryTable2 = bucketFound2->data;
    bucketEntry** entryTable3 = bucketFound3->data;

    // now check if the entries point to the same clique (point to the same address)
    TEST_ASSERT(entryTable1[entryNum1]->clique==entryTable2[entryNum2]->clique);
    TEST_ASSERT(entryTable2[entryNum2]->clique==entryTable3[entryNum3]->clique);
    TEST_ASSERT(entryTable3[entryNum3]->clique==entryTable1[entryNum1]->clique);

    destroyHT(ht,BUCK_SIZE);
    return;
}

TEST_LIST = {
	{ "createHT", test_createHT },
    { "addtoHT", test_addtoHT },
    { "foundInHT", test_foundInHT },
    { "changePointers", test_changePointers },
	{ NULL, NULL } // end of tests
};