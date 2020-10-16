#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct card{
    int faceDown;
    char rank;
    char suit;
} Card;

typedef struct moveData
{
   char dot; 
   char src;
   char dest;
} moveData;


typedef struct node {
    Card * card;
    moveData * moveData;
    struct node * next;
    
} Node;

typedef struct list {
    Node * head; 
    Node * tail;
} List;


int addNode(List *, Node *);
int hasNext(Node *);
int removeNode(List *, Node *);
void Print(List *);
void resetTail(List *);

#endif