#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


typedef struct {
    float x0;
    float x1;
    float e;
    char function[50];
} initial_variable;

typedef struct {
    float f0;
    float f1;
    float fs;
} funcions;
initial_variable get_parameters(int argc, char* argv[]);










initial_variable get_parameters(int argc, char* argv[]){

    initial_variable params;
    FILE *fp;
    char line[50];

    if (argc > 1) {
        for (int i=0; i<argc; i++) {
            if (strcmp(argv[i],"-x0") == 0) {

                params.x0 = atof(argv[i+1]);

            } else if (strcmp(argv[i], "-x1") == 0) {

                params.x1 = atof(argv[i+1]);

            } else if (strcmp(argv[i], "-e") == 0) {

                params.e = atof(argv[i+1]); 

            } else if (strcmp(argv[i], "-f") == 0) {

                fp = fopen(argv[i+1], "r");

                if (!fp) {
                    perror("Error while opening the file.\n");
                    exit(1);
                }
                while (!feof (fp)) {
                if (fgets(line, sizeof (line), fp)) {
                    printf("function found is: %s\n", line);
                    strcpy(params.function,line); 
                    }
                }

            } else if (strcmp(argv[i], "-a") == 0) {

                printf("TODO");

            } else if (strcmp(argv[i], "-h") == 0) {

                printf("Usage -x0 [smallest bound], -x1 [higher bound], -e [error tollerance], \
                if no element passed default value are: -x0=-2; -x1=2 e=0.001");

                printf("Error list 1: wrong parameters; 2 \n");
            }
        }
    }

    return params;
}
