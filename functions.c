#include "functions.h"



/**
 * Function.c File
 * 
 * Created by Davide Basile student at University of Salento
 * 
 * Project related to Parallel Algorithms exam
 * 
 * This file contains the function declared in functions.h
 * 
 */





/**
 *  Here you can define the logarithm argument or the denominator argument of the function
 */
// #define log_argument(var, max_num)  (2 * var / max_num) + var
#define log_argument(var, max_num)  3 * pow(var, 2) - max_num + 3 * pow(var, 3) - 32 * var
#define denominator(var, max_num)   (2 * pow(var,2) + max_num)


/**
 * Declaration of functions
 */
double f_exp(double var);
double f_log(double var, double log_arg);
double f_frac(double var, double den_arg);
double f_sin(double var);



double max_num = 43;  // Set here the max number of the funcion which have to be the value of the number not relatex to x

/**
 * Using the function string here we return the value of the function at the x value
 */
double compute_function(initial_variable *param, double arg) {

    double res = 0;

    if(strcmp(param->function, "exp") == 0) {
        res = f_exp(param->xs);

    } else if(strcmp(param->function, "log") == 0) {
        res = f_log(param->xs, arg);

    } else if(strcmp(param->function, "frac") == 0) {
        res = f_frac(param->xs, arg);
    } else if(strcmp(param->function, "sin") == 0) {
        res = f_sin(param->xs);
    }

    return res;
}


double f_exp(double var) {
    // e^x-3x^2
    return exp(var) - max_num * pow(var, 2);
}


double f_log(double var, double log_arg) {
    // 2x-log(2x/5+x)-2
    // return 2 * var - log(log_arg) - 2;
    // 3x^2+2x^5-log(3x+1)-43x^3-10+34x
    return 3 * pow(var, 2) + 2 * pow(var, 5) - log(log_arg) - max_num * pow(var, 3) - 10 + 34 * var;
}

double f_frac(double var, double den_arg) {
    // (3x^3-5x)/(2x^2-5)
    return (3 * pow(var,3) - max_num * var) / denominator(var, max_num);
}


double f_sin(double var) {
    // 3x^2*sin(2x+2)-4x-1
    return 3 * pow(var, 2) * sin(2 * var + 2) - 4 * var - 1;
}


/**
 * This function compute roots of the function if checks goes well
 */
void compute_roots(int max_steps, initial_variable *vars, double *result, int *check_res) {

    double f0, f1, fs;

    double logs = 0;

    /**
     * Here we are checking if the argument of logarithm is less than 0 
     */
    if(strcmp(vars->function, "log") == 0 && log_argument(vars->x0, max_num) < vars->p && log_argument(vars->x1, max_num) > 0) {
        logs = vars->p;
    } else {
        logs = log_argument(vars->x0, max_num);
    }

    /**
     * When the upperbound of logarithm is less than zero we can skip the computation
     */
    if(strcmp(vars->function, "log") == 0 && log_argument(vars->x1, max_num) < 0) {
        *check_res = 0;

    } else {

        vars->xs = vars->x0;

        /**
         * Just the last check of x0 about the denominator if fractional function is called
         */
        if(strcmp(vars->function, "log") == 0) {
            if(logs < vars->p && logs > 0){
                f0 = compute_function(vars, vars->p);
            } else {
                f0 = compute_function(vars, logs);
            }
        } else if(strcmp(vars->function, "frac") == 0 && abs(denominator(vars->xs, max_num)) < vars->p) {
            if(denominator(vars->x0, max_num) > 0){
                f0 = compute_function(vars, vars->p);
            } else {
                f0 = compute_function(vars, -vars->p);
            }
        } else {
            f0 = compute_function(vars, vars->p);
        }

        vars->xs = vars->x1; 

        /**
         * Just the last check of x1 about the denominator if fractional function is called
         */
        if(strcmp(vars->function, "log") == 0) {
            logs = log_argument(vars->xs, max_num);  
            if(logs < vars->p && logs > 0){
                f1 = compute_function(vars, vars->p);
            } else {
                f1 = compute_function(vars, logs);
            }
        } else if(strcmp(vars->function, "frac") == 0 && abs(denominator(vars->xs, max_num)) < vars->p) {
            if(denominator(vars->xs, max_num) > 0){
                f1 = compute_function(vars, vars->p);
            } else {
                f1 = compute_function(vars, -vars->p);
            }
        } else {
            f1 = compute_function(vars, vars->p);
        }
        
        /**
         * If upperbound and lowerbound has different sign we can compute the root
         */
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
                                
                if(fabs(fs) <= vars->p) {
                    *check_res = 1;
                    *result = vars->xs;
                    break;
                }
            }
        } else {
            *check_res = 0;
        }        
    }
}

/**
 * Partition the array into interval parts
 */
void axis_partitioning(double min, double max, int interval, double *intervals) {
    double interval_size = (max-min) / interval;
    for(int i = 0; i < interval; i++) {
        intervals[i] = min + i * interval_size;
    }
}


/**
 * Return the max number inside the function
 */
double get_max_num() {
    return max_num;
}

