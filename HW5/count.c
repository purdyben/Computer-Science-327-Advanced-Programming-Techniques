
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>



typedef struct node
{
    unsigned count;
    struct node *next_p;
    char *string;

} node_T;

typedef struct hashtable 
{
    unsigned size;
    node_T **node_array;

} hashtable; 

void init_table(struct hashtable*, unsigned);
void add_to_table(struct hashtable*, char*);
void show_table(struct hashtable*);
unsigned long hash(char *);
void turnLower(char *);


int main()
{   
    int size = 0;
    scanf("%d", &size);
    hashtable *hashy = malloc(sizeof(hashtable));
    printf("(1) hashy is %p\n", hashy);

    //init hashatable
    init_table(hashy, size);
    printf("this is my size in hash %d\n", hashy->size);

    // input
    char str[33];
    printf("input strings\n");
    while(!feof(stdin)){
        scanf("%s", str);
        turnLower(str);
        //addto hashtable
        printf("(2) hashy is %p\n", hashy);
        add_to_table(hashy, str);

    }
    show_table(hashy);

    //addind to table. 
    //add_to_table(hashy, &str);
    
    // tolower is per char

    // char *str = NULL;
    // scanf("input a string %s \n", str);
    // //add_to_table(hashy ,str);

}

void init_table(struct hashtable* T, unsigned size)
{
    printf("init_table\n");
    T->size = size;
    T->node_array = malloc(sizeof(node_T*) * size);
    for(int i = 0 ; i < size ; i++){
       T->node_array[i] = NULL;
    };

    // printf("initialization of hashtable \n"); 
    // for(int i = 0 ; i < size ; ++i){
    //     printf("%p\n", ((T->node_array)+i));
    // };
};
void add_to_table(struct hashtable* T, char* str)
{
    printf("add_to_table\n");
    printf("%p\n" , T);
    // if(T == NULL){
    //     printf("%p\n",T);

    // }
    
    // tests { 
    // @ T->size segfaults this code. how @
     printf("this is my size %d\n",T->size);
    //printf("this is add to table %p \n", T->node_array + 1);
    //}
    printf("searching threw hashy");
    for(int i = 0 ; i < T->size; i++)
    {
        if(((T->node_array) + i) == 0){
            printf("at A[%d] has not been initialized \n", i);
        }else
        {
            printf("surching hashy at : A[%d]\n", i );
            
            // Goes threw the the array node_array at i and if found 
            // increase counter by one before returning
            node_T *current = (T->node_array[i]);
            
            while(current != NULL)
            {
                printf("seaching for str match \n");

                if(strcmp(current->string, str) == 0){
                    current->count++; 
                    return;
                };
                current = current->next_p;  
            };
            //printf("nothing found in hashy at A[%d]\n", i );
        };
    };
    
    printf("nothing found in hashy\n");

   // string str was not found in node array add string to the hash code
    int indecy = hash(str)%(T->size);
    node_T *newNode = malloc(sizeof(node_T));
    newNode->count = 1;
    char *star = strdup(str); // copys str to newNode->string
    newNode->string = star;
    //holds the address of node_array head in temp_node
    //sets the node_array[indecy] to the new node
    //the attaches the addres of the old head to the new node. 
    //the address of temp_node might be lost in scoop
    node_T *temp_node = ((T->node_array[indecy]));
    T->node_array[indecy] = newNode;
    newNode->next_p = temp_node;
    
    //  node_T* temp_address = arrayPosition;
    // new_node->next_p = arrayPosition;



};
void show_table(struct hashtable* T)
{
    printf("show_table\n");

    if( (T->node_array) == 0){
        printf("ERR - hashtable has not been initialized\n");
        return; 
    }
    for(int i = 0 ; i < T->size; ++i)
    {   
        if((T->node_array) + i == NULL){
            // issue indecy has not been initialized
        }else{
            node_T *current = (T->node_array[i]);
            while(current != NULL){
                printf("%d %s\n", current->count, current->string);
                current = current->next_p;
            };
        };
    };
     printf("this is the end after reaturns");
};
unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c = 0;

    while (c == *str++)
        hash = ((hash << 5) + hash) + c; 

    return hash;
};

// make str all lower case.
void turnLower(char *str){
    for(int i = 0; str[i]; i++){
       str[i] = tolower(str[i]);
    }
    printf("\n%s\n", str);
};

