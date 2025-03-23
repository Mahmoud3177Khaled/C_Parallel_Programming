#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {

    int my_rank;
    int p;
    int source;
    int dest;
    int tag = 0;
    MPI_Status status;
    int result = 0;
    int final_result = 0;

    MPI_Init(&argc, &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    if(my_rank == 0) {
        int arr[10] = {1,2,3,4,5,6,7,8,9,10};

        for (int source = 1; source < p; source++)
        {
            MPI_Send(&arr[source*2], 2, MPI_INT, source, tag, MPI_COMM_WORLD);
        }

        final_result += arr[0] + arr[1];

        for (int source = 1; source < p; source++)
        {
            int x = 0;
            MPI_Recv(&x, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

            final_result += x;
        }

        printf("%d\n", final_result);

    } else {
        int arr[2];
        MPI_Recv(&arr, 2, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

        result = arr[0] + arr[1];

        MPI_Send(&result, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        

    }
    // if(my_rank != 0) {
    //     sprintf(message, "hello from proccess: %d !", my_rank);
    //     dest = 0;

    //     MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

    // } else {

    //     for (int source = 1; source <= p-1; source++) {
    //         MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);

    //         printf("%s\n", message);
    //     }
    // }

    MPI_Finalize();
    return 0;
}
