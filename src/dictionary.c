#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "commons.h"
#include "dictionary.h"

// Create a new entry
entry *createEntry(void *key,size_t key_size,void *data,size_t data_size){
    entry *new = malloc(sizeof(entry));
    new->next = NULL;
    new->value = malloc(data_size);
    new->key = malloc(key_size);
    new->v_size = data_size;
    memcpy(new->value,data,data_size);
    memcpy(new->key,key,key_size);

    return new;
}

// Destroy an entry
void destroyEntry(entry *en){
    free(en->key);
    free(en->value);
    free(en);
}

bool string_compare(entry *d1,void *d2 ){
    char *data1 = (char*)(d1->key);
    char *data2 = (char*)d2;
    //printf("Comparation in progress\n");
    return strcmp(data1,data2) == 0;
}

unsigned int hash_string(void *data){
    unsigned hashval;
    char *s = (char*) data;
    for (hashval = 0; *s != '\0'; s++)
      hashval = *s + 31 * hashval;
    return hashval ;
}

int extend_h_table(hashTable *h)
{
    if (h->size >= h->capacity){
        size_t new_capacity = h->capacity * 2;
        entry **newdata = calloc(new_capacity,sizeof(entry*));
        for (int i = 0;i < new_capacity ; ++i)
            newdata[i] = NULL;
        if (newdata == NULL)
            return 1;
        entry *d;
        unsigned int new_i;
        unsigned int h_val;
        for (int i = 0; i<h->size; ++i){
            d = h->entries[i];
            if (d != NULL){
                for (entry *curr = d; curr!=NULL; curr = curr->next){
                    h_val = h->hash(curr->key);
                    new_i = h_val % new_capacity;
                    if (newdata[new_i] != NULL)
                        curr->next = newdata[new_i];
                    newdata[new_i] = curr;
                }
            }
        }
        free(h->entries);
        h->entries = newdata;
        h->capacity = new_capacity;
    }
    return 0;
}


int get_h_table(hashTable *h,void* key,void* output)
{
    unsigned int val = h->hash(key);
    unsigned int i = val  % h->capacity;
    for(entry *curr = h->entries[i];curr != NULL;curr = curr->next){
        if (curr == NULL)
            break;
        if (h->compare(curr,key)){
            memcpy(output,curr->value,curr->v_size);
            return 0;
        }
    }
    return 1;
}

int set_h_table(hashTable *h,void *key,void *value,size_t v_size)
{
    //printf("Extending hashtable if necessary \n");
    int res = h->extend(h);
    if (res != 0)
        return res;
    unsigned int h_val = h->hash(key);
    unsigned int i = h_val  % h->capacity;
    //printf("New hashmap position %d\n",i);
    bool inserted = false;
    //printf("Setting new value of size %lu\n",v_size);
    for(entry *curr = h->entries[i];curr != NULL || inserted; curr = curr->next){
        if (curr == NULL)
            break;
        if (h->compare(curr,key)){
            entry *temp = createEntry(key,h->k_size,value,v_size);
            destroyEntry(curr);
            curr = temp;
            inserted = true;
        }
    }

    if(! inserted){
        //printf("Creating new head\n");
        entry *temp = createEntry(key,h->k_size,value,v_size);
        if (h->entries[i] == NULL)
            h->entries[i] = temp;
        else{
            entry *head = h->entries[i];
            temp->next = head;
            h->entries[i] = temp;
        }
    }
    h->size++;
    return 0;
}

int remove_h_table(hashTable *h,void *key,void* output){

    unsigned int val = h->hash(key);
    unsigned int i_val = val % h->capacity;
    entry *pre ;
    int i = 0;
    entry *temp;
    for(entry *curr = h->entries[i_val];curr != NULL;curr = curr->next){
        if (curr == NULL)
            break;
        if (i == 0  && h->compare(curr,key)){
            temp = curr;
            h->entries[i_val] = curr->next;
            memcpy(output,curr->value,curr->v_size);
            destroyEntry(temp);
        }
        else if (h->compare(curr,key)){
            temp = curr;
            pre->next = curr->next;
            memcpy(output,curr->value,curr->v_size);
            destroyEntry(temp);
        }
        i++; 
        pre = curr;
    }
    return 0;
}


bool int_compare(entry *d1,void *d2 ){
    int i1 = *(int*)(d1->value);
    int i2 = *(int*)d2;
    return i1 == i2;
}

unsigned int hash_int(void *data){
    unsigned int s = *(unsigned int*) data;
    return s;
}

hashTable *create_hashTable(enum dict_type t,size_t allocSize,bool (*compare_func)(entry*,void*),unsigned int (*hash_func)(void*))
{
    size_t k_size;
    unsigned int (*hash)(void*);
    bool (*compare)(entry*,void* key);
    switch(t){
        case INT:
            k_size = sizeof(int);
            hash = &hash_int;
            compare = &int_compare;
            break;
        case CHAR:
            k_size = MAX_CHAR_SIZE;
            hash = &hash_string;
            compare = &string_compare;
            break;
        case UNDEFINED:
            k_size = allocSize;
            hash = hash_func;
            compare = compare_func;
            break;
        default:
            return NULL;
    }
    ////printf("Creating hashTable\n");
    hashTable *h = malloc(sizeof(hashTable));
    h->k_size = k_size;
    h->size = 0;
    h->capacity = 1;
    h->extend = &extend_h_table;
    h->get = &get_h_table;
    h->set = &set_h_table;
    h->hash = hash;
    h->compare = compare;
    h->remove = &remove_h_table;
    h->entries = malloc(sizeof(entry*));
    h->entries[0] = NULL;
    return h;
}
        
