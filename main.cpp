#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<mpi.h>

int main(int argc, char** argv){

    int tamano, procesador;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &tamano); //obtenemos el tamaño del procesador
	MPI_Comm_rank(MPI_COMM_WORLD, &procesador);

    int numero=1;
    char letra;
    char *buffer;
    size_t bufsize = 64;
    size_t characters;

    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }
    int suma=0;
    FILE *ficheroEntrada;
    ficheroEntrada = fopen ( "utem.csv", "r" );
    while(!feof(ficheroEntrada)){
        if(procesador==0){
            int receptor = 1;
            getline(&buffer,&bufsize,ficheroEntrada);
            MPI_Send(&numero, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            }else if(procesador == 1){
                int proc= 0;
                MPI_Recv(&numero, 1, MPI_INT, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                suma=suma+1;
                printf("procesador 1 recv numero %d desde el procesador numero 0",numero);
            }
        if(procesador==2){
        int receptor = 3;
        getline(&buffer,&bufsize,ficheroEntrada);
        MPI_Send(&numero, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            }else if(procesador == 1){
                int proc= 0;
                MPI_Recv(&numero, 1, MPI_INT, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                suma=suma+1;
                printf("procesador 1 recv numero %d desde el procesador numero 0",numero);
            }
        }
    suma= suma-1;
    printf("La cantidad de lineas del fichero es: %d\n", suma);
    fclose ( ficheroEntrada );
    MPI_Finalize();
    return 0;
}
