#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int a_len = 0;//stores length of 2nd arg
  int b_len = 0;//stores length of 2nd arg

  int i = 0;//primary iterating variable
  int t = 0;//secondart iterating variable, used when 2 are needed

  char c;

  char* text = malloc(sizeof(char));
  int text_size = 1;

  if(argc < 3) {
    fprintf(stderr, "Error! %d arguments provided. Two arguments needed.\n", argc);
    return 1;
  }
  else if(argc > 3) {
    fprintf(stderr, "Error! %d arguments provided. Only two arguments needed.\n", argc);
    return 1;
  }

  while(argv[1][i] != '\0') {
    a_len = a_len + 1;
    i = i + 1;
  }

  i = 0;
  while(argv[2][i] != '\0') {
    b_len = b_len + 1;
    i = i + 1;
  }
  
  if(a_len != b_len) {
    fprintf(stderr, "1st arg len: %d, 2nd arg len: %d. ", a_len, b_len);
    fprintf(stderr, "Argument lengths must match. Exiting...\n");
    return 1; 
  }

  i = 0;
  for(; i < a_len; i++) {
    for(t = i + 1; t < a_len; t++) {
      if(argv[1][i] == argv[1][t]) {
	fprintf(stderr, "a character was repeated in the first argument.\n");
	fprintf(stderr, "Offending character: %c, exiting...\n", argv[1][i]);
	return 1;
      } 
    }
  }


  i = 0;
  while(read(0, &c, 1) > 0) {
    text = realloc(text, text_size*sizeof(char));
    text[i] = c;
    i = i + 1;
    text_size = text_size + 1;
  }

  i = 0;
  while(text[i] != '\0') {
    for(t = 0; t < b_len; t = t + 1) {
      if(text[i] == argv[1][t]) {
	text[i] = argv[2][t];
      }
    }
    i = i + 1;
  }

  i = 0;
  while(text[i] != '\0') {
    write(1, &text[i], 1);
    i = i + 1;
  }
  text[0] = '\n';
  write(1, &text[0], 1);

  free(text);
  return 0;
}
