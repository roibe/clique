
#include "cliques.h"
#define MAX_SIZE_FILE 500000

static int SIZE_FILE=0;
static int MAX_CLIQUE_POTENTIAL=0;
static int MAX_CLIQUE=0;
static int E=0;
static int V=0;

int**
read_from_file(char** argv, int** rows_clique_potential)
{
  FILE* f_buffer;
  int** R;
  char comma;
  int i, j, k;
  double value, indicator;
  clock_t t1 = clock();
  int n = atoi(argv[3]);
  int count=0;

  sscanf(argv[2], "%lf", &indicator);
  
  f_buffer = fopen(argv[1], "r");
  if(f_buffer == NULL) printf("not open r_file\n");

  do
    {
      fscanf(f_buffer, "%lf%c", &value, &comma);
      V++;
    }
  while(comma == ',');
 
  //if V == 0 !!!!!!

  *rows_clique_potential = (int*) malloc(sizeof(int)*(V+1));
    
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
	  if(j > i && value >= indicator) R[i][k++] = j;
	}
      E += k-1;
      R[i][0] = k-1; 
      if(k-1 >= n) (*rows_clique_potential)[++count] = i;
      R[i][k] = 0;
      R[i] = (int*) realloc(R[i], sizeof(int)*(k+1));
    }
  *(rows_clique_potential)[0] = count;
  *rows_clique_potential = (int*) realloc(*rows_clique_potential, sizeof(int)*(count+1));

  fclose(f_buffer);

  printf("done reading the graph! Graph: |V|=%d ,  |E|=%d\n", V, E);
  printf("Init Graph: %f  ms\n", (float)(clock()-t1)/1000);
  //printf("|E|= %d\n", E);
  
  return R;
}

void
clique_to_file(FILE* w_file, int** P, int i, int k)
{
  int j,x=0;
  while(*(P[k]+x)!=0)
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
get_cliques(int** R, int** P, int ** I, int argc, char** argv, int* rows_clique_potential)
{
  int n = atoi(argv[3]);
  int m = atoi(argv[4]);
  int i,k;
  int* last_ptr;
  FILE* w_file;
  double indicator;
  clock_t t1 = clock();
  int calculate_max_clique = argc == 6;
  
  sscanf(argv[2], "%lf", &indicator);
    
  //  MAX_CLIQUE_POTENTIAL = n;

  if(m > MAX_CLIQUE_POTENTIAL) m = MAX_CLIQUE_POTENTIAL;
  
  if(calculate_max_clique)
    {
      printf("Calculating max Clique size, this may take a while\n");
      n = 1;
      m = MAX_CLIQUE_POTENTIAL;
    }
  else
    {
      printf("Computing all cliques of size[%d,%d] based on %d edges graph, this may take a while\n", n, m, E);
      
      w_file = fopen("output.csv", "w");
      if(w_file == NULL) printf("not open w_file\n");

      fprintf(w_file, "All Cliques: file [min max] TH,test1.csv,%d, %d, %lf\n", n, m, indicator);
      fprintf(w_file,"index, edge, clique size, c0, c1, c2, c3, c4,  c5, c6, c7, c8, c9\n");
    }
  
  for(i=1; i <= rows_clique_potential[0]; i++)
    {printf("%d, \n",i);
      P[1] = &(R[rows_clique_potential[i]][1]);
      k = 1;
      while(1)
	{
	  last_ptr = P[k];
	  if(*(last_ptr) == 0 || k >= m-1)
	    {
	      if(k == 1) break;//from while
	      P[--k]++;
	      continue;
	    }
	  if(k + R[*last_ptr][0] < n-1)
	    {
	      P[k]++;
	      continue;
	    }
	  
	  intersection(I[k], last_ptr, R[*last_ptr] + 1);
	  if(k + I[k][0] < n-1 || I[k][0] == 0)//intersection is empty set
	    {
	      P[k]++;
	      continue;
	    }
	  //potential      
	  P[k+1] = &(I[k][1]);
	  k++;
	  if(k >= n-1)
	    {
	      if(calculate_max_clique)
		{
		  if(MAX_CLIQUE < k) n = MAX_CLIQUE = k;
		}
	      else
		{
		  clique_to_file(w_file, P, rows_clique_potential[i], k);//cliue...to file [i, *p[1], *p[2], ...] clique size = k
		  if(SIZE_FILE >= MAX_SIZE_FILE)//limit the file size
		    {
		      fclose(w_file);
		      printf("Alg: %lf  ms\n", (double) (clock()-t1)/1000);
		      printf("There are more then %d cliques of size[%d,%d] which is the limit output file size\n", MAX_SIZE_FILE, n, m);
		      return;
		    }
		}
	    }
	}//end-while
    }//end-for

  printf("Alg: %lf  ms\n", (double) (clock()-t1)/1000);
  
  if(calculate_max_clique) printf("|Max Clique|= %d\n", ++MAX_CLIQUE);
  else
    {
      fclose(w_file);
      printf("There are %d cliques of size[%d,%d]\n", SIZE_FILE, n, m);
    }
}

void
free_all(int** R, int** P, int** I, int* rows_clique_potential)
{
  int i;
  
  for(i=0; i <= V; i++)
    free(R[i]);
  free(R);

  for(i=0; i < MAX_CLIQUE_POTENTIAL; i++)
    free(I[i]);
  free(I);
  
  free(P);

  free(rows_clique_potential);
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
construct(int*** ad_R, int*** ad_P, int*** ad_I, char** argv, int** rows_clique_potential)
{
  int i;

  *(ad_R) = read_from_file(argv, rows_clique_potential);

  MAX_CLIQUE_POTENTIAL = (*(ad_R))[1][0];
  for(i=2; i < (*(ad_R))[0][0]; i++)
    if(MAX_CLIQUE_POTENTIAL < (*(ad_R))[i][0])
      MAX_CLIQUE_POTENTIAL = (*(ad_R))[i][0];
  
  *(ad_I) = (int**) malloc(sizeof(int*)*(MAX_CLIQUE_POTENTIAL+1));
  (*(ad_I))[0] = (int*) malloc(sizeof(int)*1);
  (*(ad_I))[0][0] = MAX_CLIQUE_POTENTIAL-1;
  for(i=1; i < MAX_CLIQUE_POTENTIAL; i++)
    (*(ad_I))[i] = (int*) malloc(sizeof(int)*(MAX_CLIQUE_POTENTIAL+2));

  *(ad_P) = (int**) malloc(sizeof(int*)*(MAX_CLIQUE_POTENTIAL+1));
}
