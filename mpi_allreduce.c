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
        result = 2;
    }
    
    MPI_Bcast(&result, 1, MPI_INT, 0, MPI_COMM_WORLD);
    result += my_rank;
    MPI_Allreduce(&result, &final_result, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);

    // for(int proc = 0; proc < p; proc++) {
        printf("proc num: %d | %d\n", my_rank, final_result);
    // }

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
