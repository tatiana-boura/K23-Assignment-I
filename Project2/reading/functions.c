#include "functions.h" 

#define STRSIZE 50
#define BUFFER_SIZE 52000


//================================================================================================================
char* convertPath(char* _path_ ){

	char* adjustedPath = calloc(strlen(_path_)-2,sizeof(char)); assert( adjustedPath != NULL);

	unsigned int j=0;

	// do not include .json
	unsigned int k=strlen(_path_)-5;
	for( unsigned int i=0; i < k;i++ ){

		if( _path_[i]=='/' ){
			adjustedPath[j++]='/';
			adjustedPath[j++]='/';
		}else
			adjustedPath[j++]=_path_[i];
	}

	adjustedPath[j]='\0';

	return adjustedPath;
}

//============================================ JSON ================================================================

//function to return #.json files in given directory
int count_json(char* given_directory){
	//printf("given_dir: %s\n", given_directory);

	DIR* main_dir;
	DIR* sub_dir;  
	struct dirent* main_dir_entry;
	struct dirent* sub_dir_entry;
	//FILE* json_file;
	unsigned int count=0;
	
	//create string to store, for each main directory entry, access path 
	char* path = calloc(200, sizeof(char));
	assert( path != NULL );
	memset(path ,'\0' , 200);
	strcpy(path, given_directory);
	strcat(path, "/");        //current path: "<given_directory>/"

	main_dir = opendir(given_directory);
	if (main_dir == NULL){ 
		perror("Unable to read directory"); 
		exit(-1); 
	}
	//main directory opened successfully

	//open each directory entry from the main directory 
	while ((main_dir_entry = readdir(main_dir)) != NULL){ 
        if( (strcmp(main_dir_entry->d_name,".") != 0) && (strcmp(main_dir_entry->d_name,"..")!=0) ){ 
        	strcat(path, main_dir_entry->d_name);
        	sub_dir = opendir(path);      //current path: "<given_directory>/<main_dir_entry->d_name>"
			if (sub_dir == NULL){ 
				perror("Unable to read directory"); 
				exit(-1); 
			}
			//sub directory opened successfully
			while ((sub_dir_entry = readdir(sub_dir)) != NULL){ 
        		if( (strcmp(sub_dir_entry->d_name,".") != 0) && (strcmp(sub_dir_entry->d_name,"..")!=0) ){
        			count++;
        		}
        	}
        	closedir(sub_dir);
        	
        	memset(path ,'\0' , 200);
			strcpy(path, given_directory);
			strcat(path, "/");        //current path: "<given_directory>/"
        }
    }
    closedir(main_dir);
    free(path);
    return count;
}



//=======================================================================================================

