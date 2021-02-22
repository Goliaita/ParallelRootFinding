#include "functions.h"

#define log_argument(var, max_num)  (2 * var / max_num) + var
#define denominator(var, max_num)   (2 * pow(var,2) - max_num)


/**
 * Declaration of functions
 */
double f_exp(double var);
double f_log(double var, double log_arg);
double f_frac(double var);



double max_num = 5;  // Set here the max number of the funcion which have to be the value of the number not relatex to x


double compute_function(initial_variable *param, double arg) {

    double res = 0;

    if(strcmp(param->function, "exp") == 0) {
        res = f_exp(param->xs);

    } else if(strcmp(param->function, "log") == 0) {
        res = f_log(param->xs, arg);

    } else if(strcmp(param->function, "frac") == 0) {
        res = f_frac(param->xs);
    }

    return res;
}


double f_exp(double var) {
    // e^x-3x^2
    return exp(var) - max_num * pow(var, 2);
}


double f_log(double var, double log_arg) {
    // 2x-log(2x/5+x)-2
    return 2 * var - log(log_arg) - 2;
}

double f_frac(double var) {
    // (3x^3-5x)/(2x^2-5)
    return (3 * pow(var,3) - max_num * var) / denominator(var, max_num);
}

void compute_roots(int max_steps, initial_variable *vars, int *step, double *result, int *check_res, int *steps) {

    double f0, f1, fs;

    double logs = 0;

    if(strcmp(vars->function, "log") == 0 && log_argument(vars->x0, max_num) < vars->p && log_argument(vars->x1, max_num) > 0) {
        logs = vars->p;
    } else {
        logs = log_argument(vars->x0, max_num);
    }

    if(strcmp(vars->function, "log") == 0 && log_argument(vars->x1, max_num) < 0) {
        *check_res = 0;

    } else if(strcmp(vars->function, "frac") == 0 && 
        (vars->x1 == sqrt(max_num/2) ||
            vars->x0 == sqrt(max_num/2))) {
        *check_res = 0;

    } else {

        vars->xs = vars->x0;

        if(logs < vars->p && logs > 0){
            f0 = compute_function(vars, vars->p);
        } else {
            f0 = compute_function(vars, logs);
        }

        vars->xs = vars->x1;    
        logs = log_argument(vars->xs, max_num);

        if(logs < vars->p && logs > 0){
            f1 = compute_function(vars, vars->p);
        } else {
            f1 = compute_function(vars, logs);
        }
        
        if(f0*f1 <= 0) {
            for(int i=0; i < max_steps; i++) {
                
                vars->xs = (vars->x0 * f1 - vars->x1 * f0)/(f1 - f0);

                logs = log_argument(vars->xs, max_num);
                if(logs < vars->p && logs > 0){
                    fs = compute_function(vars, vars->p);
                } else {
                    fs = compute_function(vars, logs);
                }

                if(f0*fs < 0) {
                    vars->x1 = vars->xs;
                    f1 = fs;
                } else {
                    vars->x0 = vars->xs;
                    f0 = fs;
                }
                *step = *step + 1;


                if(fabs(fs) <= vars->p) {
                    *check_res = 1;
                    *result = vars->xs;
                    *steps = i;
                    break;
                }
            }
        } else {
            *check_res = 0;
        }        
    }
}


void axis_partitioning(double min, double max, int intervall, double *intervalls) {
    double intervall_size = (max-min) / intervall;
    for(int i = 0; i < intervall; i++) {
        intervalls[i] = min + i * intervall_size;
    }
}

double get_max_num() {
    return max_num;
}

