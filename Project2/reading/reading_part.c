/* JJ project, part 1   

*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <dirent.h>

#include "functions.h" 
#include "../hash_table/hash_table.h"

#define BUFFER_SIZE 51000
#define BUCKETSIZE 200
#define STRSIZE 100


int main(int argc, char* argv[]){
	DIR* main_dir;       // [no inspo for the name] 
	struct dirent* main_dir_entry;
	struct dirent* sub_dir_entry;
	DIR* sub_dir; //sub-directory of main_dir [no inspo for this name either]
	FILE* json_file;

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

					//-----------Open json file and print contents-----------------------------------------------
					json_file = fopen(json_path, "r");
					if(json_file == NULL){
						perror("Unable to open file :(");
						exit(-1);
					} 
					//--------------create list for json file's property tuples --------------------------------
					
					node* spec_list = NULL;
					node* json_word_list = NULL;
					
					//------------Convert properties in json files in tuples ------------------------------------ 
					while( fgets(buff, BUFFER_SIZE, json_file) != NULL ){
					
						if((buff[strlen(buff)-1] != '{')&&(buff[strlen(buff)-1] != '}')){ 

							TuplePtr t = calloc(1,sizeof(Tuple));                                      
							
							if((buff[strlen(buff)-2] == '[') && (buff[strlen(buff)-1] == '\n') ){ //(the 2nd condition came up for www.cambuy.com.au/17.json) 
								//DETECTED PROPERTY WITH ARRAY OF VALUES !
								//process: 
								//strcat all the lines of .json file until the end of the array
								//use json_array_handler 

								int array_off=0;
								while(array_off == 0){
									fgets(arbuff, BUFFER_SIZE, json_file);
									buff[strlen(buff)-1] = '#'; //overwrite '\n' with special character '#'
									strcat(buff,arbuff);
									//array is not for the last property
									if((arbuff[strlen(arbuff)-3] == ']') && (arbuff[strlen(arbuff)-2] == ',')){
										array_off = 1;  //reached the end of the array
									}
									//we have array as value for the last property
									if((arbuff[strlen(arbuff)-2] == ']') && (arbuff[strlen(arbuff)-1] == '\n') ){
										array_off = 1;  //reached the end of the array
									}		
								}

								//json_array_handler(buff, t); 

								json_to_word_list_value_array_edition(buff, &json_word_list);
								
								memset(arbuff ,'\0' , BUFFER_SIZE);
								memset(buff ,'\0' , BUFFER_SIZE);

								//---add tuple to spec-list for json file ----------------------
								spec_list = appendList(spec_list, t);
								
							}else{

								// in some files the buffer contains this string "{\n"
								// -- no info so we are not even consdering using it
								if( strcmp(buff,"{\n")!=0 ){
									//json_separator(buff,t);
									
									//break buff into words and add thm to the list
									json_to_word_list(buff, &json_word_list);  //***************************

									memset(arbuff ,'\0' , BUFFER_SIZE);
									memset(buff ,'\0' , BUFFER_SIZE);

									//---add tuple to spec-list for json file ----------------------
									spec_list = appendList(spec_list, t);

								}else{ free(t); t=NULL; }
							}
						}
					}
				
					//printf("\nLIST\n");  printList(spec_list, (void*)printTuple); 
					
					//printf("\nLIST\n");  printList(json_word_list, NULL);   //*******************************
					//REMEMBER TO DESTROY THOSE LISTS
					destroyListOfStrings(json_word_list, true);  //to be removed just a reminder[!]

					//--------------Convert path to be inserted in data structures-------------------
					//"2013_camera_specs/buy.net/4233.json" --> "buy.net//4233"
				    // cut "2013_camera_specs/"
					memmove(json_path,json_path+strlen(argv[1])+1, strlen(json_path)-strlen(argv[1]));
					char* path = convertPath(json_path);   // fix special character '//' and .json
										
					//----ADD PATH & LIST in HT -----------------------------------------------------
					addtoHT(ht, path, BUCKETSIZE, spec_list);

					free(json_path);  
					fclose(json_file);
				}
			}	
			closedir(sub_dir);
		}
		memset(dirpath ,'\0' , 200);   
		strcpy(dirpath, argv[1]);
		strcat(dirpath, "/");
	}

	closedir(main_dir);
	free(buff); buff=NULL;
	free(arbuff); arbuff=NULL;
	free(dirpath); dirpath=NULL;

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
    
    //----make the output file---------------------------------------------------------
    makeOutputFile(ht, BUCKETSIZE);
    printf("\nOutput file with name [output.txt] has now been created.\n");

    //--destroy tree and all not needed memo--------------------------------------------
    printf("\nNow, the memory used in this program is being freed.\n");
    destroyHT(ht,BUCKETSIZE);

    free(buffMatces); buffMatces=NULL;
    free(data); data=NULL;
    free(left_spec_id); left_spec_id=NULL;
    free(right_spec_id); right_spec_id=NULL;
    free(label); label=NULL;
    fclose(dataset_matches);
	
	return 0;
}

