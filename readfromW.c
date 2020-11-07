//compile: gcc -o readfromW readfromW.c ./list/list.c ./hash_table/hash_table.c ./tuples/tuples.c
//run: ./readfromW ./datasets/sigmod_large_labelled_dataset.csv


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./hash_table/hash_table.h"

#define BUCKETSIZE 50
#define HTSIZE 2

int main(int argc, char* argv[]){

    
    
    
    
    //___INITIALIZING_HASH_TABLE____________________________________________________ 

	char* s1 = calloc(100,sizeof(char)); strcpy(s1,"www.ebay.com//53278");
	char* s2 = calloc(100,sizeof(char)); strcpy(s2,"www.garricks.com.au//31");
	char* s3 = calloc(100,sizeof(char)); strcpy(s3,"www.ebay.com//24817");
	char* s4 = calloc(100,sizeof(char)); strcpy(s4,"www.ebay.com//58782");
	char* s5 = calloc(100,sizeof(char)); strcpy(s5,"www.ebay.com//42055");
	char* s6 = calloc(100,sizeof(char)); strcpy(s6,"www.ebay.com//43019");


	char* k1 = calloc(20,sizeof(char)); strcpy(k1,"one");
	char* k2 = calloc(20,sizeof(char)); strcpy(k2,"two");
	char* k3 = calloc(20,sizeof(char)); strcpy(k3,"three");
	char* k4 = calloc(20,sizeof(char)); strcpy(k4,"four");
	char* k5 = calloc(20,sizeof(char)); strcpy(k5,"five");
	char* k6 = calloc(20,sizeof(char)); strcpy(k6,"six");
	char* k7 = calloc(20,sizeof(char)); strcpy(k7,"seven");
	char* k8 = calloc(20,sizeof(char)); strcpy(k8,"eight");
	char* k9 = calloc(20,sizeof(char)); strcpy(k9,"nine");
	char* k10 = calloc(20,sizeof(char)); strcpy(k10,"ten");
	char* k11 = calloc(20,sizeof(char)); strcpy(k11,"eleven");
	char* k12 = calloc(20,sizeof(char)); strcpy(k12,"twelve");
	char* k13 = calloc(20,sizeof(char)); strcpy(k13,"thirteen");
	char* k14 = calloc(20,sizeof(char)); strcpy(k14,"fourteen");
	char* k15 = calloc(20,sizeof(char)); strcpy(k15,"fifteen");
	char* k16 = calloc(20,sizeof(char)); strcpy(k16,"sixteen");
	char* k17 = calloc(20,sizeof(char)); strcpy(k17,"seventeen");
	char* k18 = calloc(20,sizeof(char)); strcpy(k18,"eighteen");
	char* k19 = calloc(20,sizeof(char)); strcpy(k19,"nineteen");
	char* k20 = calloc(20,sizeof(char)); strcpy(k20,"twenty");
	char* k21 = calloc(20,sizeof(char)); strcpy(k21,"twentyone");
	char* k22 = calloc(20,sizeof(char)); strcpy(k22,"twentytwo");
	char* k23 = calloc(20,sizeof(char)); strcpy(k23,"twentythree");
	char* k24 = calloc(20,sizeof(char)); strcpy(k24,"twentyfour");
	char* k25 = calloc(20,sizeof(char)); strcpy(k25,"twentyfive");
	char* k26 = calloc(20,sizeof(char)); strcpy(k26,"twentysix");

	TuplePtr t1 = calloc(1,sizeof(Tuple)); tupleInitialization(t1,k1,k2);
	insertAtValueList(t1,k21); insertAtValueList(t1,k22); insertAtValueList(t1,k23); insertAtValueList(t1,k24);
	TuplePtr t2 = calloc(1,sizeof(Tuple)); tupleInitialization(t2,k3,k4);
	TuplePtr t3 = calloc(1,sizeof(Tuple)); tupleInitialization(t3,k5,k6);
	TuplePtr t4 = calloc(1,sizeof(Tuple)); tupleInitialization(t4,k7,k8);
	insertAtValueList(t4,k25); insertAtValueList(t4,k26);
	TuplePtr t5 = calloc(1,sizeof(Tuple)); tupleInitialization(t5,k9,k10);
	TuplePtr t6 = calloc(1,sizeof(Tuple)); tupleInitialization(t6,k11,k12);
	TuplePtr t7 = calloc(1,sizeof(Tuple)); tupleInitialization(t7,k13,k14);
	TuplePtr t8 = calloc(1,sizeof(Tuple)); tupleInitialization(t8,k15,k16);
	TuplePtr t9 = calloc(1,sizeof(Tuple)); tupleInitialization(t9,k17,k18);
	TuplePtr t10 = calloc(1,sizeof(Tuple)); tupleInitialization(t10,k19,k20);


	node* l1=NULL; l1 = appendList(l1,t1); l1 = appendList(l1,t2); l1 = appendList(l1,t3); l1 = appendList(l1,t4);
	node* l2=NULL; l2 = appendList(l2,t5);
	node* l3=NULL; l3 = appendList(l3,t6); l3 = appendList(l3,t7);
	node* l4=NULL; l4 = appendList(l4,t8);
	node* l5=NULL; l5 = appendList(l5,t9);
	node* l6=NULL; l6 = appendList(l6,t10);


	hashTable* ht = createHT(HTSIZE);

	addtoHT(ht, s1, BUCKETSIZE, l1);
	addtoHT(ht, s2, BUCKETSIZE, l2);
	addtoHT(ht, s3, BUCKETSIZE, l3);
	addtoHT(ht, s4, BUCKETSIZE ,l4);
	addtoHT(ht, s5, BUCKETSIZE ,l5);
	addtoHT(ht, s6, BUCKETSIZE ,l6);

	//__END_INITIALIZING_HASH_TABLE____________________________________________________________________________________


	FILE* dataset_matches;

	//open dataset file with matches
    dataset_matches = fopen(argv[1], "r");
    if (dataset_matches == NULL){ perror("Unable to read directory"); exit(-1);  }
    printf("File %s opened successfully.\n",argv[1]);
    
    size_t buffSize = 250;
    char* buff;
    buff = malloc(buffSize*sizeof(char));
    
    char* nline;
    const char commas[3] = ",";
    char** data = malloc(buffSize*sizeof(char*));
    char* str;

    //int j=0;
    getline(&buff,&buffSize,dataset_matches); // read first line of instruction <left_spec_id,right_spec_id,label>
    
    //for every line in dataset_matches
    while(getline(&buff,&buffSize,dataset_matches) != EOF){  
        if((nline = strchr(buff,'\n')))   //find '\n' and erase
            *nline = 0;
        str = strtok(buff,commas);   //split into strings (get rid of gaps)
        int i = 0;
        while(str!=NULL){ //strtok returns NULL when the whole str is split
            data[i]=str; //store the different strings into a table
            str = strtok(NULL,commas); //strtok needs to start from next parameter
            i++;
        }     

        char* left_spec_id = calloc(100,sizeof(char)); strcpy(left_spec_id,data[0]);
        char* right_spec_id = calloc(100,sizeof(char)); strcpy(right_spec_id,data[1]);
        char* label = calloc(100,sizeof(char)); strcpy(label,data[2]);     
        
        if(!strcmp(label,"1")){ //if label == 1
            //find left_spec_id and right_spec_id in hash table
            unsigned int entryNum1, entryNum2;
            bucket* bucketFound1;
            bucket* bucketFound2;
            
            bool found_left=false; bool found_right = false;
            for( unsigned int i=0; i<HTSIZE; i++ ){
                found_left = foundInHT(ht, left_spec_id, BUCKETSIZE, &entryNum1, &bucketFound1 );
                if(found_left) break;
            }
            for( unsigned int i=0; i<HTSIZE; i++ ){
                found_right = foundInHT(ht, right_spec_id, BUCKETSIZE, &entryNum2, &bucketFound2 );
                if(found_right) break;
            }
            if(found_left&&found_right){
                printf("left: %s\tright: %s\tlabel:%s\n",left_spec_id,right_spec_id,label);
                changePointers(ht, BUCKETSIZE,&bucketFound1, entryNum1, &bucketFound2, entryNum2 );
            }
        }

       // j++;

        free(left_spec_id);free(right_spec_id);free(label);
        //if(j==3)
         //  break;

    }
    
    makeOutputFile(ht, BUCKETSIZE);
    destroyHT(ht,BUCKETSIZE);
    free(buff);
    free(data);

	free(s1); s1=NULL; free(s2); s2=NULL; free(s3); s3=NULL; free(s4); s4=NULL; free(s5); s5=NULL; free(s6); s6=NULL;
	free(k1); k1=NULL; free(k3); k3=NULL; free(k5); k5=NULL; free(k7); k7=NULL; free(k9); k9=NULL; free(k11); k11=NULL; 
	free(k13); k13=NULL; free(k15); k15=NULL; free(k17); k17=NULL; free(k19); k19=NULL; 
    //free(k21); k21=NULL; free(k22); k22=NULL; free(k23); k23=NULL; free(k24); k24=NULL; 
	//free(k25); k25=NULL; free(k26); k26=NULL;
    
    fclose(dataset_matches);
    return 0;
}