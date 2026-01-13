#ifndef __DICT_H
#define __DICT_H
#include "commons.h"
#define MAX_CHAR_SIZE 255;

typedef struct Entry {
    size_t v_size;
    void *value;
    void *key;
    struct Entry *next;
}entry;

entry *createEntry(void *key,size_t key_size, void *data,size_t data_size);
void destroyEntry(entry *en);

typedef struct HashTable{
    size_t k_size;
    entry **entries;
    size_t size;
    size_t capacity;
    int (*extend)(struct HashTable*);
    int (*get)(struct HashTable*,void* key,void* output);
    int (*set)(struct HashTable*,void* key, void* value,size_t v_size);
    unsigned int (*hash)(void*);
    int (*remove)(struct HashTable*,void* key,void* output);
    bool (*compare)(entry*,void* key);
} hashTable;

enum dict_type{
    INT,
    CHAR,
    UNDEFINED
};

hashTable *create_hashTable(enum dict_type t,size_t allocSize,bool (*compare_func)(entry*,void*),unsigned int (*hash_func)(void*));
#endif 
