#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *cvt(char *input, int *level)
{
    char *cp = strtok(input, ",");
    if (cp == NULL) {
        /* No more separators */
        return (int *) malloc(sizeof(int) * *level);
    }

    int my_index = -1;
    int n;
    if (sscanf(cp, "%d", &n) == 1) {
        my_index = *level;
        *level += 1;
    } else {
        printf("Invalid integer token '%s'\n", cp);
    }
    int *array = cvt(NULL, level);
    if (my_index >= 0) {
        array[my_index] = n;
    }
    return array;
}

void opcode1(int *program, int pos1, int pos2, int pos3) {
  program[pos3] = program[pos1] + program[pos2];
}

void opcode2(int *program, int pos1, int pos2, int pos3) {
  program[pos3] = program[pos1] * program[pos2];
}

int opcode99() {
  return 0;
}


int main()
{

  FILE *ifp;
  char *mode = "r";
  char currentline[1000];

  ifp = fopen("input.list", mode);

  if (ifp == NULL) {
    fprintf(stderr, "Can't open input file in.list!\n");
    exit(1);
  }
  int *program = (int*) malloc(250* sizeof(int));
  fgets(currentline, sizeof(currentline), ifp);
  int n_array = 0;
  program = cvt(currentline, &n_array);
  
  int pc = 0;
  while(1) {
    if(program[pc] > 90) {
      break;
    }
    else if(program[pc] < 2 ) {
      opcode1(program, program[pc+1], program[pc+2], program[pc+3]);
    }
    else if(program[pc] > 1) {
      opcode2(program, program[pc+1], program[pc+2], program[pc+3]);
    }
    pc = pc + 4;
  }

  printf("Opcode 0: %i,", program[0]);
  return 0;
}
