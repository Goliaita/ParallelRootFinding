#include "functions.h"

#define pf printf("siamo nel file %s, alla riga %d, nella funzione %s\n", __FILE__, __LINE__, __FUNCTION__);


/**
 * Function.c File
 * 
 * Created by Davide Basile student at University of Salento
 * 
 * Project related to Parallel Algorithms exam
 * 
 * This file contains how the function is computed and all stuff needed related to the function
 * 
 */

/**
 * Function declaration
 */
float get_ris(int *bool_sub, int *bool_fraction,float ris, float *buff_fraction,
        float *buff_number, float *buff_var);

float get_number(int *i, initial_variable *param, int *variable);

int check_variable(char var, int *variable);

void get_check_variable(int *variable, int number);

int detect_max_roots(initial_variable *vars);


/**
 * Compute function return back the value of the function written in function.txt. It manages different kind of exception
 * giving back the exact result if it exists and returning back an exit if something went wrong
 */

float compute_function(initial_variable *param) {

    float buff_number       = 1;
    float buff_exp          = 1;
    float buff_var          = 1;
    float buff_fraction     = 1;

    float buff_b_var        = 1;
    float buff_b_number     = 1;
    float buff_b_exp        = 1;
    float buff_b_fraction   = 1;

    float b_ris             = 0;
    float ris               = 0;

    int variable            = 0;

    int bool_sub            = 0;
    int bool_exp            = 0;
    int bool_brackets       = 0;
    int bool_fraction       = 0;

    int bool_b_fraction     = 0;
    int bool_b_sub          = 0;
    int bool_b_close        = 0;



    for(int i = 0; i<param->size; i++) {
        switch(param->function[i]) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
                
                check_variable(param->function[i], &variable);

                if(bool_brackets) {
                    buff_b_var = param->xs;
                } else {
                    buff_var = param->xs;
                }
                
                bool_exp = 0;
                break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':

                if(bool_brackets) {
                    if(bool_exp) {
                        buff_b_exp = get_number(&i, param, &variable);
                        buff_b_var = pow(buff_b_var, buff_b_exp);
                    } else {
                        buff_b_number = get_number(&i, param, &variable);
                    }
                } else {
                    if(bool_exp) {
                        buff_exp = get_number(&i, param, &variable);
                        buff_var = pow(buff_var, buff_exp);
                    } else {
                        buff_number = get_number(&i, param, &variable);
                    }
                }
                
                bool_exp = 0;
                
                break;

            case '^':
                bool_exp = 1;
                break;

            case '+':
                if(bool_brackets) {

                    b_ris = get_ris(&bool_b_sub, &bool_b_fraction, b_ris, &buff_b_fraction, &buff_b_number, &buff_b_var);
                    buff_b_var = 1;
                    buff_b_number = 1;

                } else if(bool_b_close) {

                    float buf = buff_var * buff_number * b_ris;

                    if(bool_sub) {
                        ris -= buf;
                    } else {
                        ris += buf;
                    } 

                    buff_number = 1;
                    buff_var = 1;
                    b_ris = 0;

                    bool_b_close = 0;
                    
                } else {
                    ris = get_ris(&bool_sub, &bool_fraction, ris, &buff_fraction, &buff_number, &buff_var);
                    buff_var = 1;
                    buff_number = 1;
                }



                bool_exp = 0;

                break;

            case '-':
                if(bool_brackets) {

                    b_ris = get_ris(&bool_b_sub, &bool_b_fraction, b_ris, &buff_b_fraction, &buff_b_number, &buff_b_var);

                    buff_b_number = 1;
                    buff_b_var = 1;

                    bool_b_sub = 1;

                } else if(bool_b_close) { 
                       
                    float buf = buff_var * buff_number * b_ris;

                    if(bool_sub) {
                        ris -= buf;
                    } else {
                        ris += buf;
                    } 

                    b_ris = 0;
                    buff_number = 1;
                    buff_var = 1;

                    bool_b_close = 0;
                    bool_sub = 1;
                    

                } else {

                    ris = get_ris(&bool_sub, &bool_fraction, ris, &buff_fraction, &buff_number, &buff_var);

                    buff_number = 1;
                    buff_var = 1;

                    bool_sub = 1;
                }

                bool_exp = 0;
                break;

            case '/': 
                if(bool_brackets) {

                    bool_b_fraction = 1;
                    buff_b_fraction = buff_b_number * buff_b_var;

                    buff_b_number = 1;
                    buff_b_var = 1;

                } else if(bool_b_close) {

                    buff_fraction = buff_var * buff_number * b_ris;
                    
                    buff_number = 1;
                    buff_var = 1;
                    b_ris = 0;

                    bool_fraction = 1;
                    bool_b_close = 0;

                } else {
                    bool_fraction = 1;
                    buff_fraction = buff_number * buff_var;

                    buff_number = 1;
                    buff_var = 1;
                }

                bool_exp = 0;
                
                break;

            case '(':
                bool_brackets = 1;
                break;

            case ')':
                
                b_ris = get_ris(&bool_b_sub, &bool_b_fraction, b_ris, &buff_b_fraction, &buff_b_number, &buff_b_var);

                if(bool_fraction) {
                    b_ris = buff_fraction / b_ris;
                    bool_fraction = 0;
                }

                if(bool_exp) {
                    buff_number = pow(buff_number * buff_var, b_ris);

                    b_ris = 0;
                    buff_var = 1;
                }

                bool_b_close = 1;
                bool_b_fraction = 0;
                bool_brackets = 0;
                bool_exp = 0;

                break;

            default:
                puts("\tCharacter not recognised\t");
                printf("\tthe unrecognised character is: \"%c\" ignored\n", param->function[i]);
                exit(1);
        }
    }

    if(bool_b_close) {

        float buf = buff_var * buff_number * b_ris;

        if(bool_sub) {
            ris -= buf;
        } else {
            ris += buf;
        } 
    } else {
        ris = get_ris(&bool_sub, &bool_fraction, ris, &buff_fraction, &buff_number, &buff_var);
    }

    return ris;
}


