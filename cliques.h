#ifndef _CLIQUES_H
#define _CLIQUES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef void *(*functiontype)();

int start(int argc, char **argv); // called from main function, starting the program using multi-threads

int construct();

void init_parameters();

void check_alloc(void* arg);

void read_from_csv_file();

void read_from_dot_file();

void *get_cliques(); // get cliques in size [lower bound , upper bound]

void *get_max_clique();

int intersection(int* des, const int* src1, const int* src2); // return 0 if [src1 intersection src2] is an empty set, else return 1

void write_clique_to_file(int** P, int k);

void print_results(); // also close cliques output file

void free_all();



#endif // _CLIQUES_H
