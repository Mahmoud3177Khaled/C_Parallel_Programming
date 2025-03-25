#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {

    int my_rank;
    int p;
    int tag = 0;
    MPI_Status status;
    int scaler = 0;
    int element = 0;
    int result = 0;
    int arr1[3];
    int arr2[3];

    MPI_Init(&argc, &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    if(my_rank == 0) {
        scaler = 3;
        arr1[0] = 2;
        arr1[1] = 3;
        arr1[2] = 1;
        // arr1[3] = 4;
    }
    
    MPI_Bcast(&scaler, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(arr1, 1, MPI_INT, &element, 1, MPI_INT, 0, MPI_COMM_WORLD);
    result = scaler * element;
    
    if(my_rank != 0) {
        MPI_Send(&result, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
    }

    if(my_rank == 0) {
        arr2[0] = result;

        for (int source = 1; source < p; source++) {
            MPI_Recv(&element, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
            arr2[source] = element;
        }

        for (int source = 0; source < p; source++) {
            printf("%d ", arr2[source]);
        }

        printf("\n");
        
    }

    MPI_Finalize();
    return 0;
}
