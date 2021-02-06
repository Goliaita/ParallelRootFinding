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
    float x0;           // lower bound
    float x1;           // higher bound
    float e;            // error
    float xs;           // value of root
    int auto_choose;    // like a boolean
    int size;           // lenght of function 
    char *function;     // string for the function definition
} initial_variable;

typedef struct {
    float f0;           // value of the function at x0
    float f1;           // value of the function at x1
    float fs;           // value of the function at next step
} functions;


/**
 * Declaration of functions
 */
initial_variable *get_parameters(int argc, char* argv[]);
float compute_function(initial_variable *param);
int detect_max_roots(initial_variable *vars);


