#include "cliques.h"
#define MAX_SIZE_FILE 100000

static int E, V, N, M;
static double INDICATOR;
static int **R, **I1, **I2, **P1, **P2, *V_IN_CLIQUE_POTENTIAL;
static int SIZE_FILE, MAX_CLIQUE_POTENTIAL, MAX_CLIQUE, CALCULATE_MAX_CLIQUE;

void
construct(int argc, char **argv)
{
  int i;
  
  init_parameters(argc, argv);

  read_from_file(argv);

  P1 = (int**) malloc(sizeof(int*)*(MAX_CLIQUE_POTENTIAL+1));
  P2 = (int**) malloc(sizeof(int*)*(MAX_CLIQUE_POTENTIAL+1));
  
  I1 = (int**) malloc(sizeof(int*)*(MAX_CLIQUE_POTENTIAL+1));
  I2 = (int**) malloc(sizeof(int*)*(MAX_CLIQUE_POTENTIAL+1));
  I1[0] = (int*) malloc(sizeof(int)*1);
  I2[0] = (int*) malloc(sizeof(int)*1);
  I1[0][0] = I2[0][0] = MAX_CLIQUE_POTENTIAL-1;
  for(i=1; i < MAX_CLIQUE_POTENTIAL; i++)
    {
      I1[i] = (int*) malloc(sizeof(int)*(MAX_CLIQUE_POTENTIAL+2));
      I2[i] = (int*) malloc(sizeof(int)*(MAX_CLIQUE_POTENTIAL+2));
    }
  print_title();
}

void
init_parameters(int argc, char **argv)
{
  MAX_CLIQUE_POTENTIAL = 0;
  MAX_CLIQUE = 0;
  E = 0;
  V = 0;
  sscanf(argv[2], "%lf", &INDICATOR );
  N = atoi(argv[3]);
  M = atoi(argv[4]);
  CALCULATE_MAX_CLIQUE = argc == 6;
}

void
read_from_file(char** argv)
{
  FILE* f_buffer;
  char comma;
  int i, j, k;
  double value;
  clock_t t1 = clock();
  int count=0;

  //  int** arr;
  
  f_buffer = fopen(argv[1], "r");
  if(f_buffer == NULL) printf("not open r_file\n");

  do
    {
      fscanf(f_buffer, "%lf%c", &value, &comma);
      V++;
    }
  while(comma == ',');
 
  //if V == 0 !!!!!!

  V_IN_CLIQUE_POTENTIAL = (int*) malloc(sizeof(int)*(V+1));
    
  R = (int**) malloc(sizeof(int*)*(V+1));
  
  R[0] = (int*) malloc(sizeof(int)*1);
  R[0][0] = V;
  R[V] = (int*) malloc(sizeof(int)*2);
  R[V][0] = R[V][1] =  0;

  rewind(f_buffer);
    
  for(i=1; i < V; i++)
    {
      R[i] = (int*) malloc(sizeof(int)*(V+1));
      for(j=1,k=1; j <= V; j++)
	{
	  fscanf(f_buffer, "%lf%c", &value, &comma); 
	  if(j > i && value >= INDICATOR) R[i][k++] = j;
	}
      E += k-1;
      R[i][0] = k-1;
      if(MAX_CLIQUE_POTENTIAL < k-1) MAX_CLIQUE_POTENTIAL = k-1;
      if(k-1 >= N) V_IN_CLIQUE_POTENTIAL[++count] = i;
      R[i][k] = 0;
      R[i] = (int*) realloc(R[i], sizeof(int)*(k+1));
    }
  V_IN_CLIQUE_POTENTIAL[0] = count;
  V_IN_CLIQUE_POTENTIAL = (int*) realloc(V_IN_CLIQUE_POTENTIAL, sizeof(int)*(count+1));
  printf("%d\n", count);
  //  for(i=0; i < count+1; i++)printf("%d, ",  V_IN_CLIQUE_POTENTIAL[i]);
  
  fclose(f_buffer);

  printf("done reading the graph! Graph: |V|=%d ,  |E|=%d\n", V, E);
  printf("Init Graph: %f  ms\n", (float)(clock()-t1)/1000);
  //printf("|E|= %d\n", E);

}

void
print_title()
{
  if(M > MAX_CLIQUE_POTENTIAL) M = MAX_CLIQUE_POTENTIAL;
  
  if(CALCULATE_MAX_CLIQUE)
    {
      printf("Calculating max Clique size, this may take a while\n");
      N = 1;
      M = MAX_CLIQUE_POTENTIAL;
    }
  else
    {
      printf("Computing all cliques of size[%d,%d] based on %d edges graph, this may take a while\n", N, M, E);

      //     fprintf(w_file, "All Cliques: file [min max] TH,test1.csv,%d, %d, %lf\n", n, m, indicator);
      //fprintf(w_file,"index, edge, clique size, c0, c1, c2, c3, c4,  c5, c6, c7, c8, c9\n");
    }
} 
  
