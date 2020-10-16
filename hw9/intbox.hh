#include <iostream>
//using namespace std;
class intbox{
    public:
    virtual bool contains(int a) = 0;
    virtual void show(std::ostream &s);
};
class singleton: public intbox{
    int value; 
    public:
    singleton(int v){
        value = v;
    };
    
    bool contains(int v);
    void show(std::ostream &s);

};
class interval : public intbox{
    int low;
    int high;
    public:
    interval(int l, int h){
        if(l > h){
            std::cout<<"Error low is greater then high";
            exit(0);
        }else{
            low = l;
            high = h;
        }
    }
    bool contains(int a);
    void show(std::ostream &s);
};
class collection : public  intbox{
    unsigned size;
    intbox **arr;
    public:
    collection(unsigned s){
        size = s;
        arr = new intbox*[s]; 
    }
    ~collection(){
        for(unsigned i = 0 ; i < size ; i++) {
            delete (arr[i]);
        }
        delete[] arr;
    }
    void set_item(unsigned i, intbox* item);
    bool contains(int a);
    void show(std::ostream &s);
};
