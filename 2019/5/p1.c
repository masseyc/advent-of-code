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
  char opcodestr[5];
  itoa(opcode, opcodestr);

  if(nDigits == 2) {
    return 0;
  } else if(nDigits == 4 && opcodestr[strlen(opcodestr)-param-2] == '1') {
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

void writevalue(int *program, int pc, int value, int param) {
  int mode = getMode(program, pc, param);
  if(mode == 1 ) {

  } else {
    program[program[pc+param]] = value;
  }
}

void opcode1(int *program, int pc) {
  int value = getValue(program, pc, 1) + getValue(program, pc, 2);
  writevalue(program, pc , value, 3);
  
}

void opcode2(int *program, int pc) {
  int value = getValue(program, pc, 1) * getValue(program, pc, 2);
  writevalue(program, pc , value, 3);}

void opcode3(int *program, int input, int par1) {
  program[par1] = input;
}

void opcode4(int *program, int pc ) {
  printf("Output: %i\n", getValue(program, pc, 1));
}

int opcode99() {
  return 0;
}

int execute(int *program, int pc, int input) {
  while(1) {
    int opcode = getOpcode(program, pc);
    if(pc > 300){
      break;
    }
    if(opcode == 99) {
      break;
    }
    else if(opcode == 1 ) {

      opcode1(program, pc);

    }
    else if(opcode  == 2) {   

      opcode2(program, pc);

    } else if(opcode == 3) {
      opcode3(program, input, program[pc+2]);

    } else if(opcode == 4) {
      opcode4(program, pc);

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


  int output = execute(program, pc, input);

  n_array = 0;
  for(loop = 0; loop < 300; loop++) {
    program[loop] = backup[loop];
  }

  printf("Answer : %i\n", output);
  return 0;
}
