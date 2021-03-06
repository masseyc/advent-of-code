#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

 typedef struct {
   char direction[2];
   int distance;
 } vector;

typedef struct {
  int x;
  int y;
  int wire;
} pos;

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
int manhatten_distance(int x1, int x2, int y1, int y2) {

  return abs(x1-x2)+abs(y1-y2);
}

int main()
{

  char **tokens;
  char **tokens2;

  //char l2[] = "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7";
  //char l1[] = "R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51";

  //char l1[] = "R75,D30,R83,U83,L12,D49,R71,U7,L72";
  //char l2[] = "U62,R66,U55,R34,D71,R55,D58,R83";

  char l1[] = "R1000,D940,L143,D182,L877,D709,L253,U248,L301,U434,R841,U715,R701,U92,R284,U115,R223,U702,R969,U184,L992,U47,L183,U474,L437,D769,L71,U96,R14,U503,R144,U432,R948,U96,L118,D696,R684,U539,L47,D851,L943,U606,L109,D884,R157,U946,R75,U702,L414,U347,R98,D517,L963,D177,R467,D142,L845,U427,R357,D528,L836,D222,L328,U504,R237,U99,L192,D147,L544,D466,R765,U845,L267,D217,L138,U182,R226,U466,R785,U989,R55,D822,L101,U292,R78,U962,R918,U218,L619,D324,L467,U885,L658,U890,L764,D747,R369,D930,L264,D916,L696,U698,R143,U537,L922,U131,R141,D97,L76,D883,R75,D657,R859,U503,R399,U33,L510,D318,L455,U128,R146,D645,L147,D651,L388,D338,L998,U321,L982,U150,R123,U834,R913,D200,L455,D479,L38,U860,L471,U945,L946,D365,L377,U816,R988,D597,R181,D253,R744,U472,L345,U495,L187,D443,R924,D536,R847,U430,L145,D827,L152,D831,L886,D597,R699,D751,R638,D580,L488,D566,L717,D220,L965,D587,L638,D880,L475,D165,L899,U388,R326,D568,R940,U550,R788,D76,L189,D641,R629,D383,L272,D840,L441,D709,L424,U158,L831,D576,R96,D401,R425,U525,L378,D907,L645,U609,L336,D232,L259,D280,L523,U938,R190,D9,L284,U941,L254,D657,R572,U443,L850,U508,L742,D661,L977,U910,L190,U626,R140,U762,L673,U741,R317,D518,R111,U28,R598,D403,R465,D684,R79,U725,L556,U302,L367,U306,R632,D550,R89,D292,R561,D84,L923,D109,L865,D880,L387,D24,R99,U934,L41,U29,L225,D12,L818,U696,R652,U327,L69,D773,L618,U803,L433,D467,R840,D281,R161,D400,R266,D67,L205,D94,R551,U332,R938,D759,L437,D515,L480,U774,L373,U478,R963,D863,L735,U138,L580,U72,L770,U968,L594";
  char l2[] = "L990,D248,L833,U137,L556,U943,R599,U481,R963,U812,L825,U421,R998,D847,R377,D19,R588,D657,R197,D354,L548,U849,R30,D209,L745,U594,L168,U5,L357,D135,R94,D686,R965,U838,R192,U428,L861,U354,R653,U543,L633,D508,R655,U575,R709,D53,L801,D709,L92,U289,L466,D875,R75,D448,R576,D972,L77,U4,L267,D727,L3,D687,R743,D830,L803,D537,L180,U644,L204,U407,R866,U886,R560,D848,R507,U470,R38,D652,R806,D283,L836,D629,R347,D679,R609,D224,L131,D616,L687,U181,R539,D829,L598,D55,L806,U208,R886,U794,L268,D365,L145,U690,R50,D698,L140,D512,L551,U845,R351,U724,R405,D245,L324,U181,L824,U351,R223,D360,L687,D640,L653,U158,R786,D962,R931,D151,R939,D34,R610,U684,L694,D283,R402,D253,R388,D195,R732,U809,R246,D571,L820,U742,L507,U967,L886,D693,L273,U558,L914,D122,R146,U788,R83,U149,R241,U616,R326,U40,L192,D845,L577,U803,L668,D443,R705,D793,R443,D883,L715,U757,R767,D360,L289,D756,R696,D236,L525,U872,L332,U203,L152,D234,R559,U191,R340,U926,L746,D128,R867,D562,L100,U445,L489,D814,R921,D286,L378,D956,L36,D998,R158,D611,L493,U542,R932,U957,R55,D608,R790,D388,R414,U670,R845,D394,L572,D612,R842,U792,R959,U7,L285,U769,L410,D940,L319,D182,R42,D774,R758,D457,R10,U82,L861,D901,L310,D217,R644,U305,R92,U339,R252,U460,R609,D486,R553,D798,R809,U552,L183,D238,R138,D147,L343,D597,L670,U237,L878,U872,R789,U268,L97,D313,R22,U343,R907,D646,L36,D516,L808,U622,L927,D982,L810,D149,R390,U101,L565,U488,L588,U426,L386,U305,R503,U227,R969,U201,L698,D850,R800,D961,R387,U632,R543,D541,R750,D174,R543,D237,R487,D932,R220";



  tokens = str_split(l1,',');
  const int CIRCUIT_SIZE = 450000;
  const int LINE_SIZE = 35000;
  vector *line1 = malloc(LINE_SIZE * sizeof(vector));
  vector *line2 = malloc(LINE_SIZE * sizeof(vector));
  pos    *circuit = (pos*) malloc(CIRCUIT_SIZE * sizeof(pos));;
  pos    *circuit2 = (pos*) malloc(CIRCUIT_SIZE * sizeof(pos));;


//Initiilise structs
  for(int q = 0; q < CIRCUIT_SIZE;q++) {
    circuit[q].x = 0;
    circuit[q].y = 0;
    circuit[q].wire = 0;
  }

  for(int r = 0; r < LINE_SIZE;r++) {
    line1[r].distance = 0;
//    line1[r].direction = "X";
  }

  for(int s = 0; s < LINE_SIZE;s++) {
    line2[s].distance = 0;
//    line2[s].direction = "X";
  }

//parse put raw data to vectors, once for each line
  if(tokens) {
    int i;
    for (i = 0; *(tokens + i); i++)
    {
      //printf("vars=[%c]\n", *(tokens + i)[0]);
      memcpy(line1[i].direction, (tokens + i)[0], 1);
      char d1 = (tokens + i)[0][1];
      char d2 = (tokens + i)[0][2];
      char d3 = (tokens + i)[0][3];
      char d4 = (tokens + i)[0][4];
      char d5 = (tokens + i)[0][5];
      char num[6];
      num[0] = d1;
      num[1] = d2;
      num[2] = d3;
      num[3] = d4;
      num[4] = d5;
      num[5] = '\0';
      //printf("str: %i\n", atoi(num));
      line1[i].distance =  atoi(num);
      //printf("D: %s\n", line1[i].direction);
      //printf("I: %i\n", line1[i].distance);
      free(*(tokens + i));
    }
    
    //printf("\n");
    free(tokens);
  }

  tokens2 = str_split(l2,',');

  if(tokens2) {
    int i;
    for (i = 0; *(tokens2 + i); i++)
    {
      //printf("vars=[%c]\n", *(tokens2 + i)[0]);
      memcpy(line2[i].direction, (tokens2 + i)[0], 1);
      char d1 = (tokens2 + i)[0][1];
      char d2 = (tokens2 + i)[0][2];
      char d3 = (tokens2 + i)[0][3];
      char d4 = (tokens2 + i)[0][4];
      char d5 = (tokens2 + i)[0][5];
      char num[6];
      num[0] = d1;
      num[1] = d2;
      num[2] = d3;
      num[3] = d4;
      num[4] = d5;
      num[5] = '\0';
      //printf("str: %i\n", atoi(num));
      line2[i].distance =  atoi(num);
      //printf("D: %s\n", line2[i].direction);
      //printf("I: %i\n", line2[i].distance);
      free(*(tokens2 + i));
    }

    printf("\n");
    free(tokens2);
  }

 // printf("dir: %s\n",line1[0].direction);
//  char **grid = (char **)malloc(100000 * sizeof(char *));
//  for(int i = 0; i < 100000; i++) {
//    grid[i]= (char *)malloc(100000 * sizeof(char));
//  }
/*  if(!grid){
    printf("Ooops");
    exit(0);
  }*/
  int total = 0;
  int j;
  int gridx = 0;
  int gridy = 0;

// Calulate every point on the lines that are occupied

  int i = 0;
  for(j=0; j < LINE_SIZE; j++) {
    if(line1[j].direction[0] == 'U'){
      for(int t = 1; t <= line1[j].distance; t++) {
//        printf("stuff: %i %i %i\n", gridx, gridy, t);
        circuit[i].x = gridx;
        circuit[i].y = gridy+t;
        circuit[i].wire = 0;
//        printf("coords: %i %i\n", circuit[i].x, circuit[i].y);
        i++;
      }
      gridy = gridy+line1[j].distance;
    } else if(line1[j].direction[0] == 'D') {
      for(int t = 1; t <= line1[j].distance; t++) {
//        printf("stuff: %i %i %i\n", gridx, gridy, t);
        circuit[i].x = gridx;
        circuit[i].y = gridy-t;
        circuit[i].wire = 0;
        i++;
      }
      gridy = gridy-line1[j].distance;
    } else if(line1[j].direction[0] == 'L') {
      for(int t = 1; t <= line1[j].distance; t++) {
//        printf("stuff: %i %i %i\n", gridx, gridy, t);
        circuit[i].x = gridx-t;
        circuit[i].y = gridy;
        circuit[i].wire = 0;
        i++;
      }
      gridx = gridx-line1[j].distance;
    } else if(line1[j].direction[0] == 'R') {
      for(int t = 1; t <= line1[j].distance; t++) {
//        printf("stuff: %i %i %i\n", gridx, gridy, t);
        circuit[i].x = gridx+t;
        circuit[i].y = gridy;
        circuit[i].wire = 0;
        i++;


      }
      gridx = gridx+line1[j].distance;
      total += line1[j].distance;
    }
  }
  gridx = 0;
  gridy = 0;
  i = 0;
  for(j=0; j < LINE_SIZE; j++) {
    if(line2[j].direction[0] == 'U'){
      for(int t = 1; t <= line2[j].distance; t++) {
        circuit2[i].x = gridx;
        circuit2[i].y = gridy+t;
        circuit2[i].wire = 1;
        i++;
      }
      gridy = gridy+line2[j].distance;
    } else if(line2[j].direction[0] == 'D') {
      for(int t = 1; t <= line2[j].distance; t++) {
        circuit2[i].x = gridx;
        circuit2[i].y = gridy-t;
        circuit2[i].wire = 1;

        i++;
      }
      gridy = gridy-line2[j].distance;
    } else if(line2[j].direction[0] == 'L') {
      for(int t = 1; t <= line2[j].distance; t++) {
        circuit2[i].x = gridx-t;
        circuit2[i].y = gridy;
        circuit2[i].wire = 1;

        i++;

      }
      gridx = gridx-line2[j].distance;
    } else if(line2[j].direction[0] == 'R') {
      for(int t = 1; t <= line2[j].distance; t++) {
        circuit2[i].x = gridx+t;
        circuit2[i].y = gridy;
        circuit2[i].wire = 1;

        i++;
      }
      gridx = gridx+line2[j].distance;
      total += line2[j].distance;

    }
  }
  int k = 0;
  j = 0;
  int min = 2147483646;


int minx = 0;
int maxx = 0;
int miny = 0;
int maxy = 0;
int a = 0;
/*  int **grid = (int **)malloc(1000 * sizeof(int *));
  for(int i = 0; i < 1000; i++) {
    grid[i]= (int *)malloc(1000 * sizeof(int));
wing license key: TSUP-5E9F-5000-5D65-B889
    for(a = 0; a < 1000; a++) {
      grid[i][a] = 1;
    }
  }
  if(!grid){
    printf("Ooops");
    exit(0);
  }*/
for(int d = 0; d < CIRCUIT_SIZE; d++){
  if (circuit[d].x < minx){
    minx = circuit[d].x;
  }
  if(circuit[d].y < miny) {
    miny = circuit[d].y;
  }
  if (circuit[d].x > maxx){
    maxx = circuit[d].x;
  }
  if(circuit[d].y > maxy) {
    maxy = circuit[d].y;
  }
}

//uncomment ot print grid
/*
int check = 0;
for(int y = maxy+abs(miny)+1; y >= 0; y--) {
  for(int x = 0; x < maxx+abs(minx)+1; x++){
    for(int z = 0; z < CIRCUIT_SIZE; z++) {
      if(circuit[z].x+abs(minx) == x) {
        if(circuit[z].y+abs(miny) == y) {
          printf("%i", circuit[z].wire);
          check = 1;
          break;
        }
      } 

    }
    if(check == 0)
    printf(".");
    check = 0;
  }
  printf("\n");
}
*/

a = 0;
k = 0;
//while(a < 1000){
//  while(k < 1000){
//    printf("%i", grid[k][a]);
//  }
//  printf("\n");
//}

k = 0;
j = 0;
printf("TotalLength: %i\n", total);
printf("MinX: %i\n", minx);
printf("MaxX: %i\n", maxx);
printf("MinY: %i\n", miny);
printf("MaxY: %i\n", maxy);
int result = 10000000;
int minpath = 10000000;
// Search whole thing for minimum crossing point
  while(k < CIRCUIT_SIZE) {
    while(j < CIRCUIT_SIZE) {
      
     /* if(circuit[k].x == circuit2[j].x) {
        if(circuit[k].y == circuit2[j].y) {

           if(circuit[k].x+circuit[k].y < min) {
             if(circuit[k].x+circuit[k].y != 0) {
             if (circuit[k].wire == circuit[j].wire){
             } else {
             min = abs(circuit[k].x)+abs(circuit[k].y);
            printf("Indexes: %i %i\n", k,j);
            printf("coords: %i %i\n", circuit[k].x, circuit[k].y);

            printf("Total: %i\n", min);
             }
             }
           }
             if(circuit[k].x+circuit[k].y != 0) {
           //printf("coords: %i %i\n", circuit[k].x, circuit[k].y);

           //printf("Total: %i\n", abs(circuit[k].x)+abs(circuit[k].y));
          }
        }
      }*/

      int distance = manhatten_distance(circuit[k].x, circuit2[j].x,circuit[k].y, circuit2[j].y);
      if(distance == 0 && manhatten_distance(circuit[k].x,0,circuit[k].y,0) != 0) {
            min = abs(circuit[k].x)+abs(circuit[k].y);
            printf("Indexes: %i %i\n", k,j);
            printf("coords: %i %i\n", circuit[k].x, circuit[k].y);

            printf("Total: %i\n", min);
            printf("Path: %i\n", k+j);
            if(minpath > k+j)
              minpath = k+j;
            if(min < result)
            result = min;
            break;

      } else {
        j+=distance;
      }


      
     if(j>i){
       break;
     }
    }   
//        printf("\n");
  j = 0;
  if(k> i) {
    break;
  }
  k++;
  
  }
//           printf("coords: %i %i\n", circuit[k].x, circuit[k].y);

  printf("Final: %i\n", result);
  printf("MinPath: %i\n", minpath);

  return 0;
}
