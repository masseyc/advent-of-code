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

void opcode3(int *program, int input, int par1) {
  program[par1] = input;
}

void opcode4(int *program, int par1 ) {
  printf("%i\n", program[par1]);
}

int opcode99() {
  return 0;
}

int execute(int *program, int pc, int input) {
  while(1) {
    if(pc > 300){
      break;
    }
    if(program[pc] == 99) {
      break;
    }
    else if(program[pc] == 1 ) {

      opcode1(program, program[pc+1], program[pc+2], program[pc+3]);

    }
    else if(program[pc]  == 2) {   

      opcode2(program, program[pc+1], program[pc+2], program[pc+3]);

    } else if(program[pc] == 3) {
      opcode3(program, input, program[pc+2]);
    }

    pc = pc + 4;
  }
  return program[0];
}


int main(int argc, char *argv[])
{
  int input;
  if (argc > 1) {
    input = (int) strtol(argv[1], (char **)NULL, 10);
  } else {
//    exit(1);
  }
  FILE *ifp;
  char *mode = "r";
  char currentline[1000];

  ifp = fopen("input.list", mode);

  if (ifp == NULL) {
    fprintf(stderr, "Can't open input file in.list!\n");
    exit(1);
  }
  int *program = (int*) malloc(250000000* sizeof(int));
  int *backup = (int*) malloc(250000000* sizeof(int));


  fgets(currentline, sizeof(currentline), ifp);
  fclose(ifp);
  int n_array = 0;

  int pc = 0;
  int noun;
  int verb;
  int finalnoun = 0;
  int finalverb = 0;
//  int result = 0;
  program = cvt(currentline, &n_array);
  int loop;
  for(loop = 0; loop < sizeof(currentline); loop++) {
    backup[loop] = program[loop];
  }

//  for(noun = 0; noun < 100; noun++) {
//    for(verb = 0; verb < 100;verb++) {
//      program[1] = noun;
//      program[2] = verb;
/*
  while(1) {
    if(pc > 300){
      break;
    }
    if(program[pc] == 99) {
      break;
    }
    else if(program[pc] == 1 ) {

      opcode1(program, program[pc+1], program[pc+2], program[pc+3]);

    }
    else if(program[pc]  == 2) {   

        opcode2(program, program[pc+1], program[pc+2], program[pc+3]);

    }

    pc = pc + 4;
  
      }
      pc = 0;


//      printf("Result : %i\n", program[0]);
*/

  int output = execute(program, pc, input);

      n_array = 0;
      for(loop = 0; loop < 300; loop++) {
        program[loop] = backup[loop];
      }
//    }
//  }

  printf("Answer : %i,", output);
  return 0;
}
