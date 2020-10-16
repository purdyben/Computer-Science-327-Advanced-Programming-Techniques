
#include <stdio.h>

int main()
{
  char name[20];
  printf("What is your name?\n");
  
  /* NEVER EVER USE THIS FUNCTION: */
  gets(name);

  printf("Hello, %s!\n", name);
  return 0;
}
