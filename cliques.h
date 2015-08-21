#ifndef _CLIQUES_H
#define _CLIQUES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef enum {
  action_calculate_max_clique = 1,
  action_get_cliques = 0,
}Action;

int construct(int argc, char** argv);

void check_alloc(void* arg);

void init_parameters(int argc, char **argv);

void read_from_csv_file(char** argv);

void read_from_dot_file(char** argv);

void get_cliques();

void get_max_clique();

int intersection(int* des ,const int* src1,const int* src2);//des = I[k]

void clique_to_file(int** P, int k);

void print_results();

void free_all();

void *thread_function(void *arg);

int start(int get_max_clique, int argc, char **argv);



#endif // _CLIQUES_H
