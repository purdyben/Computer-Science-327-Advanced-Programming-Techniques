
#include <iostream>
#include "rational.hh"
using namespace std;

  rational::rational(long num, long dnom)
      {    
        if(dnom < 0){
          denom = (-1)*dnom;
          numer = ((-1)*num); 
          
        }else{
          numer = num; 
          denom = dnom;  
        }
      
        if(numer % denom == 0){
          numer = numer /denom;
          denom = 1;
        }else{
          long count = 1;
          while(count < numer && count < denom){
            
            if(numer % count == 0 && denom % count == 0){
              numer = numer /count;
              denom = denom / count;
              count = 1;
            };
            count++;
          };
        }
      };
      rational::rational(long num)
      {    
        numer = num; 
        denom = 1;
      };
      rational::rational()
      {    
        numer = 0; 
        denom = 0;
      };

//   class rational {
    
//       public:
//       long numer, denom;
//       rational(long num, long dnom)
//       {    
//         if(dnom < 0){
//           denom = (-1)*dnom;
//           numer = ((-1)*num); 
          
//         }else{
//           numer = num; 
//           denom = dnom;  
//         }
      
//         if(numer % denom == 0){
//           numer = numer /denom;
//           denom = 1;
//         }else{
//           long count = 1;
//           while(count < numer && count < denom){
            
//             if(numer % count == 0 && denom % count == 0){
//               numer = numer /count;
//               denom = denom / count;
//               count = 1;
//             };
//             count++;
//           };
//         }
//       };
//       rational(long num)
//       {    
//         numer = num; 
//         denom = 1;
//       };
//       rational()
//       {    
//         numer = 0; 
//         denom = 0;
//       };

//     rational operator+ (rational a)
//     {
//         rational res(numer*a.denom + a.numer*denom, a.denom * denom);
//         return res;

//     };
//     rational operator- (rational a)
//     {
//       rational res(numer*a.denom - a.numer*denom, a.denom * denom);
//       return res;

//     };
//     rational operator* (rational a)
//     {
//       rational res(numer*a.denom * a.numer*denom, a.denom * denom);
//       return res;

//     };
//     rational operator/ (rational a)
//     {
//       rational res(numer*a.denom / a.numer*denom, a.denom * denom);
//       return res;

//     };
      
//         friend ostream& operator<<(ostream& os, const rational rat);
// };

// ostream& operator<<(ostream& os, rational rat)
// {
//     if(rat.denom == 1){
//       os << rat.numer;

//     }else{
//       os << rat.numer << '/' << rat.denom;

//     }
//     return os;
// }

// int main () {
//   cout<<"Hellow world\n";
//   rational a((long)4, (long)3);
//   cout<< a.numer << " "<< a.denom;
//     cout<<"\n";
//   rational b((long)7, (long)6);
//   cout<< b.numer << " "<< b.denom;
//     cout<<"\n";
//   rational c((long)-12, (long)2);
//   cout << c.numer << " "<< c.denom;
//   cout<<"\n";
//   rational d = (a+b);
//   cout << d.numer << " "<< d.denom;
//   cout<<"\n";
//   cout<< d;
//   return 0;
// };


