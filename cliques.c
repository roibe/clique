
/***Authors:*******
 *Roi Benita
 *Ariel Hazan
 *Daniel Blank
 *Netanel Ben-Haim
 *****************/
#include "cliques.h"
#define MAX_SIZE_FILE 500000

int EGDES, VERTICES, LOWER_BOUND, UPPER_BOUND, ORIGINAL_UPPER_BOUND;
double THRESHOLD_VALUE;
int **G, ***I, ***P, *V; //G for Graph, I for Intersection, P for Pointers, V for Vertices in clique potential
int SIZE_FILE, MAX_CLIQUE_POTENTIAL, MAX_CLIQUE, CALCULATE_MAX_CLIQUE;
FILE* pfile; //will point to cliques output file
int argc;
char **argv;
pthread_mutex_t mutex;
int THREAD;
functiontype thread_function;

//pthread_barrier_t barr;

int
start(int _argc, char **_argv)
{
  pthread_t *threads;
  int nthreads;
  int i;
  
  argc = _argc;
  argv = _argv;
  
  if( (nthreads = construct()) > 0 )
    {
      check_alloc( (threads = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads+1))) );
      for(i=1; i <= nthreads; i++)
	pthread_create(&(threads[i]), NULL, thread_function, NULL);
      
      for(i=1; i <= nthreads; i++)
	pthread_join(threads[i], NULL /* void ** return value could go here */);
    }
  free_all();
  if(nthreads) free(threads);

  if(CALCULATE_MAX_CLIQUE) return MAX_CLIQUE;
  else return SIZE_FILE;
}

int
construct()
{
  int i, j;
  
  init_parameters();

  for(i=0; argv[1][i+1] != '\0'; i++)
      ;
  if(argv[1][i] == 'v') read_from_csv_file();
  else read_from_dot_file();

  check_alloc( P = (int***) malloc(sizeof(int**)*(V[0]+1)) );
  check_alloc( I = (int***) malloc(sizeof(int**)*(V[0]+1)) );
  for(i=1; i <= V[0]; i++)
    {
      check_alloc( P[i] = (int**) malloc(sizeof(int*)*(G[V[i]][0]+1)) );
      check_alloc( I[i] = (int**) malloc(sizeof(int*)*(G[V[i]][0]+1)) );
      for(j=1; j <= G[V[i]][0]; j++)
	check_alloc( I[i][j] = (int*) malloc(sizeof(int)*(G[V[i]][0]+ 3 -j)) );
    }

  if(CALCULATE_MAX_CLIQUE) printf("Calculating max Clique size, this may take a while\n");
  else printf("Computing all cliques of size[%d,%d] based on %d edges graph, this may take a while\n", LOWER_BOUND, UPPER_BOUND, EGDES);
  
  if(UPPER_BOUND > MAX_CLIQUE_POTENTIAL) UPPER_BOUND = MAX_CLIQUE_POTENTIAL;

  //  pthread_barrier_init(&barr, NULL, V[0]);
    
  return V[0];
}

void
check_alloc(void* arg)
{
  if(!arg)
    {
      printf("Error, out of space\n");
      exit(1);
    }
}

void
init_parameters()
{
  MAX_CLIQUE_POTENTIAL = 1;
  MAX_CLIQUE = 2;
  EGDES = 0;
  VERTICES = 0;
  THREAD = 0;
  sscanf(argv[2], "%lf", &THRESHOLD_VALUE);
  LOWER_BOUND = atoi(argv[3]);
  ORIGINAL_UPPER_BOUND = UPPER_BOUND = atoi(argv[4]);
  thread_function = &get_max_clique;
  if(!(CALCULATE_MAX_CLIQUE = argc == 6))
    {
      thread_function = &get_cliques;
      
      if( !(pfile = fopen("output.csv", "w")) ){ printf("Error, cannot open output file");exit(1);}
      
      fprintf(pfile, "All Cliques: file [min max] TH,%s ,%d, %d, %lf\n", argv[1], LOWER_BOUND, UPPER_BOUND, THRESHOLD_VALUE);
      fprintf(pfile, "index, edge, clique size, c0, c1, c2, c3, c4,  c5, c6, c7, c8, c9\n");
    }
}

