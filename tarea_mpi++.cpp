/*
Integrantes:
    -Boris Mora
    -Nicolas Muñoz
*/

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
	int num = 100, num_2, etiqueta, emisor, count, flag=0;
    MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc); //Entrega el nro total de nodos
	MPI_Comm_rank(MPI_COMM_WORLD, &yo); //Entrega el ID del nodo actual

    list<int> vectores;
    int cant_vectores;
    int dato;
    int vector_obj[DIM];
    int limite = 0;     //Asigna la cantidad de elementos que se pasara a cada proceso

    if (yo == 0){
        printf("\n N de procesos: %d \n", nproc);
        //Lee la cantidad de vectores
        scanf("%d", &cant_vectores);
        //Comprueba los datos de entrada
        if(cant_vectores <= 0){
            printf("\nERROR: La cantidad de vectores no es valida\n");
            exit(0);
        }

        //Lee los vectores con los que se trabajara
        for(int i=0;i < cant_vectores;i++){
            for(int j=0;j < DIM;j++){
                scanf("%d", &dato);
                //Comprueba los datos de entrada
                if(dato < 0){
                    printf("\nERROR: datos de entrada para vectores no validos\n");
                    exit(0);
                }
                vectores.push_back(dato);  //Inserta el dato leido a la lista "vectores"
            }
        }

        //Lee el vector consulta
        for(int i=0;i < DIM;i++){
            scanf("%d", &dato);
            //Comprueba los datos de entrada
            if(dato < 0){
                printf("\nERROR: datos de entrada para vector objetivo no validos\n");
                exit(0);
            }
            vector_obj[i] = dato;
        }


        //El proceso 0 envia los datos leidos anteriormente al resto de procesos disponibles
        //Recorre cada uno de los procesos restantes
        for(int i=1;i < nproc;i++){

            //Envia el vector consulta al proceso destino
            MPI_Send(&vector_obj, DIM, MPI_INT, i, 1, MPI_COMM_WORLD);

            //Envia la cantidad de datos asignados al proceso destino
            limite = rand() % 5 + 2;    // CAMBIAR ESTO !!!!!!!!!!!!!
            MPI_Send(&limite, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
 
            //Envia los datos asignados al proceso destino
            for(int j=0;j < limite; j++){
                MPI_Send(&j, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
            }
        }
        
        
    }


    
    if (yo != 0)
	{
		while(flag == 0)
		{
			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
			if (flag != 0) //Mensaje arrivado listo para lectura
			{  
				//Recibe el vector consulta
				MPI_Recv(&vector_obj, DIM, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                //Recibe la cantidad de datos asignados a este proceso
                MPI_Recv(&limite, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf(" yo: %d   |   limite: %d \n", yo, limite);

                //Recibe los datos asignados a este proceso
                for(int j=0;j < limite; j++){
                    MPI_Recv(&dato, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    //printf("yo: %d,   dato: %d\n", yo, dato);
                }
                
                //IMPRIME EL VECTOR OBJETIVO EN CADA PROCESO
                /*
                for(int i=0;i < DIM;i++){
                    printf("yo: %d,   dato: %d\n", yo, vector_obj[i]);
                }
				printf("\n");
                */
				break;
			}
		}
	}

    /* BARRERA : Todos los nodos esperan a que todos alcancen este punto en el codigo */
	MPI_Barrier(MPI_COMM_WORLD);



    MPI_Finalize();

    return 0;

}
