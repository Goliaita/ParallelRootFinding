#include "functions.h"

/**
 * Falsi_seq.c File
 * 
 * Created by Davide Basile student at University of Salento
 * 
 * Project related to Parallel Algorithms exam
 * 
 * This file contain the sequential version of the falsi method is needed to compare the sequential version 
 * to the parallel one
 * 
 */

int main (int argc, char* argv[]) {

    /**
     * Set initial variable
     */
    initial_variable *vars;

    FILE *fo;
    
    int step = 1;
    int max_steps;

    double *intervalls, *results;
    int *check_results;

    double max = get_max_num();

    vars = (initial_variable *) malloc(sizeof(initial_variable));

    /**
     * Get information by command line
     */

    vars = get_parameters(argc, argv);


    /**
     * Initialize remaining variables
     */
    if(vars->auto_choose) {
        vars->x1 = max;
        vars->x0 = -max;
        
        max_steps = (int)(vars->x1 - vars->x0)/vars->p;
    }


    int max_intervalls;

    if(max >= 1){
        max_intervalls = (vars->x1-vars->x0) * max;
    } else {
        max_intervalls = (vars->x1-vars->x0) / max;
    }


    intervalls      = (double *) calloc(max_intervalls,     sizeof(double));
    results         = (double *) calloc(max_intervalls - 1, sizeof(double));
    check_results   = (int *)    calloc(max_intervalls - 1, sizeof(int));


    axis_partitioning(vars->x0, vars->x1, max_intervalls, intervalls);

    /**
     * Starting the program
     */
    
    for(int i = 0; i < max_intervalls - 1; i++) {
        vars->x0 = intervalls[i];
        vars->x1 = intervalls[i+1];
        
        int steps = 0;

        compute_roots(max_steps, vars, &step, &results[i], &check_results[i], &steps);

    }

    if((fo = fopen("./results_seq.txt", "w+")) == NULL) {
        perror("Error at opening file");
    }

    for(int i = 0; i < max_intervalls; i++) {
        if(check_results[i]) fprintf(fo, "%s %lf\n", "roots found at:", results[i]);
    }

    return 0;
}