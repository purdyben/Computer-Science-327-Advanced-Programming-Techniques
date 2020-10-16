
#include <stdio.h>

int main()
{
  int A[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  int i = 2;

  A[++i] = A[i+5] = 42;

  printf("A=[%d", A[0]);
  for (i=1; i<10; i++) {
    printf(", %d", A[i]);
  }
  printf("]\n");
  return 0;
}
