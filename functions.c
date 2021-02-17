#include "functions.h"



/**
 * Declaration of functions
 */
double f_exp(double var);
double f_log(double var);
double f_frac(double var);



double max_num = 5;  // Set here the max number of the funcion which have to be the value of the number not relatex to x


double compute_function(initial_variable *param) {

    double res = 0;

    if(strcmp(param->function, "exp") == 0) {
        res = f_exp(param->xs);
    } else if(strcmp(param->function, "log") == 0) {
        res = f_log(param->xs);
    } else if(strcmp(param->function, "frac") == 0) {
        res = f_frac(param->xs);
    }
    return res;
}


double f_exp(double var) {
    // e^x-3x^2
    return exp(var) - max_num * pow(var, 2);
}


double f_log(double var) {
    // 2x-log(2x/3+x)-2
    return 2 * var - log((2 * var / max_num) + var) - 2;
}

double f_frac(double var) {
    // (3x^3-4x-2)/(2x^2-5)
    return (3 * pow(var,3) - max_num * var) / (2 * pow(var,2) - max_num);
}


void compute_roots(int max_steps, initial_variable *vars, int *step, double *result, int *check_res, int *steps) {

    double f0, f1, fs;

    if(strcmp(vars->function, "log") == 0 && vars->x1 > 0 && vars->x0 < 0) {
        vars->x0 = 0;
    }
    if(strcmp(vars->function, "log") == 0 && vars->x1 < 0) {
        *check_res = 0;
	puts("è minore di 0");
    } else if(strcmp(vars->function, "frac") == 0 && 
        (vars->x1 == sqrt(max_num/2) ||
            vars->x0 == sqrt(max_num/2 || 
            vars->x1 == sqrt(max_num/2) || 
            vars->x0 == sqrt(max_num/2)))) {
        *check_res = 0;
    } else {

        vars->xs = vars->x0;
        f0 = compute_function(vars);

        vars->xs = vars->x1;
        f1 = compute_function(vars);
        
        if(f0*f1 <= 0) {
            for(int i=0; i < max_steps; i++) {
            
                // vars->xs = vars->x0 - (vars->x0-vars->x1) * func->f0/(func->f0-func->f1);
                vars->xs = (vars->x0 * f1 - vars->x1 * f0)/(f1 - f0);
                fs = compute_function(vars);
                
                if(f0*fs < 0) {
                    vars->x1 = vars->xs;
                    f1 = fs;
                } else {
                    vars->x0 = vars->xs;
                    f0 = fs;
                }
                *step = *step + 1;

                if(fabs(fs) <= vars->fe) {
                    *check_res = 1;
                    *result = vars->xs;
                    *steps = i;
                    break;
                } else {
		    *check_res = 0;
		}
            }
        } else {
            *check_res = 0;
        }        
    }
}

// void axis_partitioning(int max_intervalls, double min, double max, double error, double *intervalls) {
//     for(int i = 0; i < max_intervalls; i++) {
//         intervalls[i] = min + i * error;
//     }
// }


void axis_partitioning(double min, double max, double error, double *intervalls) {
    int interval = (max - min) / error;

    for(int i = 0; i < interval; i++) {
        intervalls[i] = min + i * error;
    }
}

double get_max_num() {
    return max_num;
}
