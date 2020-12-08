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


/*
json format
{
"key": "value",
"key": "value",
"key": "value"
}
*/

//function that takes a string and stores key and value into a tuple
void json_separator(char* str, TuplePtr t){  
	//printf("given string: %s \n", str);
	const char s[3] = "\""; // delimeter: "

	//string to store key
	char* property_buff;  
	//strig to store value
	char* value_buff;

   	char *token;
    int flag = 0; //flag == 1 buff contains "key" | flag == 2 buff contains "value" 
   	// get the first token
   	token = strtok(str, s);
	  
   	// walk through other tokens 
   	while( token != NULL ) {
   		if((strcmp(token,"{")!=0) && (strcmp(token,"}")!=0) && (strcmp(token,": ")!=0) && (strcmp(token,",")!=0)){
   			//printf( " token: %s\n", token );
	   			
   			if(flag == 1){
   				property_buff = calloc(strlen(token)+1,sizeof(char));
   				assert( property_buff != NULL );
				memset(property_buff, '\0', (strlen(token)+1)*sizeof(char)); 
				strcpy(property_buff, token);

		      	if(token == NULL){
		      		strcpy(property_buff, " ");
		      	}
		      	
		    }else if(flag == 2){

   				value_buff = calloc(strlen(token)+1,sizeof(char));
   				assert( value_buff != NULL );
				memset(value_buff, '\0', (strlen(token)+1)*sizeof(char)); 

		        strcpy(value_buff, token);
		       	//printf("k: %s  - v: %s\n", property_buff, value_buff);
		       	if(value_buff == NULL){
					strcpy(value_buff, " ");
				}
		        tupleInitialization(t, property_buff, value_buff); 

		    }
		    flag++;
		}		
        token = strtok(NULL, s); 
    }
    
    return;
}

//function that takes a string {that contains array!} and stores key and value into a tuple
void json_array_handler(char* str, TuplePtr t){
	//printf("given string: %s \n", str);
	const char s[3] = "\"";
	char* property_buff=NULL;
	char* value_buff=NULL;
	char *token;

	const char sa[2] = "#"; // delimeter: #

	// get the first token 
   	token = strtok(str, sa);
	bool pNameNotYet=false;
	bool firstTime=true;

   	// walk through other tokens 
   	while( token != NULL ) {
			
		if(pNameNotYet == 0){
   			
   			property_buff = calloc(strlen(token)+1,sizeof(char));
   			assert( property_buff != NULL );
			memset(property_buff, '\0', (strlen(token)+1)*sizeof(char)); 
   			
			strcpy(property_buff, token);
			if(property_buff == NULL){
		      	strcpy(property_buff, " ");
		    }
		    // made the property name 
			pNameNotYet=true;

			//if(property_buff==NULL) printf("here\n");

		}else{
			// time to make the values
			if((token[0] != ',') && (token[0] != '#') && (token[0] != ' ')){

				value_buff = calloc(strlen(token)+1,sizeof(char));
				memset(value_buff, '\0', (strlen(token)+1)*sizeof(char));
				strcpy(value_buff, token);
				if(value_buff == NULL){ strcpy(value_buff, " ");}

				if(firstTime){ // then initialize
					//if(property_buff==NULL) printf("here\n");
					tupleInitialization(t, property_buff, value_buff);
					firstTime=false;
				}else // not first time here
					insertAtValueList(t, value_buff);
			}
		}
		// get next value
		printf("^^^^^^^^^^^^^^^^%s\n",token);
        token = strtok(NULL, s); 
        printf("----------------%s\n",token);
    }

    if( value_buff == NULL ){ free(property_buff); property_buff=NULL; }

    return;
}

//=======================================================================================================================

