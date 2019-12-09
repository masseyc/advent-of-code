#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct{
  char *object[3];
  char *orbits[3];
} orbit;

static char *my_itoa_base_helper(char *dest, size_t size, int x, int base) {
  if (size == 0) return NULL;
  int digit = -(x%base);
  x /= base;
  if (x) {
    dest = my_itoa_base_helper(dest, size-1, x, base);
    if (dest == NULL) return NULL;
  }
  *dest = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[digit];
  return dest + 1;
}

char *my_itoa_base(char *dest, size_t size, int x, int base) {
  if (base < 2 || base > 36 || size < 2) return NULL;
  char *p = dest;
  if (x < 0) {
    *p++ = '-';
    size--;
  } else {
    x = -x;
  }
  p = my_itoa_base_helper(p, size-1, x, base);
  if (p == NULL) return NULL;
  *p = '\0';
  return dest;
}

int tobase36(char *str) {
  return (int)strtol(str, NULL, 36);

}

void frombase36(char *buff, int num) {
  
  my_itoa_base(buff, 4, num, 36);
  
}

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int main()
{

  const int elems = 250000;
  FILE *ifp;
  char *mode = "r";
  char currentline[100];
  int total = 0;
  int *orbits = (int*) calloc(elems, sizeof(int));
  if(orbits == NULL) {
  printf("malloc of size %d failed!\n", elems);   // could also call perror here
  exit(1);   // or return an error to caller
  }
  int parent;
  int child;

  ifp = fopen("input.list", mode);

  if (ifp == NULL) {
    fprintf(stderr, "Can't open input file in.list!\n");
    exit(1);
  }
  char **array;
  while (fgets(currentline, sizeof(currentline), ifp) != NULL) {
    array = str_split(currentline, ')');
    int child = tobase36(array[1]);
    
    int parent = tobase36(array[0]);
    orbits[child] = parent;
    //printf("%i \n",orbits[child]);
    
  }
  //printf("%i %i\n", orbits[11536], sizeof(orbits[0]));
  int j;
  for(int k = 0; k < elems; k++) {
    if(orbits[k] != 0) {
      j = k;
      while(j != 16438) {
        j = orbits[j];
        total += 1;
      }
    }
  }



  printf("Answer: %i\n", total);
  return 0;
}
