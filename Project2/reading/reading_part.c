#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <dirent.h>

#include "functions.h" 
#include "../hash_table/hash_table.h"
#include "../vocabulary/vocabulary.h"
#include "../word_info/word_info.h"
#include "../classification/logisticReg.h"
#include "../classification/data_for_training.h"



#define BUFFER_SIZE 51000
#define BUCKETSIZE 200
#define STRSIZE 100

#define HTVOCSIZE 200
#define BUCKETSIZEVOC 100

node* vocabulary = NULL; unsigned int voc_size = 0;
hashTableVOC* htVOC;

int main(int argc, char* argv[]){
	DIR* main_dir;       // [no inspo for the name] 
	struct dirent* main_dir_entry;
	struct dirent* sub_dir_entry;
	DIR* sub_dir; //sub-directory of main_dir [no inspo for this name either]
	//FILE* json_file;

	// initialize hash for vocabulary
	htVOC = createHTVOC(HTVOCSIZE);

	char* buff = calloc(BUFFER_SIZE,sizeof(char)); assert( buff != NULL );
	memset(buff ,'\0' , BUFFER_SIZE); 
	
	//arbuff : extra buffer to help collect & store 
	//value info in array form from property in .json file 
	char* arbuff = calloc(BUFFER_SIZE,sizeof(char)); assert( arbuff != NULL );
	memset(arbuff ,'\0' , BUFFER_SIZE); 
	
	//-----------Count total number of .json files to initialise hash table size-------------------------- 
	int json_num = count_json(argv[1]);
	
	// create hash table
	unsigned int hash_size = (int)(json_num*0.75);
    hashTable* ht = createHT(hash_size); 


	//---------------Open directory 2013_camera_specs-----------------------------------------------------
	main_dir = opendir(argv[1]);
	if(main_dir == NULL){
		perror("Unable to read directory");
		exit(-1); 
	}
	
	//dirpath: sting to built and store the pathname to each camera spec directory
	char* dirpath = calloc(200,sizeof(char));  
	assert( dirpath != NULL );
	memset(dirpath ,'\0' , 200);   
	strcpy(dirpath, argv[1]);
	strcat(dirpath, "/");        //current dirpath: "2013_camera_specs/"

	//-----[create stopwords list]---------------------------------
	FILE* stopwords = fopen(argv[3], "r");
	if(stopwords == NULL){
		perror("Unable to open file :(");
		exit(-1);
	} 
	fgets(buff, BUFFER_SIZE, stopwords);
					
	//create list for stop words
	node* stopwords_list = NULL;

	//insert words from .txt file to list
	char* tok; 
	char* word;

	tok = strtok(buff, ",");
	while(tok != NULL){
		word = calloc(strlen(tok)+1,sizeof(char));
		assert( word != NULL );
		memset(word, '\0', (strlen(tok)+1)*sizeof(char));
		strcpy(word, tok);

		//printf("%s\n",word);
							
		stopwords_list = appendList(stopwords_list, word);
					
		tok = strtok(NULL, ","); //take next word 
	}
	memset(buff ,'\0' , BUFFER_SIZE); //reinitialise buff
	//printList(stopwords_list, NULL);
	fclose(stopwords);
			
	//------------------------------------------------------------
	int dirs=0; //num of files in main_dir --> 2013_camera_specs
	while( (main_dir_entry = readdir(main_dir)) ){  //check each entry in the directory
		//ignore open current & parent dir
		if((strcmp(main_dir_entry->d_name,".")!=0) && (strcmp(main_dir_entry->d_name,"..")!=0)){
			dirs++;  //count #web dirs
			printf("SUBDIR %d: %s\n", dirs, main_dir_entry->d_name);

			//--------------------Open web-directory entry----------------------------------------------
			strcat(dirpath, main_dir_entry->d_name); //ex. current dirpath: "2013_camera_specs/buy.net"
			sub_dir = opendir(dirpath);
			if(sub_dir == NULL){
				printf("Unable to read sub-directory\n");
				exit(-1); 
			}

			//printf("Sub-directory opend successfully!\n");
			while( (sub_dir_entry = readdir(sub_dir)) ){
				//ignore current & parent dir
				if((strcmp(sub_dir_entry->d_name,".")!=0) && (strcmp(sub_dir_entry->d_name,"..")!=0)){ 
					
					//dirpath: sting to built and store the pathname to each json file
					char* json_path = calloc(200,sizeof(char));
					assert( json_path != NULL );
					memset(json_path ,'\0' , 200);

					//create path to .json file
					strcat(json_path,dirpath);
					strcat(json_path,"/"); 
					strcat(json_path,sub_dir_entry->d_name); //ex. current dirpath: "2013_camera_specs/buy.net/4233.json"

					//--------------create list for json file's words --------------------------------
					node* json_word_list = NULL;
					//convert .json file to list of wordInfo structs
					json_to_wordInfo_list(json_path, &json_word_list, stopwords_list,&vocabulary,&voc_size,htVOC,BUCKETSIZEVOC);
					
					//printf("\nLIST\n");  printList(json_word_list, (void*)printWordInfo);

					//--------------Convert path to be inserted in data structures-------------------
					//"2013_camera_specs/buy.net/4233.json" --> "buy.net//4233"
				    // cut "2013_camera_specs/"
					memmove(json_path,json_path+strlen(argv[1])+1, strlen(json_path)-strlen(argv[1]));
					char* path = convertPath(json_path);   // fix special character '//' and .json
									
					//----ADD PATH & LIST in HT -----------------------------------------------------
					addtoHT(ht, path, BUCKETSIZE, json_word_list);
					
					free(json_path);  
				}
			}	
			closedir(sub_dir);
		}
		memset(dirpath ,'\0' , 200);   
		strcpy(dirpath, argv[1]);
		strcat(dirpath, "/");
	}
	destroyListOfStrings(stopwords_list, true);

	closedir(main_dir);
	free(buff); buff=NULL;
	free(arbuff); arbuff=NULL;
	free(dirpath); dirpath=NULL;

	//printf("vocsize: %d\tjsonnum: %d\n\n",voc_size,json_num);
	//printList(vocabulary,(void*)printWordInfo);printf("\n");printf("\n");

	printf("\nType 'b' for BoW implementation or 't' for tf-idf implementation\n");
	bool flag=true;
	char c;
	scanf("%c",&c);
	while(flag){
		if(c == 'b'){ 
			printf("\nCreate BoW vectors\n");
			make_BoW_vectors( ht, BUCKETSIZE, voc_size, vocabulary, json_num, htVOC, BUCKETSIZEVOC );
			flag=false;
		}else if(c == 't'){
			printf("\nCreate tf-idf vectors\n\n");
			// create vector containing tfidf
			voc_size = make_tfidf_vectors(ht,BUCKETSIZE,voc_size,vocabulary,json_num,htVOC,BUCKETSIZEVOC  ); 
			flag=false;
		}else{ if(c !='\n'){printf("Please type 'b' or 't'\n");}}
		scanf("%c",&c);
	}

	//printf("\ncreate vectors\n");
	// create vector containing tfidf
	//voc_size = make_tfidf_vectors(ht,BUCKETSIZE,voc_size,vocabulary,json_num,htVOC,BUCKETSIZEVOC  );

	//___the hash table has now been created -> start making the cliques__________________________________________________

	FILE* dataset_matches;

	//open dataset file with matches
    dataset_matches = fopen(argv[2], "r");
    if (dataset_matches == NULL){ perror("Unable to read directory"); exit(-1);  }
    
    printf("\nFile [%s] for finding matches opened successfully.\n",argv[2]);
    
    size_t buffSize = 250;
    char* buffMatces;
    buffMatces = calloc(buffSize,sizeof(char)); assert( buffMatces!=NULL);
    
    char* nline;
    const char commas[3] = ",";
    char** data = calloc(buffSize,sizeof(char*)); assert( data!=NULL );
    char* str;

    char* left_spec_id = calloc(STRSIZE,sizeof(char));
    char* right_spec_id = calloc(STRSIZE,sizeof(char));
    char* label = calloc(STRSIZE,sizeof(char)); 

    // read first line of instruction <left_spec_id,right_spec_id,label>
    getline(&buffMatces,&buffSize,dataset_matches); 
    
    //for every line in dataset_matches
    while(getline(&buffMatces,&buffSize,dataset_matches) != EOF){

        if((nline = strchr(buffMatces,'\n')))   //find '\n' and erase
            *nline = 0;
        str = strtok(buffMatces,commas);   //split into strings (get rid of gaps)
        int i = 0;
        while(str!=NULL){ //strtok returns NULL when the whole str is split
            data[i]=str; //store the different strings into a table
            str = strtok(NULL,commas); //strtok needs to start from next parameter
            i++;
        }     

        strcpy(left_spec_id,data[0]);
        strcpy(right_spec_id,data[1]);
        strcpy(label,data[2]); 

        unsigned int entryNum1, entryNum2;
        bucket* bucketFound1;
        bucket* bucketFound2;

        //find left_spec_id and right_spec_id in hash table
        bool found_left=false; bool found_right = false;
        for( unsigned int i=0; i<hash_size; i++ ){
            found_left = foundInHT(ht, left_spec_id, BUCKETSIZE, &entryNum1, &bucketFound1 );
            if(found_left) break;
        }
        for( unsigned int i=0; i<hash_size; i++ ){
            found_right = foundInHT(ht, right_spec_id, BUCKETSIZE, &entryNum2, &bucketFound2 );
            if(found_right) break;
        }

        // if they are found
        if(found_left&&found_right){
        	// if label == 1 : the two items belong to the same clique
        	if(!strcmp(label,"1")) // make both point to the same clique
                changePointers(ht, BUCKETSIZE,&bucketFound1, entryNum1, &bucketFound2, entryNum2 );
            else // label == 0 : the two items don't belong to the same clique -- mark them as -1
            	adjustPointers(ht, BUCKETSIZE,&bucketFound1, entryNum1, &bucketFound2, entryNum2 );
        }       
    }

    //printHT(ht);
    printf("\n%s\n","Make array" );  //Make tfidf array

	//----TRAINING---------------------------------------------------------------------
	float** x_array = calloc(1,sizeof(float*));
	unsigned int* y_array = calloc(1,sizeof(unsigned int));
	unsigned int n;	 // number of differences (size of x and y array)
	create_x_y_array(&x_array,& y_array,ht,BUCKETSIZE, voc_size,&n);
	
	
	//-------create x,y train & valid set-------------
	float** x_train = calloc(1,sizeof(float*));
	unsigned int* y_train = calloc(1,sizeof(unsigned int));
	unsigned int size_of_train_set;
	
	float** x_valid = calloc(1,sizeof(float*));
	unsigned int* y_valid = calloc(1,sizeof(unsigned int));
	unsigned int size_of_valid_set;

	printf("\nCreate train sets & valid x,y sets\n");
	createSets( x_array, y_array, n, &x_train, &size_of_train_set, &x_valid, &size_of_valid_set, &y_train, &y_valid );
	
	//------call gradient_descent() to train model-----
	float bias=0.0;
	float* w = calloc(voc_size,sizeof(float));
	float eta = 0.05; //0.0 < eta < 1.0
	float epsilon = 0.01; //small small number (?)
	
	printf("\nTrain model using gradient_descent\n");
	gradient_descent(x_train, y_train, w, &bias, size_of_train_set, voc_size, eta, epsilon);
	
	//-----------predict-----------------------------
	printf("\nPredict class of x_valid\n");
	bool* ans = predict( x_valid, y_valid, w, bias, size_of_valid_set, voc_size);
	unsigned int t = 0;
	unsigned int f = 0;

	for(int c=0; c<size_of_valid_set ; c++){
		if(ans[c] == true) t++;
		else f++;
	}

	free(ans); ans=NULL;

	printf("\nscore: %f\n", ((float)t/(float)size_of_valid_set) );

	//printf("total: [%d], true: [%d], false: [%d]\n", size_of_valid_set,t,f);
	
	//---free TDIDF array---------
	for(unsigned int i=0;i<n;i++){
		free(x_array[i]);
	}
	free(x_array); free(y_array);

	free(x_valid); free(y_valid);

    //----make the output file---------------------------------------------------------
    makeOutputFile(ht, BUCKETSIZE);
    printf("\nOutput file with name [output.txt] has now been created.\n");

    //--destroy tree and all not needed memo--------------------------------------------
    printf("\nNow, the memory used in this program is being freed.\n");
	destroyListOfWordInfo(vocabulary,(void*)wordInfoDeletion);
    destroyHT(ht,BUCKETSIZE);
    destroyHTVOC(htVOC,BUCKETSIZEVOC);

    free(buffMatces); buffMatces=NULL;
    free(data); data=NULL;
    free(left_spec_id); left_spec_id=NULL;
    free(right_spec_id); right_spec_id=NULL;
    free(label); label=NULL;
    fclose(dataset_matches);
	
	return 0;
}

