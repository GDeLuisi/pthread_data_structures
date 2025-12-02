#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "pthread_groups.h"
#include "queue.h"
#include <unistd.h>
#define TEST_SUITE_ON 
#define DEBUG
#include "test_framework.h"
#include "debug.h"
typedef void* ( *target )( void *);
void* test_func(void* arg){
	int * val_arg = (int*)arg;
	pthread_t self_id = pthread_self();
	sleep(3);
	printf("I'm executed inside the %lu thread\n",self_id);
	printf("Received this arg %d\n",*val_arg);
	return val_arg;
}
BEGIN_TESTS;
void* outputs[3];
int i;
TEST("threadgroup_creation"){
	target t_targets[3];
	int* my_args[3] = {(int*)11,(int*)24,(int*)63};
	void ** my_target_args = (void**) my_args;
	t_targets[0] = &test_func;
	t_targets[1] = &test_func;
	t_targets[2] = &test_func;
	struct ThreadGroup* tg = create_thread_group(t_targets,my_target_args,3);
	unsigned int size = tg->size;
	dprint("Tg size %u\n",size);
	join_thread_group(tg,outputs);
	for (i = 0; i < size ; ++i)
		dprint("Output %d : %d \n",i,*(int *)outputs[i]);
	TEST_ASSERTION(0,0);
}
TEST("multi_threadgroup"){
	target t_targets[3];
	int* my_args[3] = {(int*)11,(int*)24,(int*)63};
	int* my_args_2[3] = {(int*)22,(int*)34,(int*)73};
	void ** my_target_args = (void**) my_args;
	void ** my_target_args_2 = (void**) my_args_2;
	t_targets[0] = &test_func;
	t_targets[1] = &test_func;
	t_targets[2] = &test_func;
	struct ThreadGroup* tg = create_thread_group(t_targets,my_target_args,3);
	struct ThreadGroup* tg2 =  create_thread_group(t_targets,my_target_args_2,3);
	unsigned int size = tg->size;
	dprint("Tg size %u\n",size);
	join_thread_group(tg,outputs);

	for (i = 0; i < size ; ++i)
		dprint("Output %d : %d \n",i,*(int *)outputs[i]);
	join_thread_group(tg2,outputs);
	for (i = 0; i < size ; ++i)
		dprint("Output %d : %d \n",i,*(int *)outputs[i]);
	TEST_ASSERTION(0,0);
}
TEST("queue_creation"){
	queue* q = createQueue(sizeof(int));
	int tmp = 12;
	int ret_value = 5;
	if (q == NULL){
		printf("Failed to create queue");
		return 1;
	}
	enqueue(q,&tmp);
	front(q,&ret_value);
	dprint("Value retrieved: %d\n",ret_value);

	int tmp2 = 25;
	enqueue(q,&tmp2);
	dequeue(q,&ret_value);
	dprint("Value dequeued: %d\n",ret_value);
	dprint("Length of queue: %ld\n", getSize(q));
	front(q,&ret_value);
	dprint("Peek value: %d\n",ret_value);
	TEST_ASSERTION(ret_value,25)
	destroyQueue(&q);
}
END_TESTS;

