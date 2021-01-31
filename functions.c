#include "functions.h"


/**
 * Support function
 */

float get_ris(int *bool_sub, int *bool_fraction, float *ris, float *buff_fraction,
        float *buff_number, float *buff_var);

float get_number(int *i, initial_variable *param);



/**
 * Function that manage the parameters sent by command line
 * -x0 get the lower bound constraint (float)
 * -x1 get the upper bound constraint (float)
 * -e  get the error which generate a guess of the solution (float)
 * -f  get the file [if is in another folder include the path] of the function which we need to find roots (char *)
 * -a  let to the algorithm to compute contraint except error
 */

initial_variable *get_parameters(int argc, char* argv[]){

    initial_variable *params;
    params = malloc(sizeof(initial_variable));
    params->auto_choose = 0;
    FILE *fp;
    char line[50];
    int check = 0;

    if (argc > 1) {
        for (int i=0; i<argc; i++) {
            if (strcmp(argv[i],"-x0") == 0) {

                check = 1;
                params->x0 = atof(argv[i+1]);

            } else if (strcmp(argv[i], "-x1") == 0) {

                check = 1;
                params->x1 = atof(argv[i+1]);

            } else if (strcmp(argv[i], "-e") == 0) {

                params->e = atof(argv[i+1]); 

            } else if (strcmp(argv[i], "-f") == 0) {

                fp = fopen(argv[i+1], "r");

                if (!fp) {
                    perror("Error while opening the file->\n");
                    exit(1);
                }

                for(int i = 0; i<2; i++) {
                    if(i == 0) {
                        if (fgets(line, sizeof (line), fp)) {

                            printf("Function size found is: %s\n", line);
                            params->size = atoi(line);
                            params->function = malloc(params->size*sizeof(char));
                        }
                    } else {
                        if (fgets(line, sizeof (line), fp)) {

                            printf("Function found is: %s\n", line);
                            strcpy(params->function,line); 
                            printf("%s\n", params->function);
                        }
                    }
                }

            } else if (strcmp(argv[i], "-a") == 0) {

                params->auto_choose = 1;

            } else if (strcmp(argv[i], "-h") == 0) {

                printf("Usage -x0 [smallest bound], -x1 [higher bound], -e [error tollerance],\n");
                printf("\t-f [path to file which contain the function to compute], -a autocompute constraints\n");
                printf("\tif no element passed default value are: -x0=-2; -x1=2 e=0->001\n");

                printf("Error list 1: wrong parameters; 2 \n");
            }
        }
    }
    if(params->x0 > params->x1){

        puts("Warning x1 e' minore di x0 sono stati scambiati\n");

        params->x0 += params->x1;
        params->x1 = params->x0 - params->x1;
        params->x0 -= params->x1;

    }
    if(!(params->auto_choose&&check)) {

        puts("ritorna i parametri");
        return params;

    } else {
        puts("sono diversi");
        exit(1);
    }
    
}


float compute_function(initial_variable *param){
    printf("xs è: %f", param->xs);

    float buff_number   = 1;
    float buff_exp      = 1;
    float buff_var      = 1;
    float buff_fraction = 1;
    float ris           = 0;

    int bool_sub        = 0;
    int bool_exp        = 0;
    int bool_brackets   = 0;
    int bool_fraction   = 0;
    int bool_first_n    = 0;


    for(int i = 0; i<param->size; i++) {

        switch(param->function[i]) {
        case 'x':
            buff_var = param->xs;
            bool_first_n = 0;
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
            
            if(bool_exp){
                buff_exp = get_number(&i, param);
                printf("la potenza vale: %f\n", buff_exp);
                printf("la x vale: %f\n", buff_var);
                buff_var = pow(buff_var, buff_exp);
                printf("la potenza vale: %f\n", buff_var);
            } else {
                buff_number = get_number(&i, param);
                printf("il numero vale: %f\n", buff_number);
            }
            
            bool_exp = 0;
            break;

        case '^':
            bool_exp = 1;
            break;

        case '+':

            get_ris(&bool_sub, &bool_fraction, &ris, &buff_fraction, &buff_number, &buff_var);

            buff_var = 1;
            buff_number = 1;

            bool_exp = 0;

            break;

        case '-':

            get_ris(&bool_sub, &bool_fraction, &ris, &buff_fraction, &buff_number, &buff_var);
            
            bool_sub = 1;
            bool_exp = 0;

            
            break;

        case '/': 

            bool_fraction = 1;
            bool_exp = 0;

            buff_fraction = buff_number * buff_var;
            buff_number = 1;
            buff_var = 1;
            break;

        case '(':


            bool_brackets = 1;
            bool_first_n = 0;
            bool_exp = 0;
            break;

        case ')':


            bool_brackets = 0;
            bool_first_n = 0;
            bool_exp = 0;
            break;

        default:
            puts("\tNot recognised\n");
            break;
        }
    }

    get_ris(&bool_sub, &bool_fraction, &ris, &buff_fraction, &buff_number, &buff_var);
    printf("ris vale: %f\n", ris);
    return ris;
}


float get_ris(int *bool_sub, int *bool_fraction, float *ris, float *buff_fraction,
        float *buff_number, float *buff_var) {
    if(bool_sub[0]) {
        if(bool_fraction[0]) {

            ris[0] -= buff_fraction[0] / (buff_number[0] * buff_var[0]);

            bool_fraction[0] = 0;
                
            buff_fraction[0] = 1;


        } else {
            ris[0] -= buff_number[0] * buff_var[0];
        }

        bool_sub[0] = 0;
            
    } else {
        if(bool_fraction) {

            ris[0] += buff_fraction[0] / (buff_number[0] * buff_var[0]);

            bool_fraction[0] = 0;
            
            buff_fraction[0] = 1;

        } else {
            ris[0] += buff_number[0] * buff_var[0];
        }
    }

    buff_var[0] = 1;
    buff_number[0] = 1;

    return ris[0];
}



float get_number(int *i, initial_variable *param){
    float number = atof(&param->function[i[0]]);
    int count = 0;

    for(int j = i[0]+1 ; j < param->size; j++) {
        if(param->function[j] == 'x' || param->function[j] == '^' || param->function[j] == '/' || 
        param->function[j] == '(' || param->function[j] == ')' || param->function[j] == '+' || param->function[j] == '-'){
            break;
        } else {
            number = number + atof(&param->function[j]);
        }

        count++;
    }

    i[0] += count;
    return number;
}

