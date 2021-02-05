#include "functions.h"

#define f(x) pow(x,3)+2*pow(x,2)-5*x-2


int main (int argc, char* argv[]) {

    /**
     * Set initial variable
     */
    functions *func;
    initial_variable *vars;

    func = (functions *) malloc(sizeof(functions));
    vars = (initial_variable *) malloc(sizeof(initial_variable));
    
    int step = 1;
    int max_steps;


    /**
     * Get information by command line
     */

    vars = get_parameters(argc, argv);

    printf("Function found by main in: %s\n", vars->function);


    /**
     * Initialize remaining variables
     */
    max_steps = (int)(vars->x1-vars->x0)/vars->e;

    vars->xs = vars->x0;
	func->f0 = compute_function(vars);
    
    vars->xs = vars->x1;
	func->f1 = compute_function(vars);


    /**
     * Starting the program
     */
	printf("\nStep\t\tx0\t\tx1\t\tx2\t\tf(x2)\n");

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

        if(fabs(func->fs)<vars->e) break;

    }

	printf("%d\t\t%f\t%f\t%f\t%f\n",step, vars->x0, vars->x1, vars->xs, func->fs);
    /**
     * Print the result
     */
    printf("\nRoot is: %f\n", vars->xs);

    return 0;
}