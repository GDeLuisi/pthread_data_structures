#define THREADSAFE
#include <stdio.h>
#include "dictionary.h"
#include <pthread.h>
#include <stdlib.h>
#include "pthread_groups.h"
#include "queue.h"
#include <unistd.h>
#define TEST_SUITE_ON 
#define DEBUG
#include "test_framework.h"
#include "debug.h"

void* test_threading_queue_consumer(void** arg){
	queue * q = (queue*)(arg);
	int ret_val;
    printf("Consumer Queue Size: %lu\n",getSize(q));
	printf("Trying to pull from queue\n");
	dequeue(q,&ret_val);
	printf("Pulled %d from queue\n",ret_val);
	return &ret_val;
}

void* test_threading_queue_producer(void** arg){
	queue * q = (queue*)(arg);
	int push_val = 10;
	printf("Trying to push on queue\n");
//	sleep(3);
	enqueue(q,&push_val);
	printf("Pushed %d to queue\n",push_val);
	push_val = 7;
	printf("Pushing %d to queue\n",push_val);
	enqueue(q,&push_val);
	printf("Pushed %d to queue\n",push_val);
	return &push_val;
}

void* test_func(void** arg){
    hashTable *h = (hashTable*)(arg[0]);
    int a = 35;
    char mykey[] = "giovanni";
    printf("Setting giovanni:35 on hashmap \n");
    h->set(h,mykey,&a,sizeof(int));
	int * val_arg = (int*)(arg[1]);
	pthread_t self_id = pthread_self();
	sleep(1);
	printf("I'm executed inside the %lu thread\n",self_id);
	printf("Received this arg %d\n",*val_arg);
	return val_arg;
}

void* test_func2(void** arg){
	int * val_arg = (int*)(arg[1]);
    int *a = malloc(sizeof(int));
    char mykey[] = "giovanni";
    hashTable *h = (hashTable*)(arg[0]);
	pthread_t self_id = pthread_self();
	sleep(3);

    printf("Getting  giovanni:35 on hashmap \n");
    h->get(h,mykey,a);
	printf("I'm executed inside the %lu thread, and i am thread number 2\n",self_id);
    printf("Found this in hashmap %d\n",*a);
	printf("Received this arg in tg 2 %d\n",*val_arg);
	return val_arg;
}

