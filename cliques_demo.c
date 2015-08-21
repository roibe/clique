
#include "cliques.h"

int main(int argc, char **argv)
{
  int action;
  int result;

  if(argc == 6)//if calculate |max clique|
    action = action_calculate_max_clique;
  else
    action = action_get_cliques;
  
  result = start(action, argc, argv);

  if(action == action_calculate_max_clique) printf("|Max_Clique| = %d\n", result);
   
  return 0;
}
