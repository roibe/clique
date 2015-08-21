#ifndef _CLIQUES_H
#define _CLIQUES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** read_from_file(char** argv, int** rows_clique_potential);//return R
void clique_to_file(FILE* w_file, int** P, int i, int k);
void get_cliques(int** R, int** P, int** I, int argc, char** argv, int* rows_clique_potential);
void free_all(int** R, int** P, int** I, int* rows_clique_potential);
void intersection(int* des ,int* src1, int* src2);//des = I[k]
void construct(int*** ad_R, int*** ad_P, int*** ad_I, char** argv, int** rows_clique_potential);




#endif // _CLIQUES_H