void* test_func3(void** arg){
	int * val_arg = (int*)(arg[1]);
	pthread_t self_id = pthread_self();
	sleep(3);
	printf("I'm executed inside the %lu thread, and i am thread number 3\n",self_id);
	printf("Received this arg %d\n",*val_arg);
	return val_arg;
}
BEGIN_TESTS;
void* outputs[3];
int i;
TEST("create_entry"){
    char ck[] = "chiave";
    int cval = 12;
    entry* e = createEntry(ck,sizeof(ck),&cval,sizeof(int));
    printf("created entry\n");
    int *res = malloc(sizeof(int));
    printf("Entry Resp: %d\n",*(int*)(e->value));
    TEST_ASSERTION(*(int*)(e->value),12)
}
TEST("dictionary_creation"){
    
    hashTable *h = create_hashTable(CHAR,0,NULL,NULL);
   
    char mykey[] = "chiave";
    char myvalue[] = "valore";
    printf("Inserting  %s:%s\n",mykey,myvalue);
    h->set(h,mykey,myvalue,sizeof(myvalue));
    strcpy(mykey,"chiav");
    strcpy(myvalue,"valor");
    printf("Inserting  %s:%s\n",mykey,myvalue);
    h->set(h,mykey,myvalue,sizeof(myvalue));
    strcpy(mykey,"chia");
    int myvalint = 12;
    printf("Inserting  %s:%d\n",mykey,myvalint);
    h->set(h,mykey,&myvalint,sizeof(int));
    int* resp_h_int = malloc(sizeof(int)); 
    printf("Searching this integer\n");
    h->get(h,mykey,resp_h_int);
    printf("Found this integer: %d\n",*resp_h_int);
    TEST_ASSERTION(*resp_h_int,12)
}
TEST("dict_usage"){
    hashTable *h1 = create_hashTable(CHAR,0,NULL,NULL);
    hashTable *h2 = create_hashTable(CHAR,0,NULL,NULL);
    char mykey2[] = "chiave";
    char myvalue2[] = "valore";
    char myhashtkey[] = "hashtable";
    printf("Inserting  %s:%s\n",mykey2,myvalue2);
    h1->set(h1,mykey2,myvalue2,sizeof(myvalue2));
    h2->set(h2,mykey2,myvalue2,sizeof(myvalue2));
    strcpy(mykey2,"chiav");
    strcpy(myvalue2,"valor");
    printf("Inserting  %s:%s\n",mykey2,myvalue2);
    h1->set(h1,mykey2,myvalue2,sizeof(myvalue2));
    h2->set(h2,mykey2,myvalue2,sizeof(myvalue2));
    strcpy(mykey2,"chia");
    int myvalint2 = 12;
    printf("Inserting  %s:%d\n",mykey2,myvalint2);
    h1->set(h1,mykey2,&myvalint2,sizeof(int));
    h1->set(h1,myhashtkey,h2,sizeof(hashTable));
    int* resp_h_int2 = malloc(sizeof(int)); 
    char* resp_h_char2 = malloc(250);
    strcpy(mykey2,"chiav");
    printf("Searching this string\n");
    h1->get(h1,mykey2,resp_h_char2);

    hashTable *h3 = malloc(sizeof(hashTable));
    printf("Fetching nested  map\n");
    h1->get(h1,myhashtkey,h3); 
    printf("%p == %p\n",h2,h3);
    strcpy(mykey2,"chiave");
    h3->get(h3,mykey2,resp_h_char2);
    printf("Found this string from nested map: %s\n",resp_h_char2);
    strcpy(mykey2,"chia");
    printf("Searching this integer\n");
    h1->remove(h1,mykey2,resp_h_int2);
    printf("Found this string: %s\n",resp_h_char2);
    printf("Found this integer: %d\n",*resp_h_int2);
    printf("Allocating pointer to nested map\n");

    TEST_ASSERTION(*resp_h_int2,12)
}
TEST("threadgroup_creation"){
	target t_targets[3];
	int* my_args[3] = {(int*)11,(int*)24,(int*)63};
	void ** my_target_args = (void**) my_args;
	t_targets[0] = &test_func;
	t_targets[1] = &test_func2;
	t_targets[2] = &test_func3;
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

    //int batch_tmp_en[] = {1,2,3,4,5,6,7,8,9,10};
    int *batch_tmp_en = malloc(sizeof(int)*10); 
    int *batch_tmp_de = malloc(sizeof(int)*7);
    for (int i =0;i<10;++i){
        batch_tmp_en[i] = i+1;
    }
    //printf("Size of arr: %lu\n",sizeof(batch_tmp_en));
    printf("Creating batch enqueue\n");
    batchEnqueue(q,batch_tmp_en,10);
    //for (int i=0;i<10;i++)
     //   enqueue(q,batch_tmp_en+i);
    printf("Size after batch enqueue %lu\n",getSize(q));
	dequeue(q,&ret_value);
	dequeue(q,&ret_value);
	dequeue(q,&ret_value);
    printf("Value dequeued: %d\n",ret_value);
    printf("Batch Extraction\n");
    batchDequeue(q,batch_tmp_de,7);
    for (int i=0;i<7;i++)
        printf("Found after batch dequeue at pos %d: %d\n",i,batch_tmp_de[i]);
	destroyQueue(&q);
}
TEST("threaded_queue"){
	queue* qt = createQueue(sizeof(int));

	pthread_t cons_thread;
	pthread_t prod_thread;
	int res;

	res = pthread_create(&cons_thread,NULL,&test_threading_queue_consumer,qt);
	if(res != 0){
		printf("Thread consumer creation returned an error");
		return res;
	}
	res = pthread_create(&prod_thread,NULL,&test_threading_queue_producer,qt);
	if(res != 0){
		printf("Thread producer creation returned an error");
		return res;
	}
//	sleep(10);
	pthread_join(cons_thread,NULL);	
	pthread_join(prod_thread,NULL);	
  front(qt,&res);
	printf("Received %d from front\n",res);
  destroyQueue(&qt);
	TEST_ASSERTION(7,res)
}
END_TESTS;

