#include <stdio.h>
#include <stdlib.h>

int main(){
    int array_prova[10] = {120301203};


    //array_prova = calloc(50, sizeof(int));
    int dimensione = sizeof(array_prova)/sizeof(int);

    printf("%d\n ", dimensione);
    for(int i = 0; i<10; i++) printf("%d\t", array_prova[i]);

    return 0;


}