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

    float intervall[2];
    float *intervalls;

    float *results;
    float result = 0;

    int null_check = 1;
    int *null_checks;

    functions *func;
    initial_variable *vars;

    func =          (functions *) malloc(sizeof(functions));
    vars =          (initial_variable *) malloc(sizeof(initial_variable));

    intervalls =    (float *) calloc(2*p, sizeof(float));
    
    displacement =  (int *) calloc(p, sizeof(int));
    count =         (int *) calloc(p, sizeof(int));

    results =       (float *) calloc(p, sizeof(float));
    null_checks =   (int *) calloc(p, sizeof(int));
    
    int step = 1;
    int max_steps;

    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);    

    vars = get_parameters(argc, argv);

    max_steps = (int)(vars->x1-vars->x0)/vars->e;

    // Qui devo mettere il check degli intervalli e calcolare come dividerli per ogni processo infine fare una scatter o send
    if(rank == 0){

        /**
         * Check the entire system and create the split for all processes
         */
        if(vars->auto_choose) {

        } else {
            float partitioning = (vars->x1 - vars->x0) / p;

            for(int i = 0; i <= p; i++) {
                intervalls[i] = vars->x0 + i * partitioning;
                count[i] = 2;
                displacement[i] = i;
            }
        }
    }


    // MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatterv(intervalls, count, displacement, MPI_FLOAT, intervall, 2, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    vars->x0 = intervall[0];
    vars->x1 = intervall[1];    

    vars->xs = vars->x0;
	func->f0 = compute_function(vars);
    
    vars->xs = vars->x1;
	func->f1 = compute_function(vars);


    /**
     * Starting the program
     */

    for(int i=0; i < max_steps; i++) {
    
	    // vars->xs = vars->x0 - (vars->x0-vars->x1) * func->f0/(func->f0-func->f1);
        vars->xs = (vars->x0 * func->f1 - vars->x1 * func->f0)/(func->f1 - func->f0);
		func->fs = compute_function(vars);
		
		if(func->f0*func->fs < 0) {
			vars->x1 = vars->xs;
		    func->f1 = func->fs;
		} else {
			vars->x0 = vars->xs;
			func->f0 = func->fs;
		}
		step = step + 1;

        if(fabs(func->fs)<vars->e) {
            result = vars->xs;
            null_check = 0;
            break;
        }

    }

    MPI_Gather(&null_check, 1, MPI_INT, null_checks, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&result, 1, MPI_FLOAT, results, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);  

    if(rank == 0) {
        float *final_result;
        int *check_final_result;
        int max_roots; 
        
        max_roots = detect_max_roots(vars);
        printf("max roots found: %d\n", max_roots);

        final_result =          (float *) calloc(max_roots, sizeof(float));
        check_final_result =    (int *) calloc(max_roots, sizeof(int));
        
        int result_position = 0;
        int result_found = 0;

        for(int i = 0; i < p; i++) {
            if(!null_checks[i]) {
                if(i == 0) {
                    final_result[i] = results[i];
                    check_final_result[i] = 1;
                } else {
                    for(int k = 0; k < max_roots; k++){
                        if(abs(results[i] - final_result[k]) > vars->e && check_final_result[k]) {
                            result_found = 1;
                        }
                        if (!check_final_result[k]) {
                            result_position = k;
                            break;
                        }
                        
                    }
                    if(result_found) {
                        final_result[result_position] = results[i];
                        result_found = 0;
                    }
                }
            }
        }
        for(int i = 0; i < p; i++) {
            printf("trovati i seguenti root: %d ha trovato %f\n", i, results[i]);
        }

        puts("The system found roots at: ");
        for(int i = 0; i < max_roots; i++) {
            printf("%f\t", final_result[i]);
        }
        printf("\nwith an error of %f\n", vars->e);

        // free(final_result);
        free(check_final_result);
    }

    free(vars);
    free(func);
    free(null_checks);
    free(displacement);
    free(intervalls);
    MPI_Finalize();
    return 0;

}