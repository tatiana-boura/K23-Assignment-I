#include "job_scheduler.h"


int main(void){

	printf("Typa typa kenaki\n");
	unsigned int num_threads;
	scanf("%d",&num_threads);

	printf("num of threads: %d\n",num_threads );

	JobScheduler* js = initialize_scheduler(num_threads);

	Batch* batch1 = calloc(1,sizeof(Batch));
	batch1->start = 0;
	batch1->end = 511;

	Batch* batch2 = calloc(1,sizeof(Batch));
	batch2->start = 512;
	batch2->end = 1024;

	submit_job(js,batch1);
	submit_job(js,batch2);
	
	execute_all_jobs(js);

	wait_all_tasks_finish(js);

	destroy_scheduler(js);

	return 0;
}