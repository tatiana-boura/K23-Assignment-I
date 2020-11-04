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

int main(int argc, char* argv[]){
	//char* dataset_x;
	int error;
	DIR* Dataset_X;       // [no inspo for the name] 
	struct dirent* entry;
	struct dirent* sub_entry;
	DIR* dirX; //sub-directory of Dataset_X [no inspo for this name either]
	FILE* json_spec_file;
	char* buff = calloc(200,sizeof(char));


	//open directory 2013_camera_specs
	Dataset_X = opendir(argv[1]);
	if(Dataset_X == NULL){
		perror("Unable to read directory");
		return 1;
	}else{
		perror("Directory opend successfully!\n");		
	}

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

	int dirs=0; //num of files in Dataset_X --> 2013_camera_specs
	while( entry = readdir(Dataset_X) ){  //check each entry in the directory
		
		if((strcmp(entry->d_name,".")==0) || (strcmp(entry->d_name,"..")==0)){
			//do not open current & parent dir again
			//printf("Not interested to . or ..\n");
		}else{
			dirs++;  //count #web dirs
			printf("SUBDIR %d: %s\n", dirs, entry->d_name);

			//open web-directory entry
			strcat(dirpath, entry->d_name); //current dirpath: "2013_camera_specs/<entry->d_name>"
			dirX = opendir(dirpath);
			if(dirX == NULL){
				printf("Unable to read sub-directory\n");
				return 1;
			}else{
				//printf("Sub-directory opend successfully!\n");
				printf("Lets read some Json stuff!\n");
				while( sub_entry = readdir(dirX) ){
					if((strcmp(sub_entry->d_name,".")==0) || (strcmp(sub_entry->d_name,"..")==0)){
						//do not open current & parent dir again
						//printf("Not interested to . or ..\n");
					}else{
						//printf("\tjsonFILE: %s\n", sub_entry->d_name);  // <------------WORKS :D
						
						//create path to json file
						strcat(json_path,dirpath);
						strcat(json_path,"/");
						strcat(json_path,sub_entry->d_name);

						//open json file and print contents
						json_spec_file = fopen(json_path, "r");
						if(json_spec_file == NULL){
							perror("Unable to open file :(");
							return 1;
						}else{
							//printf("Oh!It opened!\n");
							while( fgets(buff, 200, json_spec_file) != NULL ){
								//puts(buff);                                    // <------------WORKS :D
							}
						}
						fclose(json_spec_file);

						memset(json_path ,'\0' , 200);
					}
				}	
			}	
			closedir(dirX);
		}

		memset(dirpath ,'\0' , 200);   
		strcpy(dirpath, argv[1]);
		strcat(dirpath, "/");
	}

	closedir(Dataset_X);
	return 0;
}

/*
Testing code that finds the sum of json files to create hash table (use size accordingly)
//Get number of subdirectories (apart from /. and /..)
    //open given directory
    struct dirent *dp;
    DIR *d;
    //unable to open directory stream
    if (!(d= opendir("test"))) { perror("opendir"); exit(-1); }

    unsigned int num_of_dir=0;
    while ((dp = readdir(d)) != NULL) 
        if(!((!(strcmp(dp->d_name,".")))||(!(strcmp(dp->d_name,".."))))) num_of_dir++;
    closedir(d);
    rewinddir(d);

    unsigned int hash_size = num_of_dir;

    hashTable* hash = createHT(hash_size); 
*/