#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "pthread_groups.h"
#include <unistd.h>
#define TEST_SUITE_ON 
#include "test_framework.h"

typedef void* ( *target )( void *);
void* test_func(void* arg){
	int * val_arg = (int*)arg;
	pthread_t self_id = pthread_self();
	printf("I'm executed inside the %lu thread\n",self_id);
	printf("Received this arg %d\n",*val_arg);
	return NULL;
}
BEGIN_TESTS;
TEST("threadgroup_creation"){
	target t_targets[3];
	int* my_args[3] = {(int*)11,(int*)24,(int*)63};
	void ** my_target_args = (void**) my_args;
	t_targets[0] = &test_func;
	t_targets[1] = &test_func;
	t_targets[2] = &test_func;
	struct ThreadGroup* tg = create_thread_group(t_targets,my_target_args,3);
	unsigned int size = tg->size;
	printf("Tg size %u\n",size);
	join_thread_group(tg);
	TEST_ASSERTION(0,0);
}
TEST("failing_test"){
	target t_targets[3];
	int* my_args[3] = {(int*)11,(int*)24,(int*)63};
	void ** my_target_args = (void**) my_args;
	t_targets[0] = &test_func;
	t_targets[1] = &test_func;
	t_targets[2] = &test_func;
	struct ThreadGroup* tg = create_thread_group(t_targets,my_target_args,3);
	unsigned int size = tg->size;
	printf("Tg size %u\n",size);
	join_thread_group(tg);
	TEST_ASSERTION(0,0);
}
END_TESTS;

