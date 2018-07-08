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

#define DIM 1000

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

    //Se trabaja con el proceso 0
    if (yo == 0){

        clock_t t_ini, t_fin;
        double secs;
        t_ini = clock();


        printf("\n N de procesos: %d \n\n", nproc);
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

        double min_distancia = INFINITY;

        if(nproc == 1){
            //Calcula la distancia euclidiana entre el vector objetivo y cada uno de los vectores en la BD
            
            for(int i=0;i < cant_vectores;i++){

                //printf("\n Vector: %d\n", i+1);
                //Efectua la suma de las diferencias al cuadrado punto a punto entre ambos vectores 
                int resta = 0;
                for(int j=0;j < DIM;j++){
                    num = vectores.front();
                    resta = resta + pow( ( num - vector_obj[j] ), 2 );
                    vectores.pop_front();
                }
                //printf("    resta = %d\n", resta);

                //Calcula la raiz cuadrada de la diferencia obtenida anteriormente
                double distancia = sqrt(resta);
                if(distancia < min_distancia)
                    min_distancia = distancia;
                //printf("    distancia = %lf \n", distancia);
                //printf("\n");
            }
        }
        else{
            //El proceso 0 envia los datos leidos anteriormente al resto de procesos disponibles
            //Recorre cada uno de los procesos restantes

            //Calcula el modulo de cantidad de vectores por la cantidad de procesos disponibles
            int aux = cant_vectores%(nproc-1); 

            double distancia;

            for(int i=1;i < nproc;i++){

                //Envia el vector consulta al proceso destino
                MPI_Send(&vector_obj, DIM, MPI_INT, i, 1, MPI_COMM_WORLD);

                //Se calcula la cantidad de elementos que se enviaran a cada proceso
                if(aux == 0){
                    limite = cant_vectores/(nproc-1);
                    //printf("porc: %d  |  elementos: %d\n", i, limite);
                }
                else{
                    limite = cant_vectores%(nproc-1)+1;
                    aux--;
                    //printf("porc: %d  |  elementos: %d\n", i, limite);
                }
                //printf("limite: %d\n", limite);

                //Envia la cantidad de datos asignados al proceso destino
                MPI_Send(&limite, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
    
                //Envia los datos asignados al proceso destino
                for(int j=0;j < limite*DIM; j++){
                    num = vectores.front();
                    MPI_Send(&num, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
                    vectores.pop_front();
                }

                //Recibe la menor distancia calculada en cada proceso
                MPI_Recv(&distancia, 1, MPI_DOUBLE, i, 4, MPI_COMM_WORLD, &status);
                //printf("    menor distancia recibida = %lf \n", distancia);
                if(distancia < min_distancia)
                    min_distancia = distancia;
                
            }
        }
        
        printf("\n    menor distancia = %lf \n", min_distancia);

        t_fin = clock();
        secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
        printf("%.16g milisegundos\n", secs * 1000.0);
        
    }


    
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
                //printf(" yo: %d   |   limite: %d \n", yo, limite);

                //Recibe los datos asignados a este proceso
                //printf("yo: %d\n", yo);
                double min_distancia = INFINITY;

                for(int i=0;i < limite; i++){

                    int resta = 0;

                    for(int j=0; j < DIM; j++){
                        MPI_Recv(&dato, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        //printf("    %d", dato);

                        //Realiza y suma las restas entre cada punto del vector consulta y el vector actual 
                        //necesarias para calcular la distancia. Cada resta se eleva al cuadrado
                        resta = resta + pow( ( dato - vector_obj[j] ), 2 ); 
                    }
                    //printf("    resta = %d\n", resta);

                    //Obtiene la distancia euclidiana calculando la raiz de la sumaria de restas anterior
                    double distancia = sqrt(resta);    
                    if(distancia < min_distancia)
                        min_distancia = distancia;
                    //printf("distancia = %lf \n", distancia);
                    //printf("\n");
                }
                //printf("    menor distancia = %lf \n", min_distancia);
                MPI_Send(&min_distancia, 1, MPI_DOUBLE, 0, 4, MPI_COMM_WORLD);
                //printf("\n");
                
                
                
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
