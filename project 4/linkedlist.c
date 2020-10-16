#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

int addNode(List * list, Node *node){
  if( list -> head == NULL){
    list -> head = node;
    list->tail = node;
  }else{
    Node *current = list -> head;
    while(current->next != NULL){
      current = current -> next;
    }
    current -> next = node;
  };
  
  list->tail = node;
  return 1;
};
void resetTail(List *list){
  if(list->head == NULL){
    list->tail = NULL;
  }else{
    Node * current = list->head;
    while(current->next != NULL){
      current = current->next;
    }
    list->tail = current;
    list->tail->next = NULL;
  }
}

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
int removeNode(List * list, Node *node){
    // parentNode -> next = NULL;
    // parentNode ->next = node -> next;
    // free(node);
  
   if(list->head == node){
      (list->head) = (list->head->next);
    }else if( list->tail == node){
      Node *curr = list->head;
      while(curr->next != node){
        curr= curr->next;
      } 
      list->tail = curr;
    }

   return 1;
};

void Print(List * list) {
  Node *current = list ->head;
  while(hasNext(current)){
    Card *card = current -> card;
    printf("current data: %c%c\n",card ->rank, card->suit); 
    current = current -> next;
  };
    Card *card = current -> card;
    printf("current data: %c%c\n",card->rank, card->suit); 
  
}

Node Iterate_to(List *list , int num){
  Node *current = list ->head;
  for(int i = 0 ; i< num; ++i){
    current = current -> next;
  };
  return *current; 
};
