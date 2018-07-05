#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define DIM 4


double squareEuclideanDist(int  p_vec, int q_vec){
    /*
    Este método implementa el cuadrado de la distancia
    :p_vec: primer vector
    :q_vec: segundo vector
    :return: La distancia euclidiana al cuadrado de los vectores uno y dos
    */
    int diff = p_vec - q_vec;
    diff = pow (diff, 2);

    return diff;
}


void main(int argc, char **argv){

    //Lee la cantidad de vectores
    int cant_vectores;
    scanf("%d", &cant_vectores);
    if(cant_vectores <= 0){
        printf("\nERROR: datos de entrada no validos\n");
        exit(0);
    }
    printf("\ncantidad de vectores: %d\n", cant_vectores);
    printf("\n");

    //Lee los vectores
    int vectores[cant_vectores][DIM];
    for(int i=0;i < cant_vectores;i++){
        for(int j=0;j < DIM;j++){
            scanf("%d", &vectores[i][j]);
        }
    }

    //Imprime los vectores
    for(int i=0;i < cant_vectores;i++){
        for(int j=0;j < DIM;j++){
            printf("%d   ",vectores[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    //Lee el vector objetivo
    int vector_obj[DIM];
    for(int i=0;i < DIM;i++){
        scanf("%d", &vector_obj[i]);
    }

    //Imprime el vector objetivo
    for(int i=0;i < DIM;i++){
        printf("%d   ",vector_obj[i]);
    }
    printf("\n");

    double distancia = squareEuclideanDist

    getchar();
}
