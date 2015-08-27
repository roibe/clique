
#include "cliques.h"

int main(int argc, char **argv)
{
  int calculate_max_clique = argc == 6;
  int result;

  result = start(argc, argv);

  if(calculate_max_clique) printf("|Max Clique| = %d\n", result);
   
  return 0;
}
