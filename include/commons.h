#ifndef __COMMONS_H
#define __COMMONS_H
#include <assert.h>
typedef struct Node
{
    void *data;
    struct Node *next;
} node;

typedef enum { STR_TYPE, INT_TYPE , U_INT_TYPE } KeyType;

typedef union{
    char *str;
    int num_int;
    size_t u_num_int;
} acc_key_types;

typedef struct Cell {
    void *data;
    struct Cell *next;
    acc_key_types key;
    KeyType k_type;
}cell;


node *createNode(void *data, size_t allocSize);
cell *createCell(void *data, size_t allocSize,KeyType k_type,acc_key_types key);

#define acquire_lock(lock)\
do { \
    assert( 0 == pthread_mutex_lock(lock));\
}while (0)

#define release_lock(lock)\
do { \
    assert( 0 == pthread_mutex_unlock(lock));\
}while (0)

#define condition_wait(cond,lock)\
do { \
    assert( 0 == pthread_cond_wait(cond,lock));\
}while (0)

#define condition_signal(cond)\
do { \
    assert( 0 == pthread_cond_signal(cond));\
}while (0)
#endif
