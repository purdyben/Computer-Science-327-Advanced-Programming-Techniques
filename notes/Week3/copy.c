
/* Copy files */

#include <stdio.h>

/* For now */

int main(int argc, char* argv[])
{
  if (3 != argc) {
    fprintf(stderr, "Usage: %s src dest\n", argv[0]);
    return 1;
  }
  FILE* infile = fopen(argv[1], "r");
  if (!infile) {
    fprintf(stderr, "Couldn't read %s\n", argv[1]);
    return 1;
  }
  FILE* outfile = fopen(argv[2], "w");
  if (!outfile) {
    fprintf(stderr, "Couldn't write %s\n", argv[2]);
    return 1;
  }

  int c;
  for (;;) {
    c = fgetc(infile);
    if (EOF == c) break;
    fputc(c, outfile);
  }
  
  fclose(infile);
  fclose(outfile);
  return 0;
}


