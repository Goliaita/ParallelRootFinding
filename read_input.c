#include "functions.h"


/**
 * Read_Input.c File
 * 
 * Created by Davide Basile student at University of Salento
 * 
 * Project related to Parallel Algorithms exam
 * 
 * This file contains all about to read the argv and manage it
 * 
 */


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
                        if(line[j] == ' ' || line[j] == '\t' || line[j] == '\n') {
                            k++;
                            j--;
                        }
                    }

                    params->size = strlen(line);
                    params->function = malloc(params->size * sizeof(char));
                    strcpy(params->function,line); 
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