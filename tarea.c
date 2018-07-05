#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define DIM 4


void main(int argc, char **argv){

    //Lee la cantidad de vectores
    int cant_vectores;
    scanf("%d", &cant_vectores);
    //Comprueba integridad en los datos de entrada
    if(cant_vectores <= 0){
        printf("\nERROR: datos de entrada no validos\n");
        exit(0);
    }
    printf("\ncantidad de vectores en BD: %d\n", cant_vectores);
    printf("\n");

    //Lee los vectores
    int vectores[cant_vectores][DIM];
    for(int i=0;i < cant_vectores;i++){
        for(int j=0;j < DIM;j++){
            scanf("%d", &vectores[i][j]);
        }
    }

    //Imprime los vectores
    printf("Vectores en BD:\n");
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
    printf("Vector objetivo:\n");
    for(int i=0;i < DIM;i++){
        printf("%d   ",vector_obj[i]);
    }
    printf("\n");

    //Calcula la distancia entre el vector objetivo y cada uno de los vectores en la BD
    for(int i=0;i < cant_vectores;i++){

        printf("\n Vector: %d\n", i+1);
        //Efectua la suma de las diferencias punto a punto entre ambos vectores
        int resta = 0;
        for(int j=0;j < DIM;j++){
            resta = resta + pow( ( vectores[i][j] - vector_obj[j] ), 2 );
        }
        printf("    resta = %d\n", resta);

        //Calcula la raiz cuadrada de la diferencia obtenida anteriormente
        double distancia = sqrt(resta);
        printf("    distancia = %lf \n", distancia);
        printf("\n");
    }

    getchar();
}
