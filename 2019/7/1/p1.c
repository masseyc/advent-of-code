#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// prepend "0" as needed resulting in a string of _minimal_ width.
void PrependZeros(char *dest, const char *src, unsigned minimal_width) {
  size_t len = strlen(src);
  size_t zeros = (len > minimal_width) ? 0 : minimal_width - len;
  memset(dest, '0', zeros);
  strcpy(dest + zeros, src);
}

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

void reverse(char s[])
{
  int i, j;
  char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}

 void itoa(int n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
} 

int getOpcode(int *program, int pc) {
  int opcode = program[pc];
  char tmp[5];
  char opcodestr[5];
  itoa(opcode, tmp);
  PrependZeros(opcodestr, tmp, 5);
  char opstr[3];
  opstr[0] = opcodestr[strlen(opcodestr)-2];
  opstr[1] = opcodestr[strlen(opcodestr)-1];
  opstr[2] = '\0';
  return atoi(opstr);
}





int getMode(int *program, int pc, int param) {
  int opcode = program[pc];
  int nDigits = floor(log10(abs(opcode))) + 1;
  char tmp[5];
  char opcodestr[5];
  itoa(opcode, tmp);
  PrependZeros(opcodestr, tmp, 5);
  if(nDigits == 2) {
    return 0;
  } else if(opcodestr[3-param] == '1') {
    return 1;
  } else
  {
    return 0;
  }
  
}

int getValue(int *program, int pc, int param) {
  int mode = getMode(program, pc, param);
  int opcode = program[pc];
  if(mode == 0) {
    return program[program[pc+param]];
  } else {
    return program[pc+param];
  }
  
}

void printProgram(int *program) {
  for(int i = 0; i < 100; i++) {
    printf("%i, ", program[i]);
  }
  printf("\n");
}

int writevalue(int *program, int pc, int value, int param) {
  int mode = getMode(program, pc, param);
  if(mode == 1 ) {
    printf("Write Abort: Mode 1\n");
    return 1;
  } else {
    program[program[pc+param]] = value;
    return 0;
  }
}

void opcode1(int *program, int pc) {
  int value = getValue(program, pc, 1) + getValue(program, pc, 2);
  writevalue(program, pc , value, 3);
  //printf("val: %i\n", program[program[pc+3]]);
  
}

void opcode2(int *program, int pc) {
  int value = getValue(program, pc, 1) * getValue(program, pc, 2);
  writevalue(program, pc , value, 3);}

void opcode3(int *program, int input, int par1) {
  program[par1] = input;
}

int opcode4(int *program, int pc ) {
  int val = getValue(program, pc, 1);
  //printf("Output: %i\n", val);
  return val;

}

int opcode5(int *program, int pc) {
  if (getValue(program, pc, 1) == 0) {
    return pc+3;
  } else {
    return getValue(program, pc, 2);
  }
}

int opcode6(int *program, int pc) {
  if (getValue(program, pc, 1) == 0) {
    return getValue(program, pc, 2);
  } else {
    return pc+3;
    
  }
}

void opcode7(int *program, int pc) {
  if(getValue(program, pc, 1) < getValue(program, pc, 2)) {
    program[program[pc+3]] = 1;
  } else {
    program[program[pc+3]] = 0;
  }
}

void opcode8(int *program, int pc) {
  if(getValue(program, pc, 1) == getValue(program, pc, 2)) {
    program[program[pc+3]] = 1;
  } else {
    program[program[pc+3]] = 0;
  }
}

int execute(int *program, int pc, int state, int input) {
  int output;
  while(1) {
  //  printf("%i\n", pc);
    int opcode = getOpcode(program, pc);
    
    //printProgram(program);
    //printf("PC: %i\n Val: %i\n", pc, program[pc]);
    //int tmp = program[pc+2];
    //printf("%i \n", tmp);
    //printf("opcode: %i %i %i %i %i \n", pc, program[pc], program[pc+1], getValue(program, pc, 2), program[pc+3]);

    if(pc > 3000){
      printf("Program Count Exception!\n");
      break;
    }
    if(opcode == 99) {
      //printf("OpCode 99 Exit!!!\n");
      break;
    }
    else if(opcode == 1 ) {
      //addition
      opcode1(program, pc);
      pc = pc + 4;
    }
    else if(opcode  == 2) {   
      //multiply
      opcode2(program, pc);
      pc = pc + 4;

    } else if(opcode == 3) {

      opcode3(program, state, program[pc+1]);
      state = input;
      pc = pc + 2;

    } else if(opcode == 4) {
      output = opcode4(program, pc);
      pc = pc + 2;

    } else if(opcode == 5) {

      pc = opcode5(program, pc);

    } else if(opcode == 6) {

      pc = opcode6(program, pc);

    } else if(opcode == 7) {

      opcode7(program, pc);
      pc = pc + 4;

    } else if(opcode == 8) {

      opcode8(program, pc);
      pc = pc + 4;

    } else {
      printf("%i \n", program[274]);
      printf("Error: %i %i %i %i %i \n", pc, program[pc], program[pc+1], getValue(program, pc, 2), program[pc+3]);
      break;
    }


  }
  return output;
}


