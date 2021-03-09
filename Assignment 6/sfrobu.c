#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>

int normcmp(const void* a, const void* b) {
  return frobcmp((char*) a, (char*) b, 0);
}

int fcmp(const void* a, const void* b) {
  return frobcmp((char*) a, (char*) b, 1);
}

int frobcmp(char const *a, char const *b, int use_f_option) {
  int i = 0;
  char* a_ptr = *(char**)a;
  char* b_ptr = *(char**)b;
  char xor_a, xor_b;

  while(a_ptr[i] != ' ' && b_ptr[i] != ' ') {
    xor_a = a_ptr[i] ^ 42;
    xor_b = b_ptr[i] ^ 42;

    if(use_f_option == 1) {
      if(xor_a <= UCHAR_MAX && xor_a  >= 0)
	xor_a = toupper(xor_a);
      if(xor_b <= UCHAR_MAX && xor_b >= 0)
	xor_b = toupper(xor_b);
    }

    if(xor_a > xor_b)
      return 1;
    else if(xor_b > xor_a)
      return -1;

    i = i + 1;
  }
  
  i = 0;
  int a_length = 0;
  int b_length = 0;

  while(a_ptr[i] != ' ') {
    a_length = a_length + 1;
    i = i + 1;
  }

  i = 0;

  while(b_ptr[i] != ' ') {
    b_length = b_length + 1;
    i = i+1;
  }

  if(a_length > b_length)
    return 1;
  else if(b_length > a_length)
    return -1;

  return 0;
}

int main(int argc, char* argv[]) {
  //initialize ptr with 1 element
  char** ptr;
  ptr = malloc(sizeof(*ptr));
  ptr[0] = malloc(sizeof(char));
  int i = 0;

  //these variables hold the position for the next char
  int cur_row = 0;
  int cur_col = 0;
  int cur_char;
  

  struct stat info;
  int num_bytes = 8192;
  int num_read;
  int tot_read;
  char* buffer;

  //get some information on input file size
  fstat(0, &info);
  if(info.st_size != 0) {
    buffer = malloc(info.st_size * sizeof(char));
    tot_read = read(0, buffer, num_bytes);
  }
  else {
    buffer = malloc(num_bytes * sizeof(char));
    num_read = read(0, buffer, num_bytes);
    tot_read = num_read;

      while(num_read > 0) {
	num_bytes = num_bytes * 2;
	buffer = realloc(buffer, num_bytes * sizeof(char));
	num_read = read(0, &buffer[tot_read], num_bytes/2);
	if(num_read > 0)
	  tot_read = tot_read + num_read;
      }

  }

  while(i < tot_read) {
    cur_char = buffer[i];

    if(cur_char == ' ') {
      //space indicates last word ended, append SP char
      ptr[cur_row][cur_col] = ' ';
      
      //create extra space for next row in ptr
      ptr = (char**) realloc(ptr, ((cur_row + 2) * sizeof(char*)));

      //go to next row, and give it one element
      cur_row = cur_row + 1;
      ptr[cur_row] = malloc(sizeof(char));

      //start again at 0th element of current row
      cur_col = 0;
    }
    else {
      //put the current char in [cur_row][cur_col]
      ptr[cur_row][cur_col] = cur_char;
      
      //create more space for next char in array
      ptr[cur_row] = (char*) realloc(ptr[cur_row], ((cur_col + 2) *  sizeof(char)));
      cur_col = cur_col + 1;
    }
    i = i + 1;
  }
  ptr[cur_row][cur_col] = ' ';

  if(argc == 2 && argv[1][0] == '-' && argv[1][1] == 'f')
    qsort(ptr, cur_row + 1, sizeof(char*), fcmp);
  else
    qsort(ptr, cur_row + 1, sizeof(char*), normcmp);

  char store_space = ' ';
  for(i = 0; i <= cur_row; i = i + 1) {
    cur_col = 0;

    while(ptr[i][cur_col] != ' ') {
      write(1, &ptr[i][cur_col], 1);
      cur_col = cur_col + 1;
    }
    write(1, &store_space, 1);
  }

  for(i = 0; i <= cur_row; i = i + 1)
    free(ptr[i]);

  free(ptr);

  free(buffer);
}
