#include <iostream> 
#include "list_builder.hh"
using namespace std;
class fixed_list {
    list_builder list;
    fixed_list(list_builder n){
        list = n;
    };
    ~fixed_list(){

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
    fixed_list operator[] (fixed_list a)
    {

    };
    friend ostream& operator << (ostream& os, fixed_list rat)
    {

        return os;
    }

};
// (4 points) A constructor that builds a fixed_list from a list_builder object.
// (2 points) A destructor that cleans up all memory used by the object.
// (2 points) Method Size(), that returns the size of the list.
// (3 points) Overloaded operator [] for reading (not modifying) list elements, with bounds checking.
// (2 points) The appropriate declarations to disable assignments or copies of a fixed_list object.
// (5 points) Overloaded stream output so that a fixed_list object can be placed onto an output stream.