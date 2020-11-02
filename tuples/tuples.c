#include "../tuples/tuples.h"

#define NAME_SIZE 100
#define VALUE_SIZE 300

void tupleInitialization(TuplePtr t, char* pName, char* pValue){ 

	/*t = calloc(1,sizeof(Tuple));
	assert(t!=NULL);*/

	t->propertyName = calloc(NAME_SIZE,sizeof(char));
	assert(t->propertyName!=NULL);
	

	strcpy(t->propertyName,pName);

	t->propertyValue = calloc(VALUE_SIZE,sizeof(char));
	assert(t->propertyValue!=NULL);

	strcpy(t->propertyValue,pValue);

	return;
}

void tupleDeletion(TuplePtr t){ 

	free(t->propertyName); t->propertyName=NULL;
	free(t->propertyValue); t->propertyValue=NULL;
	free(t); t=NULL;

	return;
}

void printTuple(TuplePtr t){

	printf("[propertyName] = [%s],[propertyValue] = [%s]\n",t->propertyName,t->propertyValue );

	return;
}
