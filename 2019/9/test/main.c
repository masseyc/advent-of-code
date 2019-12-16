#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct
{
  __int128 *program;
  bool is_halted;
  int rbase;
} Computer;

Computer *computer_new(__int128 *new_program)
{
  Computer *a = malloc(sizeof(Computer));
  a->program = new_program;
  a->is_halted = false;
  a->rbase = 0;
  return a;
}

__int128 *cvt(char *input, int *level)
{
  char *cp = strtok(input, ",");
  if (cp == NULL)
  {
    /* No more separators */
    return (__int128 *)malloc(sizeof(__int128) * *level);
  }

  int my_index = -1;
  int n;
  if (sscanf(cp, "%d", &n) == 1)
  {
    my_index = *level;
    *level += 1;
  }
  else
  {
    printf("Invalid integer token '%s'\n", cp);
  }
  __int128 *array = cvt(NULL, level);
  if (my_index >= 0)
  {
    array[my_index] = n;
  }
  return array;
}

int main(int argc, char *argv[])
{
  const int memalloc = 20000;
  int input;
  if (argc > 1)
  {
    input = (int)strtol(argv[1], (char **)NULL, 10);
  }
  else
  {
    //    exit(1);
  }

  FILE *ifp;
  char *mode = "r";
  char currentline[4000];

  ifp = fopen("input.list", mode);

  if (ifp == NULL)
  {
    fprintf(stderr, "Can't open input file in.list!\n");
    exit(1);
  }
  __int128 *program = (__int128 *)calloc(memalloc, sizeof(__int128));
  __int128 *program1 = (__int128 *)calloc(memalloc, sizeof(__int128));

  Computer *runtime1 = computer_new(program1);

  if (program == NULL)
  {
    printf("malloc of size %d failed!\n", memalloc); // could also call perror here
    exit(1);                                         // or return an error to caller
  }
   if (runtime1->program == NULL)
   {
     printf("malloc of size %d failed!\n", memalloc); // could also call perror here
     exit(1);                                         // or return an error to caller
   }

  fgets(currentline, sizeof(currentline), ifp);
  fclose(ifp);
  int n_array = 0;

  program = cvt(currentline, &n_array);
  int loop;
  for (loop = 0; loop < memalloc; loop++)
  {

    runtime1->program[loop] = program[loop];
  }
 
  return 0;
}
