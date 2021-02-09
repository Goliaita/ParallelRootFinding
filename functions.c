#include "functions.h"




double f_exp();


double compute_function(initial_variable *param) {

    double res = 0;
    
    if(strcmp(param->function, "exp")) {
        res = f_exp();
    } else if(strcmp(param->function, "log")) {

    } else if(strcmp(param->function, "fraction")) {

    }

    return res;
}


double f_exp() {
    return 0;
}
