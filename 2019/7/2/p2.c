#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "circular_buffer.h"

void print_buffer_status(cbuf_handle_t cbuf)
{
  printf("Full: %d, empty: %d, size: %zu\n",
         circular_buf_full(cbuf),
         circular_buf_empty(cbuf),
         circular_buf_size(cbuf));
}

// prepend "0" as needed resulting in a string of _minimal_ width.
void PrependZeros(char *dest, const char *src, unsigned minimal_width)
{
  size_t len = strlen(src);
  size_t zeros = (len > minimal_width) ? 0 : minimal_width - len;
  memset(dest, '0', zeros);
  strcpy(dest + zeros, src);
}

int *cvt(char *input, int *level)
{
  char *cp = strtok(input, ",");
  if (cp == NULL)
  {
    /* No more separators */
    return (int *)malloc(sizeof(int) * *level);
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
  int *array = cvt(NULL, level);
  if (my_index >= 0)
  {
    array[my_index] = n;
  }
  return array;
}

void reverse(char s[])
{
  int i, j;
  char c;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
  {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void itoa(int n, char s[])
{
  int i, sign;

  if ((sign = n) < 0) /* record sign */
    n = -n;           /* make n positive */
  i = 0;
  do
  {                        /* generate digits in reverse order */
    s[i++] = n % 10 + '0'; /* get next digit */
  } while ((n /= 10) > 0); /* delete it */
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
}

int getOpcode(int *program, int pc)
{
  int opcode = program[(int)pc];
  char tmp[5];
  char opcodestr[5];
  itoa(opcode, tmp);
  PrependZeros(opcodestr, tmp, 5);
  char opstr[3];
  opstr[0] = opcodestr[strlen(opcodestr) - 2];
  opstr[1] = opcodestr[strlen(opcodestr) - 1];
  opstr[2] = '\0';
  return atoi(opstr);
}

int getMode(int *program, int pc, int param)
{
  int opcode = program[(int)pc];
  int nDigits = floor(log10(abs(opcode))) + 1;
  char tmp[5];
  char opcodestr[5];
  itoa(opcode, tmp);
  PrependZeros(opcodestr, tmp, 5);
  if (nDigits == 2)
  {
    return 0;
  }
  else if (opcodestr[3 - param] == '1')
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int getValue(int *program, int pc, int param)
{
  int mode = getMode(program, pc, param);
  int opcode = program[(int)pc];
  if (mode == 0)
  {
    return program[(int)program[(int)pc + param]];
  }
  else
  {
    return program[(int)pc + param];
  }
}

void printProgram(int *program)
{
  for (int i = 0; i < 100; i++)
  {
    printf("%i, ", program[(int)i]);
  }
  printf("\n");
}

int writevalue(int *program, int pc, int value, int param)
{
  int mode = getMode(program, pc, param);
  if (mode == 1)
  {
    printf("Write Abort: Mode 1\n");
    return 1;
  }
  else
  {
    program[(int)program[(int)pc + param]] = value;
    return 0;
  }
}

void opcode1(int *program, int pc)
{
  int value = getValue(program, pc, 1) + getValue(program, pc, 2);
  writevalue(program, pc, value, 3);
  //printf("val: %i\n", program[program[pc+3]]);
}

void opcode2(int *program, int pc)
{
  int value = getValue(program, pc, 1) * getValue(program, pc, 2);
  writevalue(program, pc, value, 3);
}

void opcode3(int *program, int input, int par1)
{
  program[(int)par1] = input;
}

int opcode4(int *program, int pc)
{
  int val = getValue(program, pc, 1);
  //("Output: %i\n", val);
  return val;
}

int opcode5(int *program, int pc)
{
  if (getValue(program, pc, 1) == 0)
  {
    return pc + 3;
  }
  else
  {
    return getValue(program, pc, 2);
  }
}

int opcode6(int *program, int pc)
{
  if (getValue(program, pc, 1) == 0)
  {
    return getValue(program, pc, 2);
  }
  else
  {
    return pc + 3;
  }
}

void opcode7(int *program, int pc)
{
  if ((int)getValue(program, pc, 1) < (int)getValue(program, pc, 2))
  {

    program[(int)program[(int)pc + 3]] = 1;
  }
  else
  {
    program[(int)program[(int)pc + 3]] = 0;
  }
  //printf("%i %i\n", getValue(program, pc, 1) ,getValue(program, pc, 2));
}

void opcode8(int *program, int pc)
{
  if (getValue(program, pc, 1) == getValue(program, pc, 2))
  {
    program[(int)program[(int)pc + 3]] = 1;
  }
  else
  {
    program[(int)program[(int)pc + 3]] = 0;
  }
}

int execute(int *program, int *pc, cbuf_handle_t input, cbuf_handle_t output, char *amp)
{
  // printf("%s\n", amp,"");
  bool is_halted = false;
  while (!is_halted)
  {
    //  printf("%i\n", pc);
    int opcode = getOpcode(program, *pc);

    //printProgram(program);
    //printf("PC: %i\n Val: %i\n", pc, program[pc]);
    //int tmp = program[pc+2];
    //printf("%i \n", tmp);
    //printf("opcode: %i %i %i %i %i \n", pc, program[pc], program[pc+1], getValue(program, pc, 2), program[pc+3]);
    //printf("%i %i\n", pc, program[pc]);
    //printf("%i %i %i %i %i\n", *pc, program[*pc], program[*pc+1], program[*pc+2], program[*pc+3]);

    if (*pc > 3000)
    {
      printf("Program Count Exception!\n");
      break;
    }
    if (opcode == 99)
    {
      printf("OpCode 99 Exit!!!\n");
      is_halted = true;
    }
    else if (opcode == 1)
    {
      //addition
      opcode1(program, *pc);
      *pc = *pc + 4;
    }
    else if (opcode == 2)
    {
      //multiply
      opcode2(program, *pc);
      *pc = *pc + 4;
    }
    else if (opcode == 3)
    {
      int val = 0;
      circular_buf_get(input, &val);
      //printf("\n%i %i\n", pc+1, program[(int) pc+1]);
      int dest = program[(int)*pc + 1];
      opcode3(program, val, dest);
      *pc = *pc + 2;
    }
    else if (opcode == 4)
    {
      *pc = *pc + 2;
      int val = 0;

      val = opcode4(program, *pc - 2);
      //     printf("%i\n",val);
      circular_buf_put(output, val);
      return val;
    }
    else if (opcode == 5)
    {

      *pc = opcode5(program, *pc);
    }
    else if (opcode == 6)
    {

      *pc = opcode6(program, *pc);
    }
    else if (opcode == 7)
    {

      opcode7(program, *pc);
      *pc = *pc + 4;
    }
    else if (opcode == 8)
    {

      opcode8(program, *pc);
      *pc = *pc + 4;
    }
    else
    {
      printf("Error: %i %i %i %i %i \n", *pc, program[(int)*pc], program[(int)*pc + 1], getValue(program, *pc, 2), program[*pc + 3]);
      exit(1);
    }

    //printProgram(program);
    //getchar();
  }
  return 99;
}

int main(int argc, char *argv[])
{
  const int memalloc = 10000;
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
  int *program = (int *)calloc(memalloc, sizeof(int));
  //int *backup = (int*) calloc(memalloc,sizeof(int));
  int *runtime1 = (int *)calloc(memalloc, sizeof(int));
  int *runtime2 = (int *)calloc(memalloc, sizeof(int));
  int *runtime3 = (int *)calloc(memalloc, sizeof(int));
  int *runtime4 = (int *)calloc(memalloc, sizeof(int));
  int *runtime5 = (int *)calloc(memalloc, sizeof(int));

  if (program == NULL)
  {
    printf("malloc of size %d failed!\n", memalloc); // could also call perror here
    exit(1);                                         // or return an error to caller
  }
  if (runtime1 == NULL)
  {
    printf("malloc of size %d failed!\n", memalloc); // could also call perror here
    exit(1);                                         // or return an error to caller
  }
  if (runtime2 == NULL)
  {
    printf("malloc of size %d failed!\n", memalloc); // could also call perror here
    exit(1);                                         // or return an error to caller
  }
  if (runtime3 == NULL)
  {
    printf("malloc of size %d failed!\n", memalloc); // could also call perror here
    exit(1);                                         // or return an error to caller
  }
  if (runtime4 == NULL)
  {
    printf("malloc of size %d failed!\n", memalloc); // could also call perror here
    exit(1);                                         // or return an error to caller
  }
  if (runtime5 == NULL)
  {
    printf("malloc of size %d failed!\n", memalloc); // could also call perror here
    exit(1);                                         // or return an error to caller
  }

  fgets(currentline, sizeof(currentline), ifp);
  fclose(ifp);
  int n_array = 0;

  int *pc1 = calloc(1, sizeof(int));
  int *pc2 = calloc(1, sizeof(int));
  int *pc3 = calloc(1, sizeof(int));
  int *pc4 = calloc(1, sizeof(int));
  int *pc5 = calloc(1, sizeof(int));

  *pc1 = 0;
  *pc2 = 0;
  *pc3 = 0;
  *pc4 = 0;
  *pc5 = 0;
  program = cvt(currentline, &n_array);
  int loop;
  for (loop = 0; loop < memalloc; loop++)
  {
    // printf("loop: %i\n", loop);
    //  printf("%i \n", program[loop]);
    //    int mp = program[loop];
    //    backup[loop] = mp;
    runtime1[(int)loop] = program[(int)loop];
    runtime2[(int)loop] = program[(int)loop];
    runtime3[(int)loop] = program[(int)loop];
    runtime4[(int)loop] = program[(int)loop];
    runtime5[(int)loop] = program[(int)loop];
  }
  // program[0] = 101;
  // printf("%i %i",getMode(program, pc, 1), getMode(program, pc, 2));
  // exit(0);
  int maxstate[5];
  int output;
  int outputA = 0;
  int outputB = 0;
  int outputC = 0;
  int outputD = 0;
  int outputE = 0;
  int maxoutput = 0;

  int BUFFER_SIZE = 10;

  int *buffer1 = malloc(BUFFER_SIZE * sizeof(int));
  int *buffer2 = malloc(BUFFER_SIZE * sizeof(int));
  int *buffer3 = malloc(BUFFER_SIZE * sizeof(int));
  int *buffer4 = malloc(BUFFER_SIZE * sizeof(int));
  int *buffer5 = malloc(BUFFER_SIZE * sizeof(int));

  printf("\n=== C Circular Buffer Check ===\n");

  cbuf_handle_t cbuf1 = circular_buf_init(buffer1, BUFFER_SIZE);
  cbuf_handle_t cbuf2 = circular_buf_init(buffer2, BUFFER_SIZE);
  cbuf_handle_t cbuf3 = circular_buf_init(buffer3, BUFFER_SIZE);
  cbuf_handle_t cbuf4 = circular_buf_init(buffer4, BUFFER_SIZE);
  cbuf_handle_t cbuf5 = circular_buf_init(buffer5, BUFFER_SIZE);

  printf("Buffer initialized. ");
  print_buffer_status(cbuf1);
  print_buffer_status(cbuf2);
  print_buffer_status(cbuf3);
  print_buffer_status(cbuf4);
  print_buffer_status(cbuf5);

  int b = 5;
  int x = 10;
  output = 0;
  for (int i = b; i < x; i++)
  {
    for (int j = b; j < x; j++)
    {
      for (int k = b; k < x; k++)
      {
        for (int l = b; l < x; l++)
        {
          for (int m = b; m < x; m++)
          {
            if (i != j && i != k && i != l && i != m)
            {
              if (j != k && j != l && j != m)
              {
                if (k != l && k != m)
                {
                  if (l != m)
                  {

                    //printf("%i\n", l);

                    circular_buf_put(cbuf1, i);
                    circular_buf_put(cbuf1, 0);

                    circular_buf_put(cbuf2, j);
                    circular_buf_put(cbuf3, k);
                    circular_buf_put(cbuf4, l);
                    circular_buf_put(cbuf5, m);
                    //  print_buffer_status(cbuf1);
                    // print_buffer_status(cbuf2);
                    //  print_buffer_status(cbuf3);
                    //  print_buffer_status(cbuf4);
                    //  print_buffer_status(cbuf5);

                    while (1)
                    {

                      outputA = execute(runtime1, pc1, cbuf1, cbuf2, "A");


                      outputB = execute(runtime2, pc2, cbuf2, cbuf3, "B");

                      outputC = execute(runtime3, pc3, cbuf3, cbuf4, "C");

                      outputD = execute(runtime4, pc4, cbuf4, cbuf5, "D");


                      outputE = execute(runtime5, pc5, cbuf5, cbuf1, "E");

                      //printf("MAXIMUM: %i %i %i %i %i %i\n", outputE, i, j, k, l, m);

                      if (outputE == 99)
                      {
                        int data = 0;
                        
                        circular_buf_get(cbuf1, &data);
                        circular_buf_put(cbuf1,data);
                        //printf("outputE: %i data: %i maxout: %i\n", outputE, data, maxoutput);
                        //printf("%i %i %i %i %i\n",i,j,k,l,m);
                        if (data > maxoutput)
                        {
                          maxoutput = data;
                          maxstate[0] = i;
                          maxstate[1] = j;
                          maxstate[2] = k;
                          maxstate[3] = l;
                          maxstate[4] = m;
                        }
                        break;
                      }
                    }

                    *pc1 = 0;
                    *pc2 = 0;
                    *pc3 = 0;
                    *pc4 = 0;
                    *pc5 = 0;

                    for (loop = 0; loop < memalloc; loop++)
                    {
                      runtime1[loop] = program[loop];
                      runtime2[loop] = program[loop];
                      runtime3[loop] = program[loop];
                      runtime4[loop] = program[loop];
                      runtime5[loop] = program[loop];
                    }

                    circular_buf_reset(cbuf1);
                    circular_buf_reset(cbuf2);
                    circular_buf_reset(cbuf3);
                    circular_buf_reset(cbuf4);
                    circular_buf_reset(cbuf5);
                  }
                }
              }
            }
            outputE = 0;
          }
        }
      }
    }
  }
  n_array = 0;
  //  for(loop = 0; loop < 300; loop++) {
  //    program[loop] = backup[loop];
  //  }

  printf("Answer : %i %i %i %i %i %i\n", maxoutput, maxstate[0], maxstate[1], maxstate[2], maxstate[3], maxstate[4]);
  return 0;
}
