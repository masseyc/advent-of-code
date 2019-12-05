#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

 /* itoa:  convert n to characters in s */
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

int onedoubleonly(char *num) {
  int i = 1;


}

int checkdouble(char *num) {
  int check = 0;
  int check2 = 0;
  int lastnum =0;
    for(int i = 0; i < 5; i++) {
        
        if((int)num[i]-'0' == (int)num[i+1]-'0') {

          if(check == 1&& lastnum == (int)num[i]-'0') {
            check = 0;
          }
          if((int)num[i]-'0' != (int)num[i+2]-'0') {
            if((int)num[i]-'0' != (int)num[i-1]-'0') {
            check = 1;
            lastnum = (int)num[i]-'0';
            }
          }

        }

        
    }
    
    if(check == 1) {
      printf("%s\n", num);
      return 1;
    }
    return 0;
}


int main()
{
  int total = 0;
  char num[6];
  for (int i = 372037; i < 905157; i++) {
    itoa(i, num);
    //printf("%s", num);
    int x = 0;
    for(int j = 0; j < 5; j++) {
       if((int)num[j]-'0' <= (int)num[j+1]-'0') {
         x++;
         if(x == 5) {
           total += checkdouble(num);
         }
       }
    }
  }

  printf("Answer: %i\n", total);
  return 0;
}
