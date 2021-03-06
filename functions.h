#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


/**
 * Function.h File
 * 
 * Created by Davide Basile student at University of Salento
 * 
 * Project related to Parallel Algorithms exam
 * 
 * This header file contains the main structure used in this program with the declaration of the main function used
 * 
 */

typedef struct {
    double x0;          // lower bound
    double x1;          // higher bound
    double p;           // precision of computation
    double xs;          // value of root
    int auto_choose;    // like a boolean
    // int size;        // lenght of function, used in old_function.c
    char *function;     // string in which you need to identify the function you want to use,
                        // in old_function.c is string for the function definition
} initial_variable;

/**
 * Function's declaration
 */
initial_variable get_parameters(int argc, char* argv[]);
double compute_function(initial_variable *param, double log_arg);
void compute_roots(int max_steps, initial_variable *vars, double *result, int *check_res);
void axis_partitioning(double min, double max, int interval, double *intervals);
double get_max_num();


