#include <string.h>
#include <stdlib.h>
#include "commons.h"

node *createNode(void *data, size_t allocSize)
{
    node *toInsert = (node *)malloc(sizeof(node));
    if(toInsert == NULL)
    {
    return NULL;
    }


    toInsert->data = malloc(allocSize);
    if(toInsert->data == NULL)
    {
    free(toInsert);
    return NULL;
    }

    memcpy(toInsert->data, data, allocSize);
    toInsert->next = NULL;

    return toInsert;
}


cell *createCell(void *data, size_t allocSize,KeyType k_type,acc_key_types key){
    
    cell *cl = (cell *)malloc(sizeof(cell));

    switch(k_type){
        case STR_TYPE:
            cl->key.str = malloc(strlen(key.str) + 1);
            strcpy(cl->key.str,key.str);
            break;
        case INT_TYPE:
            cl->key.num_int = key.num_int;
            break;
        case U_INT_TYPE:
            cl->key.u_num_int = key.u_num_int;
            break;
        default:
            return NULL;
    }
    cl->k_type = k_type;

    if(cl == NULL)
    {
    return NULL;
    }

    cl->data = malloc(allocSize);
    if(cl->data == NULL)
    {
        free(cl);
        return NULL;
    }

    memcpy(cl->data, data, allocSize);
    cl->next = NULL;

    return cl;
}