void
read_from_csv_file()
{
  FILE* r_pfile;
  char comma;
  int i, j, k;
  double value;
  clock_t t1;
  int V_size=0;

  t1 = clock();

  if( !(r_pfile = fopen(argv[1], "r")) ){ printf("Error, cannot open input file\n");exit(1);}

  do
    {
      fscanf(r_pfile, "%lf%c", &value, &comma);
      VERTICES++;
    }
  while(comma == ',');
 
  //if Vertices == 1 !!!!!!

  check_alloc( V = (int*) malloc(sizeof(int)*(VERTICES+1)) );
    
  check_alloc( G = (int**) malloc(sizeof(int*)*(VERTICES+1)) );
  
  check_alloc( G[VERTICES] = (int*) malloc(sizeof(int)*2) );
  G[VERTICES][0] = G[VERTICES][1] =  0;

  rewind(r_pfile);
    
  for(i=1; i < VERTICES; i++)
    {
      check_alloc( G[i] = (int*) malloc(sizeof(int)*(VERTICES+1)) );
      for(j=1,k=1; j <= VERTICES; j++)
	{
	  fscanf(r_pfile, "%lf%c", &value, &comma); 
	  if(j > i && value >= THRESHOLD_VALUE) G[i][k++] = j;
	}
      G[i][0] = k-1;
      G[i][k] = 0;

      EGDES += G[i][0];

      if(MAX_CLIQUE_POTENTIAL < G[i][0]+1) MAX_CLIQUE_POTENTIAL = G[i][0]+1;
      if(CALCULATE_MAX_CLIQUE && G[i][0] > 1) V[++V_size] = i;
      else if(G[i][0]+1 >= LOWER_BOUND) V[++V_size] = i;

      check_alloc( G[i] = (int*) realloc(G[i], sizeof(int)*(G[i][0] +2)) );
    }
  
  V[0] = V_size;
  check_alloc( V = (int*) realloc(V, sizeof(int)*(V_size+1)) );
  
  fclose(r_pfile);

  printf("done reading the graph! Graph: |V|=%d ,  |E|=%d\n", VERTICES, EGDES);
  printf("Init Graph: %f  ms\n", (float)(clock()-t1)/1000);
  printf("|E|= %d\n", EGDES);
}

void
*get_max_clique()
{
  int **P_t;
  int **I_t;
  int* last_ptr;
  int i, k;
  
  pthread_mutex_lock(&mutex);    
  i = ++THREAD;
  pthread_mutex_unlock(&mutex);
  
  //  printf("thread %d ready\n",i);
  //  pthread_barrier_wait(&barr);
  //  printf("thread %d start\n",i);

  P_t = P[i];
  I_t = I[i];

  if(G[V[i]][0] < MAX_CLIQUE) return NULL;

  for(k=0; k <= G[V[i]][0] +1 ; k++)
     I_t[1][k] = G[V[i]][k];

  P_t[0] = &(V[i]);

  P_t[1] = &(I_t[1][1]);
    
  k = 1;
  while(1)
    {	  	
      last_ptr = P_t[k];
      if(k + I_t[k][0]  <= MAX_CLIQUE)
	{//not potential
	  if(k == 1) break;
	  P_t[--k]++;
	  I_t[k][0]--;
	  continue;
	}
      if(k + G[*last_ptr][0] < MAX_CLIQUE)
	{//not potential
	  P_t[k]++;
	  I_t[k][0]--;
	  continue;
	}
      if(intersection(I_t[++k], last_ptr, G[*last_ptr] +1))
	{//potential
	  P_t[k] = &(I_t[k][1]);
	  
	  pthread_mutex_lock(&mutex);    
	  if(MAX_CLIQUE < k+1) MAX_CLIQUE = k+1; 
	  pthread_mutex_unlock(&mutex);
	}
    }
  //  printf("thread %d end\n",i);
  return NULL;
}  

void 
*get_cliques()
{
  int **P_t;
  int **I_t;
  int* last_ptr;
  int i, k;
  int upper_bound = UPPER_BOUND;

  pthread_mutex_lock(&mutex);    
  i = ++THREAD;
  pthread_mutex_unlock(&mutex);
  
  //  printf("thread %d ready\n",i);
  //  pthread_barrier_wait(&barr);
  //  printf("thread %d start\n",i);
  
  if(upper_bound > G[V[i]][0]+1) upper_bound = G[V[i]][0]+1;
  
  P_t = P[i];
  I_t = I[i];
  
  for(k=0; k <= G[V[i]][0] +1 ; k++)
     I_t[1][k] = G[V[i]][k];
 	
  P_t[0] = &(V[i]);

  P_t[1] = &(I_t[1][1]);
    
  k = 1;

  while(1)
    {
      last_ptr = P_t[k];
      
      if(*(last_ptr) == 0 || k + I_t[k][0] < LOWER_BOUND || k+1 == upper_bound)
	{//not potential
	    if(k == 1) break;
	    P_t[--k]++;
	    I_t[k][0]--;
	    continue;
	}
      if(G[*last_ptr][0] == 0 || k+1 + G[*last_ptr][0] < LOWER_BOUND)
	{//not potential  
	  P_t[k]++;
	  I_t[k][0]--;
	  continue;
	}
      if(! intersection(I_t[++k], last_ptr, G[*last_ptr] +1)) //empty set
	{//not potential
	  P_t[--k]++;
	  I_t[k][0]--;
	  continue;
	}	
      P_t[k] = &(I_t[k][1]);

      if(k+1 >= LOWER_BOUND)
	{	 
	  if(SIZE_FILE < MAX_SIZE_FILE)//limit the file size
	    {
	      pthread_mutex_lock(&mutex);    
	      write_clique_to_file(P_t, k);//cliue...to file [*p[0], *p[1], *p[2], ...] clique size = k+1
	      pthread_mutex_unlock(&mutex);
	    }
	  else break;	  
	}
    }
  //  printf("thread %d end\n",i);
  return NULL;
}

