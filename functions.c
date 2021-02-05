#include "functions.h"

#define pf printf("siamo nel file %s, alla riga %d, nella funzione %s\n", __FILE__, __LINE__, __FUNCTION__);




/**
 * Support function
 */
float get_ris(int *bool_sub, int *bool_fraction,float ris, float *buff_fraction,
        float *buff_number, float *buff_var);

float get_number(int *i, initial_variable *param, int *variable);

int check_variable(char var, int *variable);



/**
 * Function that manage the parameters sent by command line
 * -x0 get the lower bound constraint (float)
 * -x1 get the upper bound constraint (float)
 * -e  get the error which generate a guess of the solution (float)
 * -f  get the file [if is in another folder include the path] of the function which we need to find roots (char *)
 * -a  let to the algorithm to compute contraint except error
 */

initial_variable *get_parameters(int argc, char* argv[]) {

    initial_variable *params;

    params = malloc(sizeof(initial_variable));
    params->auto_choose = 0;

    FILE *fp;

    char line[50];

    int check = 0;

    if(argc > 1) {
        for(int i=0; i<argc; i++) {
            if(strcmp(argv[i],"-x0") == 0) {

                check = 1;
                params->x0 = atof(argv[i+1]);

            } else if(strcmp(argv[i], "-x1") == 0) {

                check = 1;
                params->x1 = atof(argv[i+1]);

            } else if(strcmp(argv[i], "-e") == 0) {

                params->e = atof(argv[i+1]); 

            } else if(strcmp(argv[i], "-f") == 0) {

                fp = fopen(argv[i+1], "r");

                if(!fp) {
                    perror("Error while opening the file->\n");
                    exit(1);
                }

                if(fgets(line, sizeof(line), fp)) {
                    int k = 0;
                    for(int j = 0; line[j]; j++) {
                        line[j] = line[j+k];
                        // printf("%c", line[])
                        if(line[j] == ' ' || line[j] == '\t' || line[j] == '\n') {
                            k++;
                            j--;
                        }
                    }

                    // printf("Function found is: %s\n", line);
                    params->size = strlen(line);
                    params->function = malloc(params->size * sizeof(char));
                    strcpy(params->function,line); 
                    printf("string lenght found %d\n", params->size);
                }

                fclose(fp);

            } else if(strcmp(argv[i], "-a") == 0) {

                params->auto_choose = 1;

            } else if(strcmp(argv[i], "-h") == 0) {

                printf("Usage -x0 [smallest bound], -x1 [higher bound], -e [error tollerance],\n");
                printf("\t-f [path to file which contain the function to compute], -a autocompute constraints\n");
                printf("\tif no element passed default value are: -x0=-2; -x1=2 e=0->001\n");

                //TODO List errors that could be encountered

            }
        }
    }

    if(params->x0 > params->x1) {

        puts("Warning x1 e' minore di x0 sono stati scambiati\n");

        params->x0 += params->x1;
        params->x1 = params->x0 - params->x1;
        params->x0 -= params->x1;

    }
    if(!(params->auto_choose&&check)) {
        return params;
    } else {
        puts("sono diversi");
        exit(1);
    } 
}


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
                puts("\tNot recognised\t");
                printf("%c\n", param->function[i]);
                break;
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


float get_ris(int *bool_sub, int *bool_fraction, float ris, float *buff_fraction,
        float *buff_number, float *buff_var) {

    float new_ris = 0;

    if(*bool_sub) {
        if(*bool_fraction) {

            new_ris = ris - *buff_fraction / (*buff_number * *buff_var);
            
            bool_fraction = 0;
                
            buff_fraction = 1;


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


float get_number(int *i, initial_variable *param, int *variable) {

    float number = atof(&param->function[i[0]]);
    int count = 0;

    for(int j = *i+1 ; j < param->size; j++) {
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

int check_variable(char var, int *variable) {

    int check = 1;

    switch(var) {
        case 'a':
            if(*variable == 0) {
                *variable = 1;
            } else if(*variable != 1) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'b':
            if(*variable == 0) {
                *variable = 2;
            } else if(*variable != 2) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'c':
            if(*variable == 0) {
                *variable = 3;
            } else if(*variable != 3) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'd':
            if(*variable == 0) {
                *variable = 4;
            } else if(*variable != 4) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'e':
            if(*variable == 0) {
                *variable = 5;
            } else if(*variable != 5) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'f':
            if(*variable == 0) {
                *variable = 6;
            } else if(*variable != 6) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'g':
            if(*variable == 0) {
                *variable = 7;
            } else if(*variable != 7) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'h':
            if(*variable == 0) {
                *variable = 8;
            } else if(*variable != 8) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'i':
            if(*variable == 0) {
                *variable = 9;
            } else if(*variable != 9) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'j':
            if(*variable == 0) {
                *variable = 10;
            } else if(*variable != 10) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'k':
            if(*variable == 0) {
                *variable = 11;
            } else if(*variable != 11) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'l':
            if(*variable == 0) {
                *variable = 12;
            } else if(*variable != 12) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'm':
            if(*variable == 0) {
                *variable = 13;
            } else if(*variable != 13) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'n':
            if(*variable == 0) {
                *variable = 14;
            } else if(*variable != 14) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'o':
            if(*variable == 0) {
                *variable = 15;
            } else if(*variable != 15) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'p':
            if(*variable == 0) {
                *variable = 16;
            } else if(*variable != 16) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'q':
            if(*variable == 0) {
                *variable = 17;
            } else if(*variable != 17) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'r':
            if(*variable == 0) {
                *variable = 18;
            } else if(*variable != 18) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 's':
            if(*variable == 0) {
                *variable = 19;
            } else if(*variable != 19) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 't':
            if(*variable == 0) {
                *variable = 20;
            } else if(*variable != 20) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'u':
            if(*variable == 0) {
                *variable = 21;
            }else if(*variable != 21) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'v':
            if(*variable == 0) {
                *variable = 22;
            } else if(*variable != 22) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'w':
            if(*variable == 0) {
                *variable = 23;
            } else if(*variable != 23) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'x':
            if(*variable == 0) {
                *variable = 24;
            } else if(*variable != 24) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'y':
            if(*variable == 0) {
                *variable = 25;
            } else if(*variable != 25) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;

        case 'z':
            if(*variable == 0) {
                *variable = 26;
            } else if(*variable != 26) {
                puts("Error inserted more than one variable check function and write it with only one variable");
                exit(1);
            }
            break;
    }
    return check;
}