// For the remaining two questions, you will build a class to represent polynomials.  Your class must contain the following:

// An inner struct named term for a single term of the polynomial, which contains a coefficient as a double and an exponent as an unsigned.
// A dynamically-sized array of terms, in decreasing order of exponent. The array cannot contain terms with a coefficient of zero, and cannot contain two or more terms with the same exponent.
// A constructor, which takes a coefficient and an exponent, for building a polynomial containing a single term.  If the coefficient is zero, then it should build the polynomial of no terms (equivalent to 0).
// A copy constructor.
// A destructor.
// An overloaded assignment operator.
// An overloaded addition operator.
// An overloaded multiplication operator.
// A method named evaluate, that takes a value for x (as a double) and determines the value of the polynomial for that value of x.
// Overloaded stream output for displaying a polynomial.
// Any additional helper methods or constructors needed for implementation.
// As an example, a polynomial containing terms
#include <iostream>
using namespace std;
class polynomials
{
    public:
   typedef struct term
    {
       double coefficient;
       unsigned int exponent;
    } term;
    term **polynomialarr;
     int size;


    polynomials(double d, unsigned int n);
    polynomials(const polynomials &obj );
    ~polynomials();
    polynomials operator= (polynomials a){

    }
    term operator+ (term a);
    term operator* (term a);
    int evaluate(double x){
        //what do you mean determines the value of the polynomial for that value of x.
        return x;
    }
    friend ostream& operator<<(ostream& os, polynomials poli){
          for(int i = 0 ; i < poli.size; i++){
              if(i != 0){
                  if (poli.polynomialarr[i]->exponent > 0){
                      cout<<"+";
                  }
              }
              cout << poli.polynomialarr[i]->coefficient<< "x^"<< poli.polynomialarr[i]->exponent;
             
          }
    }

};