#include <stdio.h>
#include <stdlib.h>

int frobcmp(char const *a, char const *b)
{
  int i = 0;
  char* a_ptr = *(char**)a;
  char* b_ptr = *(char**)b;

  while(a_ptr[i] != ' ' && b_ptr[i] != ' ')
  {

    char xor_a = a_ptr[i] ^ 42;
    char xor_b = b_ptr[i] ^ 42;
    if(xor_a > xor_b)
    {

      return 1;
    }
    else if(xor_b > xor_a)
    {

      return -1;
    }


    i = i + 1;
  }
  
  i = 0;
  int a_length = 0;
  int b_length = 0;

  while(a_ptr[i] != ' ')
  {
    
    a_length = a_length + 1;
    i = i + 1;
  }

  i = 0;

  while(b_ptr[i] != ' ')
  {
   
    b_length = b_length + 1;
    i = i+1;
  }

  if(a_length > b_length)
    return 1;
  else if(b_length > a_length)
    return -1;

  return 0;
}

int compare(const void * a, const void * b)
{

  return frobcmp((char*)a, (char*)b);
}

int main()
{
  //initialize ptr with 1 element
  char** ptr;
  ptr = malloc(sizeof(*ptr));
  ptr[0] = malloc(sizeof(char));

  //these variables hold the position for the next char
  int cur_row = 0;
  int cur_col = 0;
  int cur_char;

  for(;;)
  {
    
    cur_char = getchar();

    if(cur_char == EOF)
    {
      //If at EOF append ' ' char to indicate end of word
      ptr[cur_row][cur_col] = ' ';

      //then break out of loop, no more chars
      break;
    }
    else if(cur_char == ' ')
    {
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
    else
    {
      //put the current char in [cur_row][cur_col]
      ptr[cur_row][cur_col] = cur_char;
      
      //create more space for next char in array
      ptr[cur_row] = (char*) realloc(ptr[cur_row], ((cur_col + 2) *  sizeof(char)));
      cur_col = cur_col + 1;
    }
  }

  qsort(ptr, cur_row + 1, sizeof(char*), compare);
  
  int i;

  for(i = 0; i <= cur_row; i = i + 1)
  {
    cur_col = 0;

    while(ptr[i][cur_col] != ' ')
    {

      putchar(ptr[i][cur_col]);
      cur_col = cur_col + 1;
    }

    printf(" ");
  }

  for(i = 0; i <= cur_row; i = i + 1)
    free(ptr[i]);

  free(ptr);
 
}
