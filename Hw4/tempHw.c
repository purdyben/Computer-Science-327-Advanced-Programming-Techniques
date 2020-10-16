#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void show_array(unsigned* A);
unsigned* build_common(unsigned* A, unsigned* B);
unsigned* primeFactors(unsigned);
unsigned* build_factors(unsigned);

//========================================================================

//structs from linkedlist.h
typedef struct node {
  unsigned int num;
    struct node * next;
} Node;

typedef struct list {
    Node * head; 
} List;

//========================================================================
//linkedlist headers 
int addNode(List *, Node *);
int add_duplicates(List *, Node *);
int hasNext(Node *);
int removeNode(Node *, Node *);

//========================================================================

int main(){
int M;
int N;
printf("Enter two integers M, N:\n");
scanf("%d, %d", &M,&N);
printf("%d, %d\nls", M,N);

unsigned* Marr = build_factors(M);
unsigned* Narr = build_factors(N);

printf("%d has factors:", M);
show_array(Marr);
printf("%d has factors:", N);
show_array(Narr);

unsigned* common = build_common(Marr,Narr);
printf("%d and %d has common factors:", M,N);
show_array(common);


};
void show_array(unsigned* arr){
    printf("{");
    int i;
    for (i = 0; *(arr+i)!= 0; ++i){
        
        if(i != 0 && *(arr+(i))!= 0){
            printf(",");
        }
        printf("%d ", *(arr + i));

    };
    printf("}\n");
}
unsigned* build_common(unsigned* A, unsigned* B){
    List *list = malloc(sizeof(List));

    unsigned int i = 0;
    unsigned int j = 0;


    while(*(A+(i)) != 0 )
    {
        j= 0;
          while(*(B+(j)) != 0 )
          {
            if(*(A+(i)) == *(B+(j)))
            {
                Node *newNode = malloc(sizeof(Node));
                newNode->num = *(A+i);
                add_duplicates(list,newNode);
            }
              j++;
          }
          
        i++;
    }
    //===== get array size;
    int size = 0;
    Node *current = list->head;
    while(current != NULL)
    {
        size++;
        current = current->next;
       
    }

    unsigned int *arr;
    if(size > 0 ){
        
        arr = (unsigned int*)malloc(size * sizeof(int));
    };

    current = list->head;
    
    for(int i = 0 ; current != NULL ; i++)
    {   
       *(arr+i) = current->num;
        current = current->next;

    }
    
    // if(arr != NULL){
    //     i = 0;
    //     while(*(arr+i) != 0){
    //         i++;
    //     }
        
    // }
    return arr;
}
unsigned* build_factors(unsigned n){
    return primeFactors(n);
}
unsigned* primeFactors(unsigned n){
   unsigned* arr = malloc(100 * sizeof(unsigned));
    int index = 0;
    while (n % 2 == 0)  
    {  
        //printf("two :%d, n : %d, index : %d \n", 2, n, index);
        *(arr+index) = 2;
        index++;
        n = n/2;  
    }  
  
  for (int i = 3; i <= sqrt((double)n); i = i + 2)  
    {  
        // While i divides n, print i and divide n  
        while (n % i == 0)  
        {  
            //printf("for : %d, n : %d, index : %d\n", i, n,index);
           *(arr+index) = i;
            index++;
            n = n/i;  
        }  
    }  
    if (n > 2){
        //printf("% case >2:  d,  n : %d, index : %d\n", 2, n, index);
        *(arr+index) = n;
         index++;
    }  
     *(arr+index) = 0;

    return arr;
}


//========================================================================
//linked list methods
int addNode(List * list, Node *node){
  if( list -> head == NULL){
    list -> head = node;
  }else{
    Node *current = list -> head;
    while(hasNext(current)){
      current = current -> next;
    }
    current -> next = node;

  };
  return 1;
};
int hasNext(Node *node){
    //printf("%d", node->data);
    if(node == NULL){
      return 0;
    };
    if(node -> next != NULL){
      return 1;
    } else {
      return 0;
    }
    
};
int removeNode(Node *parentNode , Node *node){
   parentNode -> next = NULL;
   return 1;
};
int add_duplicates(List * list, Node *node){
  
  
  if( list -> head == NULL){
    list -> head = node;
  
  };
  Node *current = list->head;
  while(current != NULL){

    
    if(current->num == node->num){
      return 0;
    };
    current = current->next;
  };
  
  addNode(list,node);
  return 1;
};
