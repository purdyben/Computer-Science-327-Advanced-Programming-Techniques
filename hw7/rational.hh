#include <iostream>
using namespace std;
class rational {
    
      public:
      long numer, denom;
      rational(long num, long dnom);
      rational(long num);
      rational();
      rational operator+ (rational a)
      {
        rational res(numer*a.denom + a.numer*denom, a.denom * denom);
        return res;
      };
      rational operator- (rational a)
      {
        rational res(numer*a.denom - a.numer*denom, a.denom * denom);
        return res;

      };
      rational operator* (rational a)
      {
        rational res(numer * a.numer, denom * a.denom);
        return res;

      };
      rational operator/ (rational a)
      {
        rational res = rational(numer, denom)* rational(a.denom, a.numer);
        return res;

      };

    friend ostream& operator<<(ostream& os, rational rat)
{
    if(rat.denom == 1){
      os << rat.numer;

    }else{
      os << rat.numer << '/' << rat.denom;

    }
    return os;
}
};




// #include <iostream>
// using namespace std;
// //#define Debug
// class rational {
   
//     public:
//      long numer, denom;
//     rational(long num, long dnom)
//     {       
//       #ifdef Debug    
//         printf("begining n = %ld d= %ld\n" , num, dnom);
//       #endif
//       if(dnom < 0){
//         #ifdef Debug    
//           printf("dnom < 0\n");
//         #endif
//         denom = (-1)*dnom;
//         numer = ((-1)*num); 
        
//       }else{
//         #ifdef Debug    
//           printf("num = %ld , denom = %ld\n", num, dnom);
//         #endif
//         numer = num; 
//         denom = dnom;  
//       }
     
//       if(numer % denom == 0){
//         #ifdef Debug
//           printf("dnom == 0\n");
//         #endif
//         numer = numer /denom;
//         denom = 1;
//       }else{
//         long count = 1;
//         #ifdef Debug
//           printf("begining count count = %ld n = %ld d= %ld\n", count , numer, denom);
//         #endif
//         while(count < numer && count < denom){
//           if(numer % count == 0 && denom % count == 0){
//             #ifdef Debug
//             printf("this is divisable by count = %ld, n = %ld d= %ld\n", count , numer, denom);
//             #endif
//             numer = numer /count;
//             denom = denom / count;
//             count = 1;
//             #ifdef Debug
//               printf("after dividing = %ld, n = %ld d= %ld\n", count , numer, denom);
//             #endif
//           };
//           count++;
//         };
//       }
//     };
//     rational(long num)
//     {    
//       #ifdef Debug
//         printf("only num %ld\n", num);
//       #endif
//       numer = num; 
//       denom = 1;
//     };
//     rational()
//     {    
//       numer = 0; 
//       denom = 0;
//     };

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
//       rational res(numer * a.numer, denom * a.denom);
//       return res;

//     };
//     rational operator/ (rational a)
//     {
//       rational res = rational(numer, denom)* rational(a.denom, a.numer);
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
