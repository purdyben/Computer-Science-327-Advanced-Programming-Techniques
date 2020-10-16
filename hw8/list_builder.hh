#include <iostream>
using namespace std;

typedef struct node{
    int data;
    struct node *next;
} node;

class list_builder {
    public : 
    node *head;
    int size;
    public: 
    
    // list_builder::constructor(node) 
//      add_to_front(int value), which adds an element to the front of the list.
// (5 points) Method void add_to_back(int value), which adds an element to the back of the list.
// (5 points) Method void reset(), which removes all elements from the list.
    list_builder(node n){
        head = &n;
        size = 1;
    }
    ~list_builder(){
        reset();
    }
    void add_to_front(int value){
        node *tempn = new node();
        tempn->data = value;
        tempn->next = head;
        head = tempn;
        size++;

    };
    void add_to_back(int value){
        node *curr = head;
        while(curr->next != NULL){
            curr = curr->next;
        }
        node *tempn = new node();
        tempn->data = value;
        curr->next = tempn;
    };
    void reset(){
        node *curr = head;
        while(curr != NULL){
            head = curr->next;
            delete(curr);
            curr = head;
        }

    };


};