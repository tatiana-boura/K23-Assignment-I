/* JJ project, part 1   
This code is supposed to open and read JSON files

what it is capable for at the moment:
call format: ./reading_part 2013_camera_specs
-opens 2013_camera_specs directory
>for each entry of 2013_camera_specs
	*prints directory name
	*'creates' path to access directory
	*opens this directory
	*prints  all .json file entries
	>for each .json file
		+prints json name
		+creates a path to access .json file
		+opens .json file
		+prints contents of .json file  

-------------------------------------------
extra notes:
- ignores current '.' & parent '..' directories while checking a directory's entries
- to open all the web spec  directory entries from 2013_camera_specs
  creates each pathname "2013_camera_specs/<entry->d_name>" in dirpath using strcat


>>>>> CODE LINES 91 & 106: commented for obvious reasons, get rid of the '//' stuff to observe the printfs   <<<<<  
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <dirent.h>


//function to return #.json files in given directory
int count_json(char* given_directory){
	printf("given_dir: %s\n", given_directory);

	DIR* main_dir;
	DIR* sub_dir;  
	struct dirent* main_dir_entry;
	struct dirent* sub_dir_entry;
	FILE* json_file;
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
Name changes
============
Dataset_X --> main_dir
dirX -->  sub_dir
json_spec_file --> json_file
entry--> main_dir_entry
sub_entry--> sub_dir_entry
*/


int main(int argc, char* argv[]){
	//char* main_dir;
	int error;
	DIR* main_dir;       // [no inspo for the name] 
	struct dirent* main_dir_entry;
	struct dirent* sub_dir_entry;
	DIR* sub_dir; //sub-directory of main_dir [no inspo for this name either]
	FILE* json_file;
	char* buff = calloc(200,sizeof(char));

	int num = count_json(argv[1]);
	printf("num = %d\n", num);

	//for later..
	//unsigned int hash_size = num_of_dir;
    //hashTable* hash = createHT(hash_size); 

	//open directory 2013_camera_specs
	main_dir = opendir(argv[1]);
	if(main_dir == NULL){
		perror("Unable to read directory");
		exit(-1); 
	}
	//main directory opened successfully!

	//dirpath: sting to built and store the pathname to each camera spec directory
	char* dirpath = calloc(200,sizeof(char));  
	assert( dirpath != NULL );
	memset(dirpath ,'\0' , 200);   
	strcpy(dirpath, argv[1]);
	strcat(dirpath, "/");        //current dirpath: "2013_camera_specs/"

	//dirpath: sting to built and store the pathname to each json file
	char* json_path = calloc(200,sizeof(char));  
	assert( json_path != NULL );
	memset(json_path ,'\0' , 200); 

	int dirs=0; //num of files in main_dir --> 2013_camera_specs
	while( main_dir_entry = readdir(main_dir) ){  //check each entry in the directory
		//ignore open current & parent dir
		if((strcmp(main_dir_entry->d_name,".")!=0) && (strcmp(main_dir_entry->d_name,"..")!=0)){
			dirs++;  //count #web dirs
			printf("SUBDIR %d: %s\n", dirs, main_dir_entry->d_name);

			//open web-directory entry
			strcat(dirpath, main_dir_entry->d_name); //current dirpath: "2013_camera_specs/<entry->d_name>"
			sub_dir = opendir(dirpath);
			if(sub_dir == NULL){
				printf("Unable to read sub-directory\n");
				exit(-1); 
			}

			//printf("Sub-directory opend successfully!\n");
			printf("Lets read some Json stuff!\n");
			while( sub_dir_entry = readdir(sub_dir) ){
				//ignore current & parent dir
				if((strcmp(sub_dir_entry->d_name,".")!=0) && (strcmp(sub_dir_entry->d_name,"..")!=0)){ 
					//printf("\tjsonFILE: %s\n", sub_dir_entry->d_name);  // <------------WORKS :D
						
					//create path to json file
					strcat(json_path,dirpath);
					strcat(json_path,"/");
					strcat(json_path,sub_dir_entry->d_name);

					//open json file and print contents
					json_file = fopen(json_path, "r");
					if(json_file == NULL){
						perror("Unable to open file :(");
						exit(-1);
					} 
					//printf("Oh!It opened!\n");
					/*
					GONNA ADD STUFF - to 'break' .json content into tupples
					*/
					while( fgets(buff, 200, json_file) != NULL ){
						//puts(buff);                                    // <------------WORKS :D
					}
					fclose(json_file);

					memset(json_path ,'\0' , 200);
				}
			}	
			
			closedir(sub_dir);
		}

		memset(dirpath ,'\0' , 200);   
		strcpy(dirpath, argv[1]);
		strcat(dirpath, "/");
	}

	closedir(main_dir);
	return 0;
}

