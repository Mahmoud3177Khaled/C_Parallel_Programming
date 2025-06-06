#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {

    int my_rank;
    int p;
    int source;
    int dest;
    int tag = 0;
    char message[100];
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(my_rank != 0) {
        sprintf(message, "hello from proccess: %d !", my_rank);
        dest = 0;

        MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

    } else {

        for (int source = 1; source <= p-1; source++) {
            MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);

            printf("%s\n", message);
        }
    }

    MPI_Finalize();
    return 0;
}
