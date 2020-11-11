#include "functions.h" 

#define STRSIZE 50
#define BUFFER_SIZE 52000


//================================================================================================================
char* convertPath(char* _path_ ){

	char* adjustedPath = calloc(strlen(_path_)+2,sizeof(char)); assert( adjustedPath != NULL);

	unsigned int j=0;

	unsigned int k=strlen(_path_)-5;
	for( unsigned int i=0; i < k;i++ ){
	//for( unsigned int i=0; _path_[i] != '\0'; i++ )

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
        			//printf("%d) %s\n", count, sub_dir_entry->d_name ); 
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
    int str_length;
   	// get the first token
   	token = strtok(str, s);
	  
   	// walk through other tokens 
   	while( token != NULL ) {
   		if((strcmp(token,"{")!=0) && (strcmp(token,"}")!=0) && (strcmp(token,": ")!=0) && (strcmp(token,",")!=0)){
   			//printf( " token: %s\n", token );
	   			
   			if((flag == 1)){
   				str_length = strlen(token);
   				property_buff = calloc(str_length+1,sizeof(char));
   				assert( property_buff != NULL );
				memset(property_buff, '\0', (str_length+1)*sizeof(char)); 
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
    //will be freed by destroy tuple
    //free(property_buff);
    //free(value_buff);
}


//function that takes a string {that contains array!} and stores key and value into a tuple
void json_array_handler(char* str, TuplePtr t){
	//printf("given string: %s \n", str);
	const char s[3] = "\"";
	char* property_buff=NULL;
	char* value_buff=NULL;
	char *token;

	const char sa[2] = "$"; // delimeter: \n

	// get the first token 
   	token = strtok(str, sa);
	int flag=0;

   	// walk through other tokens 
   	while( token != NULL ) {
		//printf( " token: %s\n", token );	
		if(flag == 0){
   			property_buff = calloc(strlen(token)+1,sizeof(char));
   			assert( property_buff != NULL );
			memset(property_buff, '\0', (strlen(token)+1)*sizeof(char)); 
   			
			strcpy(property_buff, token);
			if(property_buff == NULL){
		      	strcpy(property_buff, " ");
		    }

		    //printf("%s\n",property_buff );
			//tupleInitialization(t, property_buff, value_buff);  //PREV <-- AYTO MALLON MAS EKANE ZHMIA..
		}
		
		//flag == 1 --> empty line
		int f=0;  //<------------------------------------------------[CHANGE]
		if(flag>=1){
			if((token[0] != ',') && (token[0] != '$') ){

				//strcat(value_buff,token);
				value_buff = calloc(strlen(token)+1,sizeof(char));
				memset(value_buff, '\0', (strlen(token)+1)*sizeof(char));
				strcpy(value_buff, token);
				if(value_buff == NULL){
					strcpy(value_buff, " ");
				}
				//insertAtValueList(t, value_buff1);  //PREV

				//------------------------------[CHANGE]---------------------------------------
				if(f == 0){
					//printf("ARRAY k: %s  - v: %s\n", property_buff, value_buff);
					tupleInitialization(t, property_buff, value_buff);
					f=1;
				}else{
					insertAtValueList(t, value_buff);
				}
				//-------------------------------------------------------------------------

			}
		}
		flag++;
        token = strtok(NULL, s); 
    }
    printf("\n");
    //if( value_buff == NULL ){ free(property_buff); property_buff=NULL; }

    //printf("ARRAY k: %s  - v: %s\n", property_buff, value_buff);

    //will be freed by destroy tuple
    //free(property_buff);
    //free(value_buff);  
}

//=======================================================================================================================