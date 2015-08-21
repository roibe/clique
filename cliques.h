#ifndef _CLIQUES_H
#define _CLIQUES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

void print_title();
void init_parameters(int argc, char **argv);
void read_from_file(char** argv);
void clique_to_file(FILE* w_file, int** P, int i, int k);
void get_cliques();
void free_all();
void intersection(int* des ,int* src1, int* src2);//des = I[k]
void construct(int argc, char** argv);




#endif // _CLIQUES_H
