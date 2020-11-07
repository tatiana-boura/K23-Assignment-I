
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	FILE* dataset_matches;
	int error;
	char* buff = calloc(200,sizeof(char));

	//open dataset file with matches
    dataset_matches = fopen(argv[1], "r");
    if (dataset_matches == NULL){ perror("Unable to read directory"); exit(-1);  }
    printf("File %s opened successfully.\n",argv[1]);
    
    
    
    
    
    
    return 0;
}