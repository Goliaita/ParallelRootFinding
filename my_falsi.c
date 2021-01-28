#include "functions.h"

#define f(x) pow(x,2)+x-5


int main (int argc, char* argv[]) {

    /**
     * Set initial variable
     */
    funcions func;
    initial_variable vars;

    vars.x0 = -2;
    vars.x1 = 2;
    vars.e  = 0.001;

    float xs;
    int step = 1;
    int max_steps;


    /**
     * Get information by command line
     */

    vars = get_parameters(argc, argv);

    /**
     * Initialize remaining variables
     */
    max_steps = (int)(vars.x1-vars.x0)/vars.e;

    printf("steps are: %d", max_steps);

	func.f0 = f(vars.x0);
	func.f1 = f(vars.x1);


    /**
     * Starting the program
     */
	printf("\nStep\t\tx0\t\tx1\t\tx2\t\tf(x2)\n");

    for(int i=0; i < 300; i++){
    
	    xs = vars.x0 - (vars.x0-vars.x1) * func.f0/(func.f0-func.f1);
		func.fs = f(xs);
		printf("%d\t\t%f\t%f\t%f\t%f\n",step, vars.x0, vars.x1, xs, func.fs);
		
		if(func.f0*func.fs < 0){
			vars.x1 = xs;
		    func.f1 = func.fs;
		} else {
			vars.x0 = xs;
			func.f0 = func.fs;
		}
		step = step + 1;

        if(fabs(func.fs)<vars.e) break;

    }

    /**
     * Print the result
     */
    printf("\nRoot is: %f\n", xs);

    return 0;
}