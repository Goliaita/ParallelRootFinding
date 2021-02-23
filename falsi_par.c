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
    double *buff_res;

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

    results_found   = (int *) calloc(p, sizeof(int));
    
    int step = 1;
    int max_steps = 0;

    vars = get_parameters(argc, argv);

    max_steps = (int)(vars->x1 - vars->x0)/vars->p;    
    
    double max = get_max_num();
    

    if(rank == 0){

        /**
         * Check the entire system and create the split for all processes
         */
        if(vars->auto_choose) {
            vars->x1 = max;
            vars->x0 = -max;
            
            max_steps = (int)(vars->x1 - vars->x0)/vars->p;
        }

        double partitioning = (vars->x1 - vars->x0) / p;

        for(int i = 0; i <= p; i++) {
            intervalls[i] = vars->x0 + i * partitioning;
            count[i] = 2;
            displacement[i] = i;
        }
        
    }

    MPI_Barrier(MPI_COMM_WORLD);


    MPI_Bcast(&max_steps, 1, MPI_INT, 0, MPI_COMM_WORLD);


    MPI_Scatterv(intervalls, count, displacement, MPI_DOUBLE, intervall, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    vars->x0 = intervall[0];
    vars->x1 = intervall[1];  

    /**
     * Starting the computation
     */
    if(rank == 0) puts("\tStarting computation...\n");

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

    for(int i = 0; i < max_intervalls - 1; i++) {
        vars->x0 = intervalls[i];
        vars->x1 = intervalls[i+1];
        if(intervalls[i] > -0.5 && intervalls[i+1] < -0.2) printf("rank %d ha intervalli [%lf, %lf]\n", rank, intervalls[i], intervalls[i+1]);
        
        int steps = 0;

        compute_roots(max_steps, vars, &step, &results[i], &check_results[i], &steps);

    }

    if(rank == 0) puts("\t...End computation\n");

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
    
    buff_res = (double *) malloc(result_found * sizeof(double));

    int k = 0;
    for(int i = 0; i < max_intervalls - 1; i++) {
        if(check_results[i]) {
            buff_res[k] = results[i];
            k++;
        }
    }

    free(results);
    free(check_results);
    fflush(stdout);

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(&result_found, 1, MPI_INT, results_found, 1, MPI_INT, 0, MPI_COMM_WORLD);

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
    free(buff_res);
    free(count);


    if(rank == 0) {
        printf("\n\n");
        for(int i = 0; i < total_results; i++)
            printf("root found at %lf\n", collect_results[i]);

        printf("\nwith an error of %lf\n", vars->p);
    }

    free(collect_results);
    free(vars);

    return 0;

}


