#include "functions.h"
#include <time.h>

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
    initial_variable vars;

    FILE *fo;
    
    int max_steps;

    double *intervalls, *results;
    int *check_results;

    double max = get_max_num();

    double time = -clock();


    /**
     * Get information by command line
     */

    vars = get_parameters(argc, argv);


    /**
     * Initialize remaining variables
     */
    if(vars.auto_choose) {
        vars.x1 = max;
        vars.x0 = -max;
    }

    max_steps = (int)(vars.x1 - vars.x0)/vars.p;

    int max_intervalls;

    if(max >= 1){
        max_intervalls = (vars.x1-vars.x0) * max;
    } else {
        max_intervalls = (vars.x1-vars.x0) / max;
    }


    intervalls      = (double *) calloc(max_intervalls,     sizeof(double));
    results         = (double *) calloc(max_intervalls - 1, sizeof(double));
    check_results   = (int *)    calloc(max_intervalls - 1, sizeof(int));


    axis_partitioning(vars.x0, vars.x1, max_intervalls, intervalls);

    /**
     * Starting the computation
     */
    
    for(int i = 0; i < max_intervalls - 1; i++) {
        vars.x0 = intervalls[i];
        vars.x1 = intervalls[i+1];

        compute_roots(max_steps, &vars, &results[i], &check_results[i]);
    }

    time = (time + clock()) / CLOCKS_PER_SEC;


    /**
     * Ending computation
     */

    if((fo = fopen("./results_seq.txt", "w+")) == NULL) {
        perror("Error at opening file");
    }

    /**
     * Printing results
     */
    int results_found = 0;
    for(int i = 0; i < max_intervalls; i++) {
        if(check_results[i]) {
            fprintf(fo, "%s %lf\n", "roots found at:", results[i]);
            results_found++;
        }
    }

    printf("\n\n\t found %d roots\n", results_found);
    printf("Computation time: %lf", time);

    return 0;

}