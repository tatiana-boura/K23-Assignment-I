#include "job_scheduler.h"

// gcc -g -o test sch_tester.c job_scheduler.c circ_buffer.c ../list/list.c ../../Project2/word_info/word_info.c -pthread

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

	Batch* batch3 = calloc(1,sizeof(Batch));
	batch3->start = 1025;
	batch3->end = 2048;

	Batch* batch4 = calloc(1,sizeof(Batch));
	batch4->start = 2049;
	batch4->end = 3000;

	Batch* batch5 = calloc(1,sizeof(Batch));
	batch5->start = 3001;
	batch5->end = 4000;

	Batch* batch6 = calloc(1,sizeof(Batch));
	batch6->start = 4001;
	batch6->end = 5000;

	submit_job(js,batch1);
	submit_job(js,batch2);
	submit_job(js,batch3);
	submit_job(js,batch4);
	submit_job(js,batch5);
	submit_job(js,batch6);
	
	execute_all_jobs(js);

	wait_all_tasks_finish(js);

	destroy_scheduler(js);

	return 0;
}