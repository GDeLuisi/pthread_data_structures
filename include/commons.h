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
