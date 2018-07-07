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

    if (yo == 0){
        printf("\n N de procesos: %d \n", nproc);
        //Lee la cantidad de vectores
        scanf("%d", &cant_vectores);
        //Comprueba los datos de entrada
        if(cant_vectores <= 0){
            printf("\nERROR: datos de entrada no validos\n");
            exit(0);
        }
        //MPI_Send(&cant_vectores, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);


        for(int i=0;i < cant_vectores;i++){
            for(int j=0;j < DIM;j++){
                scanf("%d", &dato);
                //Comprueba los datos de entrada
                if(dato < 0){
                    printf("\nERROR: datos de entrada no validos\n");
                    exit(0);
                }
                vectores.push_back(dato);
                //MPI_Send(&dato, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
            }
        }


        for(int i=0;i < DIM;i++){
            scanf("%d", &dato);
            //Comprueba los datos de entrada
            if(dato < 0){
                printf("\nERROR: datos de entrada no validos\n");
                exit(0);
            }
            vector_obj[i] = dato;
            //MPI_Send(&dato, 1, MPI_INT, 1, 3, MPI_COMM_WORLD);
        }
        
    }


    /* BARRERA : Todos los nodos esperan a que todos alcancen este punto en el codigo */
	MPI_Barrier(MPI_COMM_WORLD);


    if (yo == 0){
        int emisor = 1;
        for(int i=0;i < cant_vectores;i++){
            for(int j=0;j < DIM;j++){
                num = vectores.front();
                MPI_Send(&num, 1, MPI_INT, emisor, 1, MPI_COMM_WORLD);
                vectores.pop_front();
                emisor++;
                if(emisor >= nproc)
                    emisor = 1;
            }
            //printf("\n");
        }
    }


    int dato2;
    if (yo != 0)
	{
        MPI_Recv(&num_2, count, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        printf("yo: %d,   dato: %d\n", yo, dato2);
           
	}

    



/*
    //Recibe los vectores y los guarda en una matriz
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
                        vectores.push_back(dato2);
                    }
                }

                for(int i=0;i < DIM;i++){
                    MPI_Recv(&dato2, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);
                    vector_obj[i] = dato2;
                }
                
			}
		}
	}



    if (yo == 1){
        printf("\nLista de Vectores: \n");
        int suma = 0;
        for(int i=0;i < cant_vectores;i++){
            for(int j=0;j < DIM;j++){
                num = vectores.front();
                printf("%d    ", num);
                vectores.pop_front();
            }
            printf("\n");
        }
    }


    if (yo == 1){
        printf("\nVector Objetivo: \n");
        int suma = 0;
        for(int i=0;i < DIM;i++){
            printf("%d    ", vector_obj[i]);
        }
        printf("\n");
    }

*/
    /* BARRERA : Todos los nodos esperan a que todos alcancen este punto en el codigo */
	MPI_Barrier(MPI_COMM_WORLD);

/*
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
*/

    MPI_Finalize();

    return 0;

}