void json_to_word_list(char* str, node** l){  
	//printf("given string: %s \n", str);

	//turn string to lower case
	int i=0;
	while(str[i]){
		str[i] = tolower(str[i]);
		i++;
	}
	const char s[3] = "\""; // delimeter: "

	//string to store key
	char* property_buff;  
	//strig to store value
	char* value_buff;

   	char *token;
    int flag = 0; //flag == 1 buff contains "key" | flag == 2 buff contains "value" 
   	// get the first token
   	token = strtok(str, s);
	  
   	// walk through other tokens 
   	while( token != NULL ) {
   		if((strcmp(token,"{")!=0) && (strcmp(token,"}")!=0) && (strcmp(token,": ")!=0) && (strcmp(token,",")!=0)){
   			//printf( " token: %s\n", token );
	   			
   			if(flag == 1){
   				property_buff = calloc(strlen(token)+1,sizeof(char));
   				assert( property_buff != NULL );
				memset(property_buff, '\0', (strlen(token)+1)*sizeof(char)); 
				strcpy(property_buff, token);

		      	if(token == NULL){
		      		strcpy(property_buff, " ");
		      	}
		      	
		    }else if(flag == 2){

   				value_buff = calloc(strlen(token)+1,sizeof(char));
   				assert( value_buff != NULL );
				memset(value_buff, '\0', (strlen(token)+1)*sizeof(char)); 

		        strcpy(value_buff, token);
		       	//printf("k: %s  - v: %s\n", property_buff, value_buff);
		       	if(value_buff == NULL){
					strcpy(value_buff, " ");
				}
				//now: property_buff = word/phrase/sentence & value_buff = word/phrase/sentence 

		        //add words to list of json words
		        char* tok; 
		        char* word;
		        //do not add (<key name> - no )
				if( strcmp(value_buff,"no")!=0 ){

					//break <property> phrases or sentences..
		        	tok = strtok(property_buff, " ");
		        	while(tok != NULL){
		        		word = calloc(strlen(tok)+1,sizeof(char));
		        		assert( word != NULL );
						memset(word, '\0', (strlen(tok)+1)*sizeof(char)); 
						strcpy(word, tok);
			
		        		*l = appendList(*l, word);
		        		tok = strtok(NULL, " ");  //take next word
		        	}
					
		        	//break <value> phrases or sentences..
			        tok = strtok(value_buff, " ");
			        while(tok != NULL){
			        	word = calloc(strlen(tok)+1,sizeof(char));
			        	assert( word != NULL );
						memset(word, '\0', (strlen(tok)+1)*sizeof(char));
						strcpy(word, tok);

						//it is common to have stuff like  "word,"
						if(word[strlen(word)-1] == ','){
							word[strlen(word)-1] = '\0'; //erase the ','
						} 

						if( (atoi(word) == 0) && (strcmp(word,"yes")!=0) && (strlen(word)>3)){  //strlen(word) >1, 2 or 3 ? 
			        		*l = appendList(*l, word);
			        	}else{
			        		free(word);
			        	}
			        	tok = strtok(NULL, " "); //take next word
			       	}		        	
		        }
		    }

		    flag++;
		}		
        token = strtok(NULL, s); 
    }
    free(value_buff);
    free(property_buff);
    return;
}



//function that takes a string {that contains array!} and stores key and value into a tuple
void json_to_word_list_value_array_edition(char* str,  node** l){
	//printf("given string: %s \n", str);
	const char s[3] = "\"";
	char* property_buff=NULL;
	char* value_buff=NULL;
	char *token;

	const char sa[2] = "#"; // delimeter: #

	// get the first token 
   	token = strtok(str, sa);
	bool pNameNotYet=false;
	//bool firstTime=true;

	char* tok; 
	char* word;

   	// walk through other tokens 
   	while( token != NULL ) {
			
		if(pNameNotYet == 0){
   			
   			property_buff = calloc(strlen(token)+1,sizeof(char));
   			assert( property_buff != NULL );
			memset(property_buff, '\0', (strlen(token)+1)*sizeof(char)); 
   			
			strcpy(property_buff, token);
			if(property_buff == NULL){
		      	strcpy(property_buff, " ");
		    }
		    // made the property name 
			pNameNotYet=true;
			
			//if(property_buff==NULL) printf("here\n");
		}else{
			// time to make the values
			if((token[0] != ',') && (token[0] != '#') && (token[0] != ' ')){

				value_buff = calloc(strlen(token)+1,sizeof(char));
				memset(value_buff, '\0', (strlen(token)+1)*sizeof(char));
				strcpy(value_buff, token);
				if(value_buff == NULL){ strcpy(value_buff, " ");}

				//to add <value> into list of words
				//break <value> phrases or sentences..
				tok = strtok(value_buff, " ");
			    while(tok != NULL){
			        word = calloc(strlen(tok)+1,sizeof(char));
			        assert( word != NULL );
					memset(word, '\0', (strlen(tok)+1)*sizeof(char));
					strcpy(word, tok);
					

					//it is common to have stuff like  "word,"
					if(word[strlen(word)-1] == ','){
						word[strlen(word)-1] = '\0'; //erase the ','
					} 

					if( (atoi(word) == 0) && (strlen(word)>3)){  //strlen(word) >1, 2 or 3 ? 
			        	*l = appendList(*l, word);
			        }else{
			        	free(word);
			        }
			       	tok = strtok(NULL, " "); //take next word
			    }
			}
		}
        token = strtok(NULL, s); // get next value
    }

    //standard form: *space*"property":*space*[  --> property (remove " " and :)
	property_buff[4] = ' ';
	property_buff[strlen(property_buff)-4] = '\0';

	//to add <property> into list of words
	//break <property> phrases or sentences..
   	tok = strtok(property_buff, " ");
	while(tok != NULL){
		word = calloc(strlen(tok)+1,sizeof(char));
		assert( word != NULL );
		memset(word, '\0', (strlen(tok)+1)*sizeof(char)); 
		strcpy(word, tok);
			
		if( strlen(word)>3){  //strlen(word) >1, 2 or 3 ?
		    *l = appendList(*l, word);
		}else{
			free(word);
		}
		       
		tok = strtok(NULL, " ");  //take next word
	}

    if( value_buff == NULL ){ free(property_buff); property_buff=NULL; }

    return;
}