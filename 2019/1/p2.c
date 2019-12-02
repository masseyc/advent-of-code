#include <stdio.h>
#include <stdlib.h>

int calcfuel(int mass) {
 int totalfuel = 0;
 if (mass < 6) {
   return 0;
 }
 int fuel = 0;
 int fuel1 = mass / 3;
 fuel = fuel1 - 2;
 totalfuel += fuel;
 totalfuel += calcfuel(fuel); 

 return totalfuel;
}

int main()
{

  FILE *ifp;
  char *mode = "r";
  char currentline[100];
  int total = 0;
  int mass = 0;

  ifp = fopen("input.list", mode);

  if (ifp == NULL) {
    fprintf(stderr, "Can't open input file in.list!\n");
    exit(1);
  }
  while (fgets(currentline, sizeof(currentline), ifp) != NULL) {
    mass = atoi(currentline);
    total = total + calcfuel(mass);
  }

  printf("Answer: %i\n", total);
  return 0;
}

