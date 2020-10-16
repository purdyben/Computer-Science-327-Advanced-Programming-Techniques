


typedef struct node {
  unsigned int num;
    struct node * next;
} Node;

typedef struct list {
    Node * head; 
} List;

int addNode(List *, Node *);
int add_duplicates(List *, Node *);
int hasNext(Node *);
int removeNode(Node *, Node *);
