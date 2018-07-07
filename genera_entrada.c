#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<math.h>

void main(int argc, char **argv){

    int DIM;
    //printf("Defina la dimension de los vectores:");
    scanf("%d", &DIM);

    int cant_vectores;
    //printf("Defina la cantidad de vectores:");
    scanf("%d", &cant_vectores);
    
    /*
    printf("\nResumen de entrada a generar: \n");
    printf("    dimension de vectores: %d\n", DIM);
    printf("    cantidad de vectores: %d\n", cant_vectores);
    */
    printf("%d\n", cant_vectores);

    srand(time(NULL));   // should only be called once

    //Imprime los vectores
    for(int i=0;i < cant_vectores;i++){
        for(int j=0;j < DIM;j++){
            printf("%d   ",rand() % 100);
        }
        printf("\n");
    }
    //printf("\n");

    //Imprime el vector objetivo
    for(int i=0;i < DIM;i++){
        printf("%d   ",rand() % 100);
    }

}    