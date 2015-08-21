#include "cliques.h"

int main(int argc, char** argv)
{
  int **R, **P, **I, *rows_clique_potential;
  clock_t t1 = clock();
  construct(&R, &P, &I, argv, &rows_clique_potential);
  
  get_cliques(R, P, I, argc, argv, rows_clique_potential);

  free_all(R, P, I, rows_clique_potential);
  printf("total: %lf  ms\n", (double) (clock()-t1)/1000);  
  return 0;
}