int
intersection(int* des, const int* src1, const int* src2)
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
  des[0] = k-1;
  if(k==1)
    {
      des[0] = 0;
      return 0;
    }
  return 1;
}

void 
write_clique_to_file(int **P, int k)
{
  int j,x=0;
  while(*(P[k]+x)!=0)
    {
      fprintf(pfile, "%d, %d, ", SIZE_FILE, k+1);
      for(j=0; j < k; j++)
	fprintf(pfile, "%d,", *(P[j]));
      fprintf(pfile, "%d,\n", *(P[k]+x));
      SIZE_FILE++;
      x++;
    }
}

void
print_results()
{
  if(! CALCULATE_MAX_CLIQUE)
    {
      fprintf(pfile,"%c", '\0');
      fclose(pfile);
      if(SIZE_FILE < MAX_SIZE_FILE)
	printf("There are %d cliques of size[%d,%d] with threshold value %lf\n", SIZE_FILE, LOWER_BOUND, ORIGINAL_UPPER_BOUND, THRESHOLD_VALUE);
      else
	printf("There are more then %d cliques of size[%d,%d] with threshold value %lf (which is the limit output file size, see src file)\n", MAX_SIZE_FILE, LOWER_BOUND, ORIGINAL_UPPER_BOUND, THRESHOLD_VALUE);
    }
  else
    printf("|Max_Clique| = %d\n", MAX_CLIQUE);
}
 
void
free_all()
{
  int i,j;

  print_results();

  if(V[0])
    {
      for(i=1; i <= V[0]; i++)
	{
	  for(j=1; j <= G[V[i]][0]; j++)
	    free(I[i][j]);
	  free(I[i]);
	  free(P[i]);
	}
    }     
  free(I);
  free(P);
  free(V);
  
  for(i=1; i <= VERTICES; i++)
    free(G[i]);
  free(G);  
}

void
read_from_dot_file()
{
  FILE* r_pfile;
  char c;
  int i, j, k;
  double value;
  clock_t t1;
  int V_size=0;

  t1 = clock();

  if( !(r_pfile = fopen(argv[1], "r")) ){ printf("Error, cannot open input file\n");exit(1);}

  //title
  do
    {
      fscanf(r_pfile, "%c", &c);
    }
  while(c != '\n');
  //count vertices
  do
    {
      fscanf(r_pfile, "%d", &i);

      VERTICES++;
      
      do fscanf(r_pfile, "%c", &c);
      while(c != '\n');
    }
  while(i == 1);
  
  //if V == 0 !!!!!!

  check_alloc( V = (int*) malloc(sizeof(int)*(VERTICES+1)) );
    
  check_alloc( G = (int**) malloc(sizeof(int*)*(VERTICES+1)) );
  
  check_alloc( G[VERTICES] = (int*) malloc(sizeof(int)*2) );
  G[VERTICES][0] = G[VERTICES][1] =  0;

  rewind(r_pfile);

  //title
  do
    {
      fscanf(r_pfile, "%c", &c);
    }
  while(c != '\n');
  //count vertices    

  for(i=1; i < VERTICES; i++)
    {
      check_alloc( G[i] = (int*) malloc(sizeof(int)*(VERTICES+2 -i)) );
      for(j = i+1,k=1; j <= VERTICES; j++)
	{
	  do fscanf(r_pfile, "%c", &c);
	  while(c != '=');
	  
	  fscanf(r_pfile, "%lf", &value);
	  if(value >= THRESHOLD_VALUE) G[i][k++] = j;

	  do fscanf(r_pfile, "%c", &c);
	  while(c != '\n');
	}
      G[i][0] = k-1;
      G[i][k] = 0;

      EGDES += G[i][0];

      if(MAX_CLIQUE_POTENTIAL < G[i][0]+1) MAX_CLIQUE_POTENTIAL = G[i][0]+1;
      if(CALCULATE_MAX_CLIQUE && G[i][0] > 1) V[++V_size] = i;
      else if(G[i][0]+1 >= LOWER_BOUND) V[++V_size] = i;

      check_alloc( G[i] = (int*) realloc(G[i], sizeof(int)*(G[i][0] +2)) );
    }
  
  V[0] = V_size;
  check_alloc( V = (int*) realloc(V, sizeof(int)*(V_size+1)) );
  
  fclose(r_pfile);

  printf("done reading the graph! Graph: |V|=%d ,  |E|=%d\n", VERTICES, EGDES);
  printf("Init Graph: %f  ms\n", (float)(clock()-t1)/1000);
  printf("|E|= %d\n", EGDES);
}