//gets json - converts into list of wordInfo
void json_to_wordInfo_list(char* file,  node** l, node* stopwords,node** vocabulary,unsigned int* vocabSize, hashTableVOC* htVOC, unsigned int bucketSize ){
	FILE* json_file;
	json_file = fopen(file, "r");
	if(json_file == NULL){
		perror("Unable to open file :(");
		exit(-1);
	}

	char* buff = calloc(BUFFER_SIZE,sizeof(char)); assert( buff != NULL );
	memset(buff ,'\0' , BUFFER_SIZE); 

	char* property_name = calloc(BUFFER_SIZE,sizeof(char)); assert( property_name != NULL );
	memset(property_name ,'\0' , BUFFER_SIZE); 

	int array_on=0;
	char* tok; 
	char* word;
	
	while( fgets(buff, BUFFER_SIZE, json_file) != NULL ){
		//printf("%s ----- %ld\n",buff,strlen(buff));
		                       
		if(strlen(buff) > 2){     //cut '{' && '}' from start and end of json file                             

			if((buff[strlen(buff)-3] == ']') && (buff[strlen(buff)-2] == ',')){
				array_on = 0;  //reached the end of the array
			}

			if((buff[strlen(buff)-2] == ']') && (buff[strlen(buff)-1] == '\n') ){
				array_on = 0;  //reached the end of the array
			}

			if(array_on == 0){
				if((buff[strlen(buff)-2] == '[') && (buff[strlen(buff)-1] == '\n') ){ //detected array 
					array_on = 1;
				}
				for(int c=0; c<strlen(buff) ;c++){
					if(buff[c] == ':'){
						property_name[c] = '\0';
						break;
					}
					property_name[c] = buff[c];
					buff[c] = ' ';
				}

				buff = str_preprocess_symbols(buff); //now string contains only letters and numbers

				//printf("property: %s \nbuff: %s\n",property_name,buff);
				if(array_on == 1){ //in arrays name caries info
					//printf("%s caries info!\n",property_name);
					//break <property> phrases or sentences..
		        	tok = strtok(buff, " ");
		        	while(tok != NULL){
		        		word = calloc(strlen(tok)+1,sizeof(char));
		        		assert( word != NULL );
						memset(word, '\0', (strlen(tok)+1)*sizeof(char)); 
						strcpy(word, tok);
		        		addToVoc(vocabulary,word,*l,vocabSize,htVOC,bucketSize);
		        		addToWordInfoList(l,word);
		        		//printf("%s\n",word);
		        		tok = strtok(NULL, " ");  //take next word
		        	}
				}

				if( strcmp(buff,"yes")==0 ){ 				//1) If vallue == yes: add property
					property_name = str_preprocess_symbols(property_name); //now string contains only letters and numbers
					//break <property> phrases or sentences..
		        	tok = strtok(property_name, " ");
		        	while(tok != NULL){
		        		word = calloc(strlen(tok)+1,sizeof(char));
		        		assert( word != NULL );
						memset(word, '\0', (strlen(tok)+1)*sizeof(char)); 
						strcpy(word, tok);
		        		if( (atoi(word) == 0) && (strlen(word)>1) && notInlist(stopwords, word) ){ 
							addToVoc(vocabulary,word,*l,vocabSize,htVOC,bucketSize);
				        	addToWordInfoList(l,word);
							//printf("%s\n",word);
				        }else{
				        	free(word);
				        }
		        		//printf("%s\n",word);
		        		tok = strtok(NULL, " ");  //take next word
		        	}
				}else if( strcmp(buff,"no")!=0  ){  		//2)if vallue != yes/no: add value
			        //break <value> phrases or sentences..
				    tok = strtok(buff, " ");
				    while(tok != NULL){
				    	word = calloc(strlen(tok)+1,sizeof(char));
				        assert( word != NULL );
						memset(word, '\0', (strlen(tok)+1)*sizeof(char));
						strcpy(word, tok);

						if( (atoi(word) == 0) && (strlen(word)>1) && notInlist(stopwords, word) ){ 
							addToVoc(vocabulary,word,*l,vocabSize,htVOC,bucketSize);
				        	addToWordInfoList(l,word);
							//printf("%s\n",word);
				        }else{
				        	free(word);
				        }
				        tok = strtok(NULL, " "); //take next word
				    }		        	
		        }//3)if value == no: do NOT add value and property in list of words
		        
			}else{
				buff = str_preprocess_symbols(buff); //now string contains only letters and numbers
				//printf("in array: %s\n",buff);
				tok = strtok(buff, " ");
				while(tok != NULL){
				    word = calloc(strlen(tok)+1,sizeof(char));
				    assert( word != NULL );
					memset(word, '\0', (strlen(tok)+1)*sizeof(char));
					strcpy(word, tok);

					if( (atoi(word) == 0) && (strlen(word)>1) && notInlist(stopwords, word) ){ 
						addToVoc(vocabulary,word,*l,vocabSize,htVOC,bucketSize);
				       	addToWordInfoList(l,word);
						//printf("%s\n",word);
				    }else{
				       	free(word);
				    }
				    tok = strtok(NULL, " "); //take next word
				}		        	
			}

		}
		memset(buff ,'\0' , BUFFER_SIZE);
	} 

	fclose(json_file);
	free(buff);
	free(property_name);
}

//=================================================================================================================
char* str_preprocess_symbols(char* str){
	int i=0;
	//get rid of symbols
	for(i=0 ; i<strlen(str); i++){
		str[i] = tolower(str[i]);  //turn letters to lower case
		//overwrite characters other than letters and numbers 
		if(( (str[i]<48)||(str[i]>57) )&&( (str[i]<97)||(str[i]>122) )){ 
			str[i] = ' ';
		}
	}
	return str;
}


