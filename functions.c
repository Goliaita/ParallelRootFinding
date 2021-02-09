#include "functions.h"




double f_exp(initial_variable *param);


double compute_function(initial_variable *param) {

    double res = 0;

    if(strcmp(param->function, "exp") == 0) {
        res = f_exp(param);
    } else if(strcmp(param->function, "log") == 0) {

    } else if(strcmp(param->function, "fraction") == 0) {

    }
    return res;
}


double f_exp(initial_variable *param) {

    // e^x-3x^2

    return exp(param->xs)-3*pow(param->xs, 2);
}
