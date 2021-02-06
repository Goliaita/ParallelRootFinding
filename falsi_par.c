#include <mpi.h>
#include "functions.h"

#define BLOCK_LOW(id, p, n)      ((id)*((n)/(p)))
#define BLOCK_HIGH(id, p, n)    (BLOCK_LOW((id)+1, p, n) - 1)
#define BLOCK_SIZE(id, p, n)    (BLOCK_HIGH(id, p, n) - BLOCK_LOW(id, p, n) + 1)
#define BLOCK_OWNER(index, p, n)  (((p)*(index) + 1) -1) /(n))

int main(int argc, char* argv[]) {


    MPI_Init(&argc, &argv);

    int rank;
    int p;

    int size = 0;

    int n = 10;
    int vector_size;

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
            printf("partiamo da %f e arriviamo a %f\n", vars->x0, vars->x1);
            for(int i = 0; i <= p; i++) {
                intervalls[i] = vars->x0 + i * partitioning;
                count[i] = 2;
                displacement[i] = i;
            }
        }

        results = (float *) calloc(p, sizeof(float));
        null_checks = (int *) calloc(p, sizeof(int));
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
	// printf("\nStep\t\tx0\t\tx1\t\tx2\t\tf(x2)\n");

    for(int i=0; i < max_steps; i++) {
    
	    vars->xs = vars->x0 - (vars->x0-vars->x1) * func->f0/(func->f0-func->f1);
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
        int max_roots = detect_max_roots(vars);
        // TODO trovare quando deve essere grande

        final_result =          (float *) calloc(max_roots, sizeof(float));
        check_final_result =    (int *) calloc(max_roots, sizeof(int));
        
        float buff_result = 0;
        int result_found = 0;

        for(int i = 0; i < p; i++) {
            if(!null_checks[i]) {
                if(i == 0) {
                    final_result[i] = results[i];
                    check_final_result[i] = 1;
                } else {
                    for(int k = 0; k < max_roots; k++){
                        if(abs(results[i] - final_result[k]) > vars->e && check_final_result[k]) {
                            buff_result = results[i];
                        }
                        if (!check_final_result[k]) {
                            result_found = k;
                            break;
                        }
                        
                    }
                    if(result_found != 0) {
                        final_result[result_found] = results[i];
                        result_found = 0;
                    }
                }
            }
        }

        for(int i = 0; i < max_roots; i++) {
                printf("rank: %d ha dato come risultato %f\n", i, final_result[i]);
        }
    }

    // free(vars);
    free(func);
    MPI_Finalize();
    return 0;

}