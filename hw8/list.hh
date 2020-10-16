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
    
// list_builder::constructor(node) 
//      add_to_front(int value), which adds an element to the front of the list.
// (5 points) Method void add_to_back(int value), which adds an element to the back of the list.
// (5 points) Method void reset(), which removes all elements from the list.

    list_builder(){
        head = NULL;
        size = 0;
    }
    list_builder(node n){
        head = &n;
        size = 1;
    }
    ~list_builder(){
        reset();
        head = NULL;
    }
    void add_to_front(int value){
        if(head != NULL){
            node *tempn = new node();
            tempn->data = value;
            tempn->next = head;
            head = tempn;
        }else{
            node *tempn = new node();
            tempn->data = value;
            head = tempn;
        }
        size++;

    };
    void add_to_back(int value){
        if(head != NULL){
            node *curr = head;
            while(curr->next != NULL){
                curr = curr->next;
            }
            node *tempn = new node();
            tempn->data = value;
            curr->next = tempn;
        }else {
            node *tempn = new node();
            tempn->data = value;
            head = tempn;
        }
        size++;
        //B3 B1 B4 B1 B5 B9 LR
    };
    void reset(){
        head = NULL;
        //this some how segfaults 
        node *prev = NULL;
        node *curr = head;
        while(curr != NULL){
            prev = curr;
            curr = curr->next;
            prev->next = NULL;
            delete prev;
        }
        head = NULL;
        size = 0;   
     };
};

class fixed_list {
    public:
    list_builder list;
    fixed_list(){
        list = *(new list_builder());
    };
    fixed_list(list_builder n){
        list = n;
    };
    ~fixed_list(){
       
        list.~list_builder();


    };
    int Size()
    {
        node *curr = list.head;
        int size;
        while(curr != NULL){
            size++;
            curr = curr->next;
        }
        return size;
    };
    int operator[] (int index)
    {
        if(Size() < index)
        {
            return -1;
        }
        node *curr = list.head;
        int i = 0;
        while(i < index){
            curr = curr->next;
        }

        return curr->data;
    };
    friend ostream& operator << (ostream& os, fixed_list l)
    {        
        cout << "List is now: [";
        node *curr = l.list.head;
        if(curr != NULL){
            cout <<curr->data;
            curr = curr->next;
            while(curr != NULL){
                cout << ", "<< curr->data;
                curr = curr->next;
            }
        }
        cout << "]";    
        return os;
    }
 //int printf();

};
// fixed_list::printl(){
// }