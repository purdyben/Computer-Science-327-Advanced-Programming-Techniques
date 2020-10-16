#include <iostream>
#include "intbox.hh"
void singleton::show(std::ostream &s){
     s << value;
}
bool singleton::contains(int v){
    if(value == v){
        return true;
    }
    return false;
}
bool interval::contains(int a){
    if( a >= low && a <= high){
        return true;
    }
    return false;
}
void interval::show(std::ostream &s){
    s<<"["<<low<<","<<high<<"]";
};

void collection::set_item(unsigned i, intbox* item){
    if(i < size){
        arr[i] = item;
    }
}
bool collection::contains(int a){
    for(unsigned i = 0 ; i < (unsigned)size ; i++) {
        if(arr[i]->contains(a)){
            return true;
        }
    }
    return false;
}
void collection::show(std::ostream &s){
    s<<"{";
    for(unsigned i = 0 ; i < size ; i++){
        if(i == size-1){
            arr[i]->show(s) ;
        }else{
                arr[i]->show(s); 
                s<< ",";
        }
    }
    s<<"}";
};