#include <mpi.h>
#include "functions.h"

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

    int rank;
    int p;

    int *displacement;
    int *count;

    double intervall[2];
    double *intervalls;

    double *results;
    double *collect_results;

    int *check_results;
    int *results_found;

    int total_results = 0;

    
    initial_variable *vars;

    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);    

    vars            = (initial_variable *) malloc(sizeof(initial_variable));

    intervalls      = (double *) calloc(2*p, sizeof(double));
    
    displacement    = (int *) calloc(p, sizeof(int));
    count           = (int *) calloc(p, sizeof(int));

    // results =       (double *) calloc(p, sizeof(double));
    results_found   = (int *) calloc(p, sizeof(int));
    
    int step = 1;
    int max_steps;


    vars = get_parameters(argc, argv);

    max_steps = (int)(vars->x1 - vars->x0)/vars->fe;

    double max = get_max_num();

    // Qui devo mettere il check degli intervalli e calcolare come dividerli per ogni processo infine fare una scatter o send
    if(rank == 0){

        /**
         * Check the entire system and create the split for all processes
         */
        if(vars->auto_choose) {
            vars->x1 = max;
            vars->x0 = -max;
        }

        double partitioning = (vars->x1 - vars->x0) / p;

        for(int i = 0; i <= p; i++) {
            intervalls[i] = vars->x0 + i * partitioning;
            count[i] = 2;
            displacement[i] = i;
        }
        
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatterv(intervalls, count, displacement, MPI_DOUBLE, intervall, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    vars->x0 = intervall[0];
    vars->x1 = intervall[1];    

    /**
     * Starting the computation
     */
    if(rank == 0) puts("Starting computation");

    // TODO (vars->x1-vars->x0) * max
    double max_intervalls = (vars->x1-vars->x0) * max;
    // double max_intervalls = (vars->x1-vars->x0)/max;

    intervalls      = (double *) calloc(max_intervalls, sizeof(double));
    results         = (double *) calloc(max_intervalls - 1, sizeof(double));
    check_results   = (int *) calloc(max_intervalls - 1, sizeof(int));


    axis_partitioning(vars->x0, vars->x1, max_intervalls, intervalls);

    for(int i = 0; i < max_intervalls - 1; i++) {
        vars->x0 = intervalls[i];
        vars->x1 = intervalls[i+1];
        int steps = 0;
        compute_roots(max_steps, vars, &step, &results[i], &check_results[i], &steps);
        // if(check_results[i]) printf("rank: %d has done %d steps\n", rank, steps);
    }

    if(rank == 0) puts("End computation");

    /**
     * End computation
     */

    free(intervalls);

    /**
     * Start collecting data
     */

    int result_found = 0;
    for(int i = 0; i < max_intervalls - 1; i++) {
        result_found += check_results[i];
    }
    
    double *buff_res = (double *) malloc(result_found * sizeof(double));

    int k = 0;
    for(int i = 0; i < max_intervalls - 1; i++) {
        if(check_results[i]) {
            buff_res[k] = results[i];
            k++;
        }
    }

    for(int i = 0; i < result_found; i++) printf("%d trova: %lf\n", rank, buff_res[i]);

    free(results);
    free(check_results);
    fflush(stdout);

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(&result_found, 1, MPI_INT, results_found, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        
        for(int i = 0; i < p; i++) {
            total_results += results_found[i];
        }
        printf("trovati %d risultati\n", total_results);
        displacement[0] = 0;

        for(int i = 1; i < p; i++) {
            displacement[i] = displacement[i-1] + results_found[i-1];
        }
    }

    collect_results = (double *) calloc(total_results, sizeof(float));

    MPI_Barrier(MPI_COMM_WORLD); 

    MPI_Gatherv(buff_res, 1, MPI_DOUBLE, collect_results, results_found, displacement, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    free(displacement);
    free(buff_res);
    free(count);


    if(rank == 0) {
        for(int i = 0; i < total_results; i++)
            printf("trovati i seguenti root rank: %d ha trovato %lf\n", i, collect_results[i]);

        printf("with an error of %lf\n", vars->fe);

    }

    free(collect_results);
    free(vars);
    return 0;

}


