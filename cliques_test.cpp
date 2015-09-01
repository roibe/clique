extern "C"
{
#include "cliques.h"
}
#include "gtest/gtest.h"

int CALCULATE_MAX_CLIQUE, GET_CLIQUES;
int RESULT;
int VERTICES= 1728;
int INDICATOR;
int CONNECT= 1;
int DISCONNECT= 0;

int
main(int argc, char** argv)
{
  CALCULATE_MAX_CLIQUE = argc == 6;
  GET_CLIQUES = (! CALCULATE_MAX_CLIQUE);
  
  RESULT = start(argc, argv);

  INDICATOR = atoi(argv[2]);
  
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(clique, get_max_clique)
{
  if(CALCULATE_MAX_CLIQUE)
    ASSERT_EQ(RESULT, 8);
}

TEST(clique, get_cliques)
{
  if(GET_CLIQUES)
    ASSERT_EQ(RESULT, 438);
}


TEST(clique, check_cliques)
{
  if(GET_CLIQUES)
    {
      FILE* pfile;
      char c=' ';
      int V=0;
      
      ASSERT_TRUE(pfile = fopen("output.csv", "r"));
      
      while(c != '\0')
      	{
	  do fscanf(pfile,"%c", &c);
	  while(c != '\n' && c != '\0');
	  V++;
	}

      V -= 3;
      
      ASSERT_EQ(V, 438);

      ASSERT_TRUE(!fclose(pfile));
    }
}

/*
TEST(clique, check_clique)
{
  if(ACTION == action_get_cliques)
    {
      int G[1729][1729];
      // int C[6];
      FILE *input_file;
      FILE *output_file;
      int i, j;
      double input_value;
      //int output_value;
      char c;
      
      ASSERT_TRUE(output_file = fopen("output.csv", "r"));
      ASSERT_TRUE(input_file = fopen("gtest_test1.csv", "r"));
      
      for(i=1; i <= VERTICES; i++)
	{
	  for(j=1; j <= VERTICES; j++)
	    {
	      fscanf(input_file,"%lf%c", &input_value, &c);
	      if(input_value >= INDICATOR) G[i][j] = CONNECT;
	      else G[i][j] = DISCONNECT;
	      printf("%d,",G[i][j]);
	    }
	  printf("\n");
	}
  
      for(i=0; i < 2; i++)
	do fscanf(output_file,"%c", &c);
	while(c != '\n');
      fscanf(output_file,"%d%c", &output_value, &c);
      fscanf(output_file,"%d%c", &output_value, &c);

      for(i=0; i < 5; i++)
	fscanf(output_file,"%d%c", &C[i], &c);

      ASSERT_EQ(G[C[0]][C[1]], 1);

  
      fclose(input_file);
      fclose(output_file);
    }	
}
*/