int main(int argc, char *argv[])
{
  const int memalloc = 5000;
  int input;
  if (argc > 1) {
    input = (int) strtol(argv[1], (char **)NULL, 10);
  } else {
//    exit(1);
  }
  FILE *ifp;
  char *mode = "r";
  char currentline[4000];

  ifp = fopen("input.list", mode);

  if (ifp == NULL) {
    fprintf(stderr, "Can't open input file in.list!\n");
    exit(1);
  }
  int *program = (int*) calloc(memalloc,sizeof(int));
  int *backup = (int*) calloc(memalloc,sizeof(int));
  int *runtime1 = (int*) calloc(memalloc,sizeof(int));
  int *runtime2 = (int*) calloc(memalloc,sizeof(int));
  int *runtime3 = (int*) calloc(memalloc,sizeof(int));
  int *runtime4 = (int*) calloc(memalloc,sizeof(int));
  int *runtime5 = (int*) calloc(memalloc,sizeof(int));

  if(program == NULL) {
  printf("malloc of size %d failed!\n", memalloc);   // could also call perror here
  exit(1);   // or return an error to caller
  }
  if(backup == NULL) {
  printf("malloc of size %d failed!\n", memalloc);   // could also call perror here
  exit(1);   // or return an error to caller
  }
  if(runtime1 == NULL) {
  printf("malloc of size %d failed!\n", memalloc);   // could also call perror here
  exit(1);   // or return an error to caller
  }
  if(runtime2 == NULL) {
  printf("malloc of size %d failed!\n", memalloc);   // could also call perror here
  exit(1);   // or return an error to caller
  }
  if(runtime3 == NULL) {
  printf("malloc of size %d failed!\n", memalloc);   // could also call perror here
  exit(1);   // or return an error to caller
  }
  if(runtime4 == NULL) {
  printf("malloc of size %d failed!\n", memalloc);   // could also call perror here
  exit(1);   // or return an error to caller
  }
  if(runtime5 == NULL) {
  printf("malloc of size %d failed!\n", memalloc);   // could also call perror here
  exit(1);   // or return an error to caller
  }



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
  for(loop = 0; loop < memalloc; loop++) {
  // printf("loop: %i\n", loop);
  //  printf("%i \n", program[loop]);
    int mp = program[loop];
    backup[loop] = mp;
    runtime1[loop] = program[loop];
    runtime2[loop] = program[loop];
    runtime3[loop] = program[loop];
    runtime4[loop] = program[loop];
    runtime5[loop] = program[loop];
  }
 // program[0] = 101;
 // printf("%i %i",getMode(program, pc, 1), getMode(program, pc, 2));
 // exit(0);
  int maxstate[5];
  int output;
  int maxoutput = 0;
 /* int i = 4;
  int j = 3;
  int k = 2;
  int l = 1;
  int m = 0;*/
  for(int i = 0; i<5;i++) {
    for(int j = 0; j<5;j++) {
      for(int k = 0; k<5;k++) {
        for(int l = 0; l<5;l++) {
          for(int m = 0; m<5;m++) {
            if(i != j && i != k && i != l && i != m) {
              if(j != k && j != l && j != m) {
                if(k != l && k != m) {
                  if(l != m) {
                    
            input = 0;
            pc = 0;
            output = execute(runtime1, pc, i, input);
            pc = 0;
            output = execute(runtime2, pc, j, output);
            pc = 0;
            output = execute(runtime3, pc, k, output);
            pc = 0;
            output = execute(runtime4, pc, l, output);
            pc = 0;
            output = execute(runtime5, pc, m, output);
            
            if(output > maxoutput) {
              maxoutput = output;
              maxstate[0] = i;
              maxstate[1] = j;
              maxstate[2] = k;
              maxstate[3] = l;
              maxstate[4] = m;
              //printf("MAXIMUM: %i %i %i %i %i %i\n", output, i, j, k, l, m);
            }
            }}}}
            output = 0;

  }}}}}
  n_array = 0;
  for(loop = 0; loop < 300; loop++) {
    program[loop] = backup[loop];
  }

  printf("Answer : %i %i %i %i %i %i\n", maxoutput, maxstate[0],maxstate[1],maxstate[2],maxstate[3],maxstate[4]);
  return 0;
}