/**
 * Get_ris gives back the result of the function computated until this function is created
 */
float get_ris(int *bool_sub, int *bool_fraction, float ris, float *buff_fraction,
        float *buff_number, float *buff_var) {

    float new_ris = 0;

    if(*bool_sub) {
        if(*bool_fraction) {

            new_ris = ris - *buff_fraction / (*buff_number * *buff_var);
            
            *bool_fraction = 0;
                
            *buff_fraction = 1;


        } else {
            new_ris = ris - *buff_number * *buff_var;
        }
        
        *bool_sub = 0;
            
    } else {
        if(*bool_fraction) {

            new_ris = ris + *buff_fraction / (*buff_number * *buff_var);

            *bool_fraction = 0;
            
            *buff_fraction = 1;

        } else {
            new_ris = ris + *buff_number * *buff_var;
        }
    }

    *buff_var = 1;
    *buff_number = 1;

    return new_ris;
}


/**
 * Get_number gives back the entire number, jumping over it to the next non number character
 */
float get_number(int *i, initial_variable *param, int *variable) {

    float number = atof(&param->function[*i]);
    int count = 0;

    for(int j = *i++ ; j < param->size; j++) {
        if(check_variable(param->function[j], variable) || param->function[j] == '^' || param->function[j] == '/' || 
        param->function[j] == '(' || param->function[j] == ')' || param->function[j] == '+' || param->function[j] == '-'){
            break;
        } else {
            number = number + atof(&param->function[j]);
        }

        count++;
    }

    *i += count;
    return number;
}


/**
 * Detect_max_roots as name suggest check all the function one time to compute the max number of the roots
 */
int detect_max_roots(initial_variable *vars) {

    int max_roots   = 0;
    int variable    = 0;

    for(int i = 0; i < vars->size; i++) {
        switch(vars->function[i]) {
            case '^':
                i++;
                max_roots = (int) get_number(&i, vars, &variable);
        }
    }
    return max_roots;
}


/**
 * Check_variable check if the user inserted more than one variable in the function
 */
int check_variable(char var, int *variable) {

    int check = 1;

    switch(var) {
        case 'a':
            get_check_variable(variable, 1);
            break;

        case 'b':
            get_check_variable(variable, 2);
            break;

        case 'c':
            get_check_variable(variable, 3);
            break;

        case 'd':
            get_check_variable(variable, 4);
            break;

        case 'e':
            get_check_variable(variable, 5);
            break;

        case 'f':
            get_check_variable(variable, 6);
            break;

        case 'g':
            get_check_variable(variable, 7);
            break;

        case 'h':
            get_check_variable(variable, 8);
            break;

        case 'i':
            get_check_variable(variable, 9);
            break;

        case 'j':
            get_check_variable(variable, 10);
            break;

        case 'k':
            get_check_variable(variable, 11);
            break;

        case 'l':
            get_check_variable(variable, 12);
            break;

        case 'm':
            get_check_variable(variable, 13);
            break;

        case 'n':
            get_check_variable(variable, 14);
            break;

        case 'o':
            get_check_variable(variable, 15);
            break;

        case 'p':
            get_check_variable(variable, 16);
            break;

        case 'q':
            get_check_variable(variable, 17);
            break;

        case 'r':
            get_check_variable(variable, 18);
            break;

        case 's':
            get_check_variable(variable, 19);
            break;

        case 't':
            get_check_variable(variable, 20);
            break;

        case 'u':
            get_check_variable(variable, 21);
            break;

        case 'v':
            get_check_variable(variable, 22);
            break;

        case 'w':
            get_check_variable(variable, 23);
            break;

        case 'x':
            get_check_variable(variable, 24);
            break;

        case 'y':
            get_check_variable(variable, 25);
            break;

        case 'z':
            get_check_variable(variable, 26);
            break;
    }
    return check;
}


void get_check_variable(int *variable, int number) {
    if(*variable == 0) {
        *variable = number;
    } else if(*variable != number) {
        puts("Error inserted more than one variable check function and write it with only one variable");
        exit(1);
    }
}