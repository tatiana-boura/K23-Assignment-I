#include "job_scheduler.h"


int main(void){

	printf("Typa typa kenaki\n");
	unsigned int num_threads;
	scanf("%d",&num_threads);

	printf("num of threads: %d\n",num_threads );

	JobScheduler* js = initialize_scheduler(num_threads);

	destroy_scheduler(js);

	return 0;
}