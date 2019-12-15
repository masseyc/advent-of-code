#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>

#include "circular_buffer.h"

typedef struct
{
  int64_t *program;
  bool is_halted;
  int rbase;
} Computer;

Computer *computer_new(int64_t *new_program)
{
  Computer *a = malloc(sizeof(Computer));
  a->program = new_program;
  a->is_halted = false;
  a->rbase = 0;
  return a;
}

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

int64_t *cvt(char *input, int64_t *level)
{
  char *cp = strtok(input, ",");
  if (cp == NULL)
  {
    /* No more separators */
    return (int64_t *)malloc(sizeof(int64_t) * *level);
  }

  int64_t my_index = -1;
  int64_t n;
  if (sscanf(cp, "%d", &n) == 1)
  {
    my_index = *level;
    *level += 1;
  }
  else
  {
    printf("Invalid integer token '%s'\n", cp);
  }
  int64_t *array = cvt(NULL, level);
  if (my_index >= 0)
  {
    array[my_index] = n;
  }
  return array;
}

void reverse(char s[])
{
  int64_t i, j;
  char c;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
  {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void itoa(int64_t n, char s[])
{
  int64_t i, sign;

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

int64_t getOpcode(Computer *program, int pc)
{
  int64_t opcode = program->program[(int)pc];
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

int getMode(Computer *program, int pc, int param)
{
  int64_t opcode = program->program[(int64_t)pc];
  int64_t nDigits = floor(log10(abs(opcode))) + 1;
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
  else if (opcodestr[3 - param] == '2')
  {
    return 2;
  }
  else
  {
    return 0;
  }
}

int64_t getValue(Computer *program, int pc, int param)
{
  int mode = getMode(program, pc, param);
  int opcode = program->program[(int)pc];
  if (mode == 0)
  {
    return program->program[(int)program->program[(int)pc + param]];
  }
  else if (mode == 2)
  {
    return program->program[(int)program->rbase + (int)program->program[(int)pc + param]];
  }
  else
  {
    return program->program[(int)pc + param];
  }
}

void printProgram(Computer *program)
{
  for (int i = 0; i < 100; i++)
  {
    printf("%i, ", program->program[(int)i]);
  }
  printf("\n");
  printf("is_halted: %i\n", program->is_halted);
  printf("rbase: %i\n", program->rbase);
}

int64_t writevalue(Computer *program, int pc, int64_t value, int param)
{
  int mode = getMode(program, pc, param);
  //printf("%i\n", mode);
  if (mode == 1)
  {
    printf("Write Abort: Mode 1\n");
    return 1;
  }
  else if(mode == 2)
  {
    
    //printf("%i %i %i %i %i \n", program->program[(int) pc+param],program->rbase, program->rbase + program->program[pc + param], program->program[1013], value);
    //printProgram(program);
    //program->program[1013] = 43;
    //printf("%i %i\n", (program->program[pc + param])+program->rbase, value);
    program->program[(int)(program->program[pc + param]) + (int)program->rbase] = value;
    return 0;
  }
  else
  {
    program->program[(int)program->program[(int)pc + param]] = value;
    return 0;
  }
}

void opcode1(Computer *program, int pc)
{
  int64_t value = getValue(program, pc, 1) + getValue(program, pc, 2);
  writevalue(program, pc, value, 3);
  //printf("val: %i\n", program[program[pc+3]]);
}

void opcode2(Computer *program, int pc)
{
  printf("%"PRId64" %"PRId64" %"PRId64" %"PRId64"\n", program->program[pc], program->program[pc+1], program->program[pc+2], program->program[pc+3]);
  int64_t value = getValue(program, pc, 1) * getValue(program, pc, 2);

  writevalue(program, pc, value, 3);
  printf("%"PRId64"\n", program->program[63]);
}

void opcode3(Computer *program, int pc, int64_t input, int par1)
{
  if (getMode(program, pc, 1) == 2)
  {
    program->program[(int) par1+program->rbase] = input;
  }
  else
  {
    program->program[(int)par1] = input;
  }
}

int64_t opcode4(Computer *program, int pc)
{
  int64_t val = getValue(program, pc, 1);
  //("Output: %i\n", val);
  return val;
}

int64_t opcode5(Computer *program, int pc)
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

int64_t opcode6(Computer *program, int pc)
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

void opcode7(Computer *program, int pc)
{
  if ((int64_t)getValue(program, pc, 1) < (int64_t)getValue(program, pc, 2))
  {

    program->program[(int)getValue(program,pc,3)] = 1;
  }
  else
  {
    //printf("%i\n",getValue(program,pc,3));
    program->program[(int)getValue(program,pc,3)] = 0;
  }
  //printf("%i %i\n", getValue(program, pc, 1) ,getValue(program, pc, 2));
}

void opcode8(Computer *program, int pc)
{
  if ((int64_t)getValue(program, pc, 1) == (int64_t)getValue(program, pc, 2))
  {
    //printf("%i %i %i\n",getValue(program, pc, 1), getValue(program, pc, 2), (int)getValue(program,pc,3));
    program->program[(int)getValue(program,pc,3)] = 1;
  }
  else
  {
    //printf("%i %i %i\n",getValue(program, pc, 1), getValue(program, pc, 2), (int)getValue(program,pc,3));
    program->program[(int)getValue(program,pc,3)] = 0;
  }
}

void opcode9(Computer *program, int pc)
{
  
  program->rbase += getValue(program, pc, 1);
}

int64_t execute(Computer *program, int *pc, cbuf_handle_t input, cbuf_handle_t output, char *amp)
{
  // printf("%s\n", amp,"");
  while (!program->is_halted)
  {
    //  printf("%i\n", pc);
    int opcode = getOpcode(program, *pc);

    //printProgram(program);
    //printf("PC: %i\n Val: %i\n", pc, program[pc]);
    //int64_t tmp = program[pc+2];
    //printf("%i \n", tmp);
    //printf("opcode: %i %i %i %i %i \n", pc, program[pc], program[pc+1], getValue(program, pc, 2), program[pc+3]);
    //printf("%i %i\n", pc, program[pc]);
    //printf("%i %"PRId64" %"PRId64" %"PRId64" %"PRId64"\n", *pc, program[*pc], program[*pc+1], program[*pc+2], program[*pc+3]);

    if (*pc > 3000)
    {
      printf("Program Count Exception!\n");
      break;
    }
    if (opcode == 99)
    {
      printf("OpCode 99 Exit!!!\n");
      program->is_halted = true;
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
      int64_t val = 0;
      circular_buf_get(input, &val);
      //printf("\n%i %i\n", pc+1, program[(int64_t) pc+1]);
      int64_t dest = program->program[(int)*pc + 1];
      opcode3(program, *pc, val, dest);
      *pc = *pc + 2;
    }
    else if (opcode == 4)
    {
      *pc = *pc + 2;
      int64_t val = 0;

      val = opcode4(program, *pc - 2);
      printf("%"PRId64"\n", val);
      circular_buf_put(output, val);
      //return val;
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
    else if (opcode == 9)
    {

      opcode9(program, *pc);
      *pc = *pc + 2;
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

int64_t main(int64_t argc, char *argv[])
{
  const int64_t memalloc = 16000;
  int64_t input;
  if (argc > 1)
  {
    input = (int64_t)strtol(argv[1], (char **)NULL, 10);
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
  int64_t *program = (int64_t *)calloc(memalloc, sizeof(int64_t));
  //int64_t *backup = (int64_t*) calloc(memalloc,sizeof(int64_t));
  int64_t *program1 = (int64_t *)calloc(memalloc, sizeof(int64_t));

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
  int64_t n_array = 0;

  int *pc1 = calloc(1, sizeof(int));

  *pc1 = 0;

  program = cvt(currentline, &n_array);
  int loop;
  for (loop = 0; loop < memalloc; loop++)
  {
    // printf("loop: %i\n", loop);
    //  printf("%i \n", program[loop]);
    //    int64_t mp = program[loop];
    //    backup[loop] = mp;
    //printf("%i\n", loop);
    runtime1->program[(int)loop] = program[(int)loop];
  }
  // program[0] = 101;
  // printf("%i %i",getMode(program, pc, 1), getMode(program, pc, 2));
  // exit(0);
  int64_t output;
  int64_t outputA = 0;
  int64_t maxoutput = 0;

  int BUFFER_SIZE = 10;

  int64_t *buffer1 = malloc(BUFFER_SIZE * sizeof(int64_t));

  printf("\n=== C Circular Buffer Check ===\n");

  cbuf_handle_t cbuf1 = circular_buf_init(buffer1, BUFFER_SIZE);

  printf("Buffer initialized. ");
  print_buffer_status(cbuf1);

  output = 0;

  //printf("%i\n", l);

  circular_buf_put(cbuf1, 1);

  outputA = execute(runtime1, pc1, cbuf1, cbuf1, "A");

  *pc1 = 0;

  for (loop = 0; loop < memalloc; loop++)
  {
    runtime1->program[loop] = program[loop];
  }

  circular_buf_reset(cbuf1);

  //n_array = 0;
  //  for(loop = 0; loop < 300; loop++) {
  //    program[loop] = backup[loop];
  //  }

  //printf("Answer : %i %i %i %i %i %i\n", maxoutput, maxstate[0], maxstate[1], maxstate[2], maxstate[3], maxstate[4]);
  return 0;
}
