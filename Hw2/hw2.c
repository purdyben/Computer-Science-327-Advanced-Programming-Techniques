
/*
    Greetings.

    Code is not documented on purpose.
    But at least I didn't run it through a code obfuscator.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE+1];
int  bufptr;

void addToBuffer(char* s)
{
  if (0==s) return;
  for (; *s; s++) {
    if (bufptr >= BUFFER_SIZE) {
      fprintf(stderr, "Name to long\n");
      exit(1);
    }
    if (*s == ' ') continue;
    if (*s == '\t') continue;
    buffer[bufptr++] = *s;
  }
}

void buffer2code(char* codestr, unsigned len)
{
  int i;
  unsigned long hash = 5381;
  unsigned short hash2 = 101;
  unsigned m,b;
  for (i=0; buffer[i]; i++) {
    hash = ((hash << 5) + hash) + buffer[i];
    hash2 = ((hash2 << 5) + hash2) + buffer[i];
  }
  snprintf(codestr, len, "%016lx", hash);
  if (len < 34) return;
  m = 0x8000;
  for (i=16; i<32; i++) {
    b = m & hash2;
    codestr[i] = b ? 9 : 32;
    m /= 2;
  }
  codestr[32] = 10;
  codestr[33] = 0;
  if (len < 42) return;
  unsigned long code = 0x657962646f6f47;
  strcpy(codestr+33, (char*) &code);
}

int RunNames(int argc, char** argv)
{
  int i;
  printf("Name:");
  for (i=0; i<argc; i++) {
    addToBuffer(argv[i]);
    printf(" %s", argv[i]);
  }
  printf("\n");

  char code[256];
  buffer2code(code, 256);

  printf("Code: %s\n", code);

  return 0;
}

void readline(FILE* inf, int skip, char* buf, unsigned len)
{
  int c;
  int i;
  for (i=0; i<skip; i++) {
    c = fgetc(inf);
    if ((EOF==c) || ('\n'==c)) {
      buf[0] = 0;
      return;
    }
  }
  for (i=0; i<len; i++) {
    c = fgetc(inf);
    if ((EOF==c) || ('\n'==c)) {
      buf[i] = 0;
      return;
    }
    buf[i] = c;
  }
  buf[len-1] = 0;
}

int CheckNames(char* file) 
{
  if (0==file) return 1;

  FILE* inf = fopen(file, "r");
  if (0==inf) {
    fprintf(stderr, "Couldn't open file %s\n", file);
    return 1;
  }

  char line[BUFFER_SIZE+6];
  readline(inf, 6, line, BUFFER_SIZE+6);
  addToBuffer(line);

  printf("Name: %s\n", line);

  char code[256];
  buffer2code(code, 256);
  fread(line, 1, 6, inf);
  fread(line, 1, 40, inf);
  line[40] = 0;
  fclose(inf);

  if (0==strcmp(code, line)) {
    printf("Code strings match\n");
    return 0;
  }

  if (0==strncmp(code, line, 16)) {
    printf("Code strings partly match\n");
    return 0;
  }

  printf("Code strings did NOT match\n");
  return 0;
}

int main(int argc, char** argv)
{
  if (argc <= 1) {
    printf("  Usage: %s Student Name\n", argv[0]);
    return 1;
  }
  bufptr = 0;
  bzero(buffer, BUFFER_SIZE+1);
  if (0==strcmp("-g", argv[1])) {
    return CheckNames(argv[2]);
  }
  return RunNames(argc-1, argv+1);
}
