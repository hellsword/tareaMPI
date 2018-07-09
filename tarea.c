#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <time.h>

#define DIM 1000

void main(int argc, char **argv){

    clock_t t_ini, t_fin;
    double secs;

    t_ini = clock();

    //Lee la cantidad de vectores
    int cant_vectores;
    scanf("%d", &cant_vectores);
    //Comprueba los datos de entrada
    if(cant_vectores <= 0){
        printf("\nERROR: cantidad de vectores no valida\n");
        exit(0);
    }
    printf("\ncantidad de vectores en BD: %d\n", cant_vectores);
    printf("\n");

    //Lee los vectores
    int vectores[cant_vectores][DIM];
    for(int i=0;i < cant_vectores;i++){
        for(int j=0;j < DIM;j++){
            scanf("%d", &vectores[i][j]);
            //Comprueba los datos de entrada
            if(vectores[i][j] < 0){
                printf("\nERROR: datos para vectores incorrectos\n");
                exit(0);
            }
        }
    }

    //Imprime los vectores
    printf("Vectores en BD:\n");
    for(int i=0;i < cant_vectores;i++){
        for(int j=0;j < DIM;j++){
            //printf("%d   ",vectores[i][j]);
        }
        //printf("\n");
    }
    //printf("\n");

    //Lee el vector objetivo
    int vector_obj[DIM];
    for(int i=0;i < DIM;i++){
        scanf("%d", &vector_obj[i]);
        //Comprueba los datos de entrada
        if(vector_obj[i] < 0){
            printf("\nERROR: datos del vector consulta incorrectos\n");
            exit(0);
        }
    }

    //Imprime el vector objetivo
    printf("Vector objetivo:\n");
    for(int i=0;i < DIM;i++){
        //printf("%d   ",vector_obj[i]);
    }
    //printf("\n");

    //Calcula la distancia euclidiana entre el vector objetivo y cada uno de los vectores en la BD
    double min_distancia = INFINITY;
    for(int i=0;i < cant_vectores;i++){

        //printf("\n Vector: %d\n", i+1);
        //Efectua la suma de las diferencias al cuadrado punto a punto entre ambos vectores 
        int resta = 0;
        for(int j=0;j < DIM;j++){
            resta = resta + pow( ( vectores[i][j] - vector_obj[j] ), 2 );
        }
        //printf("    resta = %d\n", resta);

        //Calcula la raiz cuadrada de la diferencia obtenida anteriormente
        double distancia = sqrt(resta);
        if(distancia < min_distancia)
            min_distancia = distancia;
        //printf("    distancia = %lf \n", distancia);
        //printf("\n");
    }

    printf("    menor distancia = %lf \n", min_distancia);
    

    t_fin = clock();
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    printf("%.16g milisegundos\n", secs * 1000.0);

    getchar();
}
