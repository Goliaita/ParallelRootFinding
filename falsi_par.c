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


    double time;

    /**
     * MPI variables
     */
    int rank;
    int p;

    /**
     * Gather and Scatter support variables
     */
    int *displacement = NULL;
    int *count = NULL;

    /**
     * Interval variables where we will look for a solution
     */
    double interval[2];
    double *intervals = NULL;

    /**
     *  Results buffer variables
     */
    double *results = NULL;
    double *collect_results = NULL;
    double *buff_res = NULL;

    /**
     * Counting and checking variables for solutions
     */
    int *check_results = NULL;
    int *results_found = NULL;
    int total_results = 0;

    FILE *fo = NULL;
    
    initial_variable vars;
    
    int max_steps = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);    

    time = -MPI_Wtime();

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
  
    double max = get_max_num();   

    /**
     * Check the entire system and create the split for all processes
     */
    if(vars.auto_choose) {
        vars.x1 = max;
        vars.x0 = -max;
    } 

    /**
     * Some rank 0 stuff
     */
    if(rank == 0){
        /**
         * Opening file to store results
         */
        // if((fo = fopen("./results.txt", "w+")) == NULL) {
        //     perror("fileopen");
        // }
        // fprintf(fo, "%s", "\n");

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
     * Scattering the interval to all process
     */
    MPI_Scatterv(intervals, count, displacement, MPI_DOUBLE, interval, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    vars.x0 = interval[0];
    vars.x1 = interval[1];  


    /**
     * Computing the steps we will do at most
     */
    max_steps = (int)(vars.x1 - vars.x0)/vars.p;  

    printf("\nMax step computized are: %d\n", max_steps);

    int max_intervals;

    if(max >= 1){
        max_intervals = (vars.x1-vars.x0) * max;
    } else {
        max_intervals = (vars.x1-vars.x0) / max;
    }

    printf("\nMax intervals of x axis are: %d\n", max_intervals);

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

    intervals = NULL;

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

    results = NULL;
    check_results = NULL;

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

        collect_results = (double *) calloc(total_results, sizeof(double));
    }

    MPI_Gatherv(buff_res, result_found, MPI_DOUBLE, collect_results, results_found, displacement, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    free(displacement);
    free(count);
    free(results_found);

    displacement = NULL;
    count = NULL;
    results_found = NULL;

    time += MPI_Wtime();
 
    /**
     * Print results
     */
    if(rank == 0) {
        printf("\n");
        
        if(total_results != 0) {

            if((fo = fopen("./results.txt", "w+")) == NULL) {
                perror("fileopen");
            }

            for(int k = 0; k < total_results; k++){
                fprintf(fo, "%lf\n", collect_results[k]);
            }
        } else {
            puts("\tNo roots found\n");
        }

        printf("Total compute time is:%lf\n", time);
        
        free(buff_res);
        free(collect_results);

        buff_res = NULL;
        collect_results = NULL;
    }


    MPI_Finalize();

    return 0;

}


