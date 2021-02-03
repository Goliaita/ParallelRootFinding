#include <mpi.h>
#include "functions.h"



int main(int argc, char* argv[]) {


    MPI_Init(&argc, &argv);

    int rank;
    int p;

    int size = 0;

    int n = 10;
    int vector_size;

    int *vector = NULL;
    int *recvbuff;


    functions *func;
    initial_variable *vars;

    func = (functions *) malloc(sizeof(functions));
    vars = (initial_variable *) malloc(sizeof(initial_variable));
    
    int step = 1;
    int max_steps;

    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype data_type;
    


    int offsets[3] = {0};
    int oldtypes[3] = {0};
    int datacounts[3] = {0};


    vars = get_parameters(argc, argv);

    max_steps = (int)(vars->x1-vars->x0)/vars->e;



    // Get the number of processes

    
    if(rank == 0){

        /**
         * Check the entire system and create the split for all processes
         */



    }

    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);


    MPI_Barrier(MPI_COMM_WORLD);
    
    
    fflush(stdout);

    free(vars);
    free(func);
    MPI_Finalize();
    return 0;

}