void
get_cliques(int thread)
{
  FILE* w_file;
  int **P;
  int **I;
  int* last_ptr;
  int i=1, k, end;
  //clock_t t1 = clock();

  //  printf("thread %d start\n",thread);
  end = V_IN_CLIQUE_POTENTIAL[0];
  
  if(thread == 1)
    {
      w_file = fopen("output1.csv", "w");
      P = P1;
      I = I1;
      //i = 1;
      //end = V_IN_CLIQUE_POTENTIAL[0] / 2;
      if(V_IN_CLIQUE_POTENTIAL[0] %2 == 1) end++;
   }   
   else
    {
      w_file = fopen("output2.csv", "w");
      P = P2;
      I = I2;
      //i = V_IN_CLIQUE_POTENTIAL[0] / 2  + 1;
      //    end = V_IN_CLIQUE_POTENTIAL[0] / 2;
      i=2;
    }
    
  //  MAX_CLIQUE_POTENTIAL = n
 
  for( ; i <= end; i+=2)
    {//printf("%d, \n",i);
      //      if(CALCULATE_MAX_CLIQUE && R[V_IN_CLIQUE_POTENTIAL[i]][0] < MAX_CLIQUE) continue;
	
      P[1] = &(R[V_IN_CLIQUE_POTENTIAL[i]][1]);
      k = 1;
      while(1)
	{    
	  last_ptr = P[k];
	  if(*(last_ptr) == 0 || k >= M-1)
	    {
	      if(k == 1) break;//from while
	      P[--k]++;
	      continue;
	    }
	  if(k + R[*last_ptr][0] < N-1)
	    {
	      P[k]++;
	      continue;
	    }
	  intersection(I[k], last_ptr, R[*last_ptr] + 1);  
	  if(k + I[k][0] < N-1 || I[k][0] == 0)//intersection is empty set
	    {
	      P[k]++;
	      continue;
	    }
	  //potential      
	  P[k+1] = &(I[k][1]);
	  k++;  
	  if(k >= N-1)
	    {  
	      if(CALCULATE_MAX_CLIQUE)
		{
		  if(MAX_CLIQUE < k) N = MAX_CLIQUE = k;
		}
	      else
		{  
		  clique_to_file(w_file, P, V_IN_CLIQUE_POTENTIAL[i], k);//cliue...to file [i, *p[1], *p[2], ...] clique size = k
		  if(SIZE_FILE >= MAX_SIZE_FILE)//limit the file size
		    {
		      fclose(w_file);
		      //    printf("Alg: %lf  ms\n", (double) (clock()-t1)/1000);
		      printf("There are more then %d cliques of size[%d,%d] which is the limit output file size\n", MAX_SIZE_FILE, N, M);
		      return;
		    }
		}
	    }  
	}//end-while
    }//end-for

  //printf("Alg: %lf  ms\n", (double) (clock()-t1)/1000);
  
  //  if(!CALCULATE_MAX_CLIQUE)
  // {
      fclose(w_file);
      //  printf("There are %d cliques of size[%d,%d]\n", SIZE_FILE, N, M);
      // }

      
      // printf("thread %d end work: %f  ms\n",thread, (float)(clock()-t1)/1000);
}
   
void
intersection(int* des, int* src1, int* src2)//const
{
  int i=0,j=0,k=1;
 
  while(src1[i]!=0 && src2[j]!=0)
    {
      if(src1[i] == src2[j])
	{
	  des[k++] = src1[i];
	  i++;j++;
	}
      else if(src1[i] < src2[j]) i++;
      else j++;
    }
  des[k] = 0;
  des[0] = k;
  if(k==1) des[0] = 0;
}

void 
clique_to_file(FILE* w_file, int **P, int i, int k)
{
  int j,x=0;
  while(*(P[k]+x)!=0 && (SIZE_FILE < MAX_SIZE_FILE))
    {
      fprintf(w_file, "%d, %d, ", SIZE_FILE, k+1);
      fprintf(w_file, "%d,", i-1);
      for(j=1; j < k; j++)
	fprintf(w_file, "%d,", *(P[j]) -1);
      fprintf(w_file, "%d,\n", *(P[k]+x) -1);
      SIZE_FILE++;
      x++;
    }
}

void
free_all()
{  if(CALCULATE_MAX_CLIQUE) printf("|Max Clique|= %d\n", ++MAX_CLIQUE);
  int i;
  
  for(i=0; i <= V; i++)
      free(R[i]);
  free(R);

  for(i=0; i < MAX_CLIQUE_POTENTIAL; i++)
    {
      free(I1[i]);
      free(I2[i]);
    }
  free(I1);
  free(I2);
  
  free(P1);
  free(P2);

  free(V_IN_CLIQUE_POTENTIAL);
}

