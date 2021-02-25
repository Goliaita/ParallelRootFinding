#include <mpi.h>
#include "functions.h"
#include <errno.h>

/**
 * Falsi_Par.c File
 * 
 * Created by Davide Basile student at University of Salento
 * 
 * Project related to Parallel Algorithms exam
 * 
 * This file contain the parallel version about the root finding problem that is solved with 
 * the falsi method which compute the function (x0f1 - x1f0) / (f1 - f0)
 * 
 */


int main(int argc, char* argv[]) {


    MPI_Init(&argc, &argv);


    /**
     * MPI variables
     */
    int rank;
    int p;

    /**
     * Gather and Scatter support variables
     */
    int *displacement;
    int *count;

    /**
     * Interval variables where we will look for a solution
     */
    double interval[2];
    double *intervals;

    /**
     *  Results buffer variables
     */
    double *results;
    double *collect_results;
    double *buff_res;

    /**
     * Counting and checking variables for solutions
     */
    int *check_results;
    int *results_found;
    int total_results = 0;

    FILE *fo;
    
    initial_variable vars;
    
    int max_steps = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);    

    /**
     * Initializing variables
     */
    intervals       = (double *) 	calloc(2 * p,	sizeof(double));
    displacement    = (int *) 		calloc(p,		sizeof(int));
    count           = (int *) 		calloc(p, 		sizeof(int));
    results_found   = (int *) 		calloc(p, 		sizeof(int));
    
    /**
     * Getting data from command line
     */
    vars = get_parameters(argc, argv);

    /**
     * Computing the steps we will do at most
     */
    max_steps = (int)(vars.x1 - vars.x0)/vars.p;    
    double max = get_max_num();   

    /**
     * Check the entire system and create the split for all processes
     */
    if(vars.auto_choose) {
        vars.x1 = max;
        vars.x0 = -max;
        
        max_steps = (int)(vars.x1 - vars.x0)/vars.p;
    } 

    /**
     * Some rank 0 stuff
     */
    if(rank == 0){
        /**
         * Opening file to store results
         */
        if((fo = fopen("./results.txt", "w+")) == NULL) {
            perror("fileopen");
        }
        fprintf(fo, "%s", "\n");

        /**
         * Partitioning the entire array to processes and then preparing
         * variables to scatter the intervalls to all processes
         */
        double partitioning = (vars.x1 - vars.x0) / p;

        for(int i = 0; i <= p; i++) {
            intervals[i] = vars.x0 + i * partitioning;
            count[i] = 2;
            displacement[i] = i;
        }
        
    }


    /**
     * Let all process wait the process 0 then scatter the interval to all process
     */
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Scatterv(intervals, count, displacement, MPI_DOUBLE, interval, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    vars.x0 = interval[0];
    vars.x1 = interval[1];  



    int max_intervals;

    if(max >= 1){
        max_intervals = (vars.x1-vars.x0) * max;
    } else {
        max_intervals = (vars.x1-vars.x0) / max;
    }

    /**
     * Set up remaining variables
     */
    intervals       = (double *) realloc(intervals,        max_intervals * sizeof(double));
    results         = (double *) calloc(max_intervals - 1, sizeof(double));
    check_results   = (int *)    calloc(max_intervals - 1, sizeof(int));

    axis_partitioning(vars.x0, vars.x1, max_intervals, intervals);

    /**
     * Starting the computation
     */
    if(rank == 0) puts("\tStarting computation...\n");

    for(int i = 0; i < max_intervals - 1; i++) {
        vars.x0 = intervals[i];
        vars.x1 = intervals[i+1];        

        compute_roots(max_steps, &vars, &results[i], &check_results[i]);
    }

    if(rank == 0) puts("\t...End computation\n");

    /**
     * End computation
     */

    free(intervals);

    /**
     * Start collecting data
     */
    int result_found = 0;
    for(int i = 0; i < max_intervals - 1; i++) {
        result_found += check_results[i];
    }
    
    buff_res = (double *) malloc(result_found * sizeof(double));

    int k = 0;

    /**
     * Free results from empty set
     */
    for(int i = 0; i < max_intervals - 1; i++) {
        if(check_results[i]) {
            buff_res[k] = results[i];
            k++;
        }
    }

    free(results);
    free(check_results);

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(&result_found, 1, MPI_INT, results_found, 1, MPI_INT, 0, MPI_COMM_WORLD);

    /**
     * After Gathered how much results have to be saved rank 0 compute the displacement
     * and get the data from all processes 
     */
    if(rank == 0) {
        
        displacement[0] = 0;

        for(int i = 0; i < p; i++) {
            total_results += results_found[i];
            displacement[i+1] = displacement[i] + results_found[i];
        }
        
        printf("%d results found\n", total_results);
    }

    MPI_Barrier(MPI_COMM_WORLD); 

    collect_results = (double *) calloc(total_results, sizeof(float));

    MPI_Gatherv(buff_res, result_found, MPI_DOUBLE, collect_results, results_found, displacement, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    free(displacement);
    free(count);
    free(results_found);
    MPI_Barrier(MPI_COMM_WORLD);
 
    /**
     * Print results
     */
    if(rank == 0) {
        printf("\n");

        // if((fo = fopen("./results.txt", "w+")) == NULL) perror("Unable to open file");

        if(total_results != 0) {
            for(int k = 0; k < total_results; k++){
                fprintf(fo, "%s %s %s %lf\n","root", "found", "at", collect_results[k]);
            }
        } else {
            puts("\tNo roots found\n");
        }

        
        free(buff_res);
    }


    free(collect_results);
    MPI_Finalize();

    return 0;

}


