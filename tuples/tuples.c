#include "../tuples/tuples.h"

#define NAME_SIZE 100
#define VALUE_SIZE 300

void tupleInitialization(TuplePtr t, char* pName, char* pValue){ 

	/*t = calloc(1,sizeof(Tuple));
	assert(t!=NULL);*/

	t->propertyName=pName;

	t->propertyValueList = NULL;
	t->propertyValueList = appendList(t->propertyValueList,pValue);

	return;
}

void tupleDeletion(TuplePtr t){ 

	free(t->propertyName); t->propertyName=NULL;

	destroyListOfStrings(t->propertyValueList,true);
	free(t); t=NULL;

	return;
}

void insertAtValueList( TuplePtr t, char* pValue ){

	t->propertyValueList = appendList(t->propertyValueList,pValue);
	return;
}

void printTuple(TuplePtr t){

	if(t==NULL){ printf("NULL tuple\n"); return;}

	printf("[propertyName] = [%s],[propertyValue] = \t",t->propertyName );
	printList(t->propertyValueList,NULL);

	return;
}
