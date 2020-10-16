#include <iostream>
#include"poli.hh"
using namespace std;
//constructor
    polynomials::polynomials(double d, unsigned int n){
        polynomialarr = new term[1];
        term * t = new term;
        t->coefficient = d;
        t->exponent = n;
        polynomialarr[0] = t;
        size = 1;
    };
    //copy constructor 
    polynomials::polynomials(const polynomials &obj ){
        polynomialarr = new term*[obj.size];
        for(int i = 0 ; i < obj.size; i++){
            term *t = new term;
                t->coefficient = obj.polynomialarr[i]->coefficient;
                t->exponent = obj.polynomialarr[i]->exponent;
            polynomialarr[i] = t;
        }
        size = obj.size;
    };
    //destructor
    polynomials::~polynomials(){
       for(int i = 0 ; i < size; i++){
           delete(polynomialarr[i]);
       }
       delete polynomialarr;
    };
   polynomials::term operator+ (term a){
       //what did you want adding terms of adding hole polinomials ??
       return a;
   }
    polynomials::term operator* (term a){
        return a;
    }
    //polynomiterm evaluate(double x);

