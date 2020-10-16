#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"


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
