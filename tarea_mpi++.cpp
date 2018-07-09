/***************************************
Sistemas Distribuidos
Tarea MPI

Integrantes:
    -Boris Mora
    -Nicolas Muñoz
***************************************/

#include "mpi.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <list>

using namespace std;

#define DIM 4

int main(int argc, char **argv){

    srand(time(NULL));

    int nproc; /* Número de procesos */
	int yo; /* Mi dirección: 0<=yo<=(nproc-1) */
	int num, flag=0;
    MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc); //Entrega el nro total de nodos
	MPI_Comm_rank(MPI_COMM_WORLD, &yo); //Entrega el ID del nodo actual

    int cant_vectores;  //cantidad de vectores que se leeran
    int dato;
    int vector_obj[DIM];    //aqui se almacena el vector consulta
    int limite = 0;     //Asigna la cantidad de elementos que se pasara a cada proceso
    int vector_temp[DIM];

    //Se trabaja con el proceso 0
    if (yo == 0){

        clock_t t_ini, t_fin;
        double secs;
        t_ini = clock();


        int vector_min[DIM];

        //Lee la cantidad de vectores
        scanf("%d", &cant_vectores);
        //Comprueba los datos de entrada
        if(cant_vectores <= 0){
            printf("\nERROR: La cantidad de vectores no es valida\n");
            exit(0);
        }

        int **vectores;
        // Reserva de Memoria 
        vectores = (int **)malloc(cant_vectores*sizeof(int*)); 
        for (int i=0;i < cant_vectores;i++) 
            vectores[i] = (int*)malloc(DIM*sizeof(int)); 

        //Lee los vectores
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

        //Lee el vector consulta
        for(int i=0;i < DIM;i++){
            scanf("%d", &vector_obj[i]);
            //Comprueba los datos de entrada
            if(vector_obj[i] < 0){
                printf("\nERROR: datos de entrada para vector objetivo no validos\n");
                exit(0);
            }
        }

        double min_distancia = INFINITY;
        int cont = 0;

        if(nproc == 1){
            //Calcula la distancia euclidiana entre el vector objetivo y cada uno de los vectores en la BD

            for(int i=0;i < cant_vectores;i++){

                //Efectua la suma de las diferencias al cuadrado punto a punto entre ambos vectores 
                int resta = 0;
                for(int j=0;j < DIM;j++){
                    resta = resta + pow( ( vectores[i][j] - vector_obj[j] ), 2 );
                }

                //Calcula la raiz cuadrada de la diferencia obtenida anteriormente
                double distancia = sqrt(resta);
                if(distancia < min_distancia){
                    min_distancia = distancia;
                    for(int j=0; j < DIM; j++){
                        vector_min[j] = vectores[i][j];
                    }
                }
            }
        }
        else{
            //El proceso 0 envia los datos leidos anteriormente al resto de procesos disponibles

            int aux = 99;
            //Calcula el modulo de la cantidad de vectores por la cantidad de procesos disponibles
            if( cant_vectores < nproc-1 ){
               aux = cant_vectores%(nproc-1);  
            }
            

            double distancia;
            int inicio = 0;

            //Recorre cada uno de los procesos restantes
            for(int i=1;i < nproc;i++){

                //Envia el vector consulta al proceso destino
                MPI_Send(&vector_obj, DIM, MPI_INT, i, 1, MPI_COMM_WORLD);


                //Se calcula la cantidad de elementos que se enviaran a cada proceso
                //#########################  AQUI EMPIEZA EL ERROR
                /*
                if( cant_vectores < nproc-1 && cont < cant_vectores){
                    cont++;
                    limite = 1;
                }
                else if( cant_vectores < nproc-1 && cont >= cant_vectores ){
                    cont++;
                    limite = 0;
                }*/
                //#########################  AQUI TERMINA EL ERROR
                if(aux == 0){
                    limite = cant_vectores/(nproc-1);
                }
                else{
                    limite = cant_vectores%(nproc-1)+1;
                    aux--;
                }

                printf("proc: %d    |      limite: %d\n", i, limite);

                //Envia la cantidad de datos asignados al proceso destino
                MPI_Send(&limite, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
    
                //Envia los datos asignados al proceso destino
                int j;
                for(j=inicio;j < limite+inicio; j++){
                    for(int k=0;k < DIM;k++){
                        MPI_Send(&vectores[j][k], 1, MPI_INT, i, 3, MPI_COMM_WORLD);
                    }
                }
                inicio = j;

                //Recibe la menor distancia calculada en cada proceso
                MPI_Recv(&distancia, 1, MPI_DOUBLE, i, 4, MPI_COMM_WORLD, &status);

                //Recibe el vector mas cercano al vector consulta
                MPI_Recv(&vector_temp, DIM, MPI_DOUBLE, i, 5, MPI_COMM_WORLD, &status);

                if(distancia < min_distancia){
                    min_distancia = distancia;
                    for(j=0; j < DIM; j++){
                        vector_min[j] = vector_temp[j];
                    }
                }
            }
        }

        //IMprime los resultados finales
        printf("\nVector mas cercano: \n");
        for(int j=0; j < DIM; j++){
            printf("    %d", vector_min[j]);
        }
        printf("\n\nmenor distancia = %lf \n", min_distancia);

        t_fin = clock();
        secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
        printf("%.16g milisegundos\n", secs * 1000.0);
        
    }


    //Se trabaja con los procesos distintos a 0
    if (yo != 0)
	{
		while(flag == 0)
		{
			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
			if (flag != 0) //Mensaje arrivado listo para lectura
			{  
				//Recibe el vector consulta
				MPI_Recv(&vector_obj, DIM, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

                //Recibe la cantidad de datos asignados a este proceso
                MPI_Recv(&limite, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);

                //Recibe los datos asignados a este proceso
                double min_distancia = INFINITY;
                int vector_min[DIM];

                for(int i=0;i < limite; i++){

                    int resta = 0;

                    for(int j=0; j < DIM; j++){
                        MPI_Recv(&dato, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);
                        vector_temp[j] = dato;

                        //Realiza y suma las restas entre cada punto del vector consulta y el vector actual 
                        //necesarias para calcular la distancia. Cada resta se eleva al cuadrado
                        resta = resta + pow( ( dato - vector_obj[j] ), 2 ); 
                    }

                    //Obtiene la distancia euclidiana calculando la raiz de la sumaria de restas anterior
                    double distancia = sqrt(resta);    
                    if(distancia < min_distancia){
                        min_distancia = distancia;
                        for(int j=0; j < DIM; j++)
                            vector_min[j] = vector_temp[j];
                    }
                }

                //Envia la minima distancia calculada en este proceso, hacia el proceso 0
                MPI_Send(&min_distancia, 1, MPI_DOUBLE, 0, 4, MPI_COMM_WORLD);

                //Envia el vector mas cercano al vector consulta encontrado en este proceso, hacia el proceso 0
                MPI_Send(&vector_min, DIM, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD);

				break;
			}
		}
	}

    MPI_Finalize();
    return 0;

}