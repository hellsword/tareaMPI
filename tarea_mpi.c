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

#define DIM 4

void main(int argc, char **argv){

    int nproc; /* Número de procesos */
	int yo; /* Mi dirección: 0<=yo<=(nproc-1) */
	int num = 100, num_2, etiqueta, emisor, count, flag=0;
    MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc); //Entrega el nro total de nodos
	MPI_Comm_rank(MPI_COMM_WORLD, &yo); //Entrega el ID del nodo actual

    int cant_vectores = 0;

    if (yo == 0){
        //Lee la cantidad de vectores
        scanf("%d", &cant_vectores);
        //Comprueba los datos de entrada
        if(cant_vectores <= 0){
            printf("\nERROR: datos de entrada no validos\n");
            exit(0);
        }
        MPI_Send(&cant_vectores, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
    }

    /* BARRERA : Todos los nodos esperan a que todos alcancen este punto en el codigo */
	MPI_Barrier(MPI_COMM_WORLD);

    //Lee los vectores y los envia
    int dato;
    if (yo == 0){
        for(int i=0;i < cant_vectores;i++){
            for(int j=0;j < DIM;j++){
                scanf("%d", &dato);
                //Comprueba los datos de entrada
                if(dato < 0){
                    printf("\nERROR: datos de entrada no validos\n");
                    exit(0);
                }
                MPI_Send(&dato, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
            }
        }
    }
    
    int vectores[cant_vectores][DIM];

    //Recibe los vectores y los guarda en una matriz
    int dato2;
    if (yo == 1)
	{
		while(flag == 0)
		{
			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
			if (flag != 0) //Mensaje arrivado listo para lectura
			{

                MPI_Recv(&cant_vectores, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
                
                    for(int i=0;i < cant_vectores;i++){
                        for(int j=0;j < DIM;j++){
                            MPI_Recv(&dato2, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
                            printf("%d  ", dato2);
                        }
                        printf("\n");
                    }
                
			}
		}
	}

    
   

    /* BARRERA : Todos los nodos esperan a que todos alcancen este punto en el codigo */
	MPI_Barrier(MPI_COMM_WORLD);


  


    MPI_Finalize();

}
