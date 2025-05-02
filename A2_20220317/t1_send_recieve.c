#include<stdio.h>
#include"mpi.h"
#include<math.h>

int main(int argc, char** argv) {

    // mpi variables
    int my_rank;
    int p;
    int tag = 0;
    MPI_Status status;
    
    // problem variables
    int partition = 0;
    int my_count = 0;
    int x = 0;
    int y = 0;
    
    
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    if(my_rank == 0) {
        

        printf("Hello from master process.\n");

        // printf("Enter start x: \n");
        // scanf("%d", &x);
        
        // printf("Enter end y: \n");
        // scanf("%d", &y);

        x = 1;
        y = 10;

        partition = floor((double)((y-x)+1)/(p-1));
        
        // printf("partition size is: %d", partition);
        
        // MPI_Bcast(&partition, 1, MPI_INT, 0, MPI_COMM_WORLD);
        // MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
        // MPI_Bcast(&y, 1, MPI_INT, 0, MPI_COMM_WORLD);

        for(int i = 1; i < p; i++) {
            MPI_Send(&partition, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&x, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&y, 1, MPI_INT, i, 2, MPI_COMM_WORLD);

        }

        // after coputations done by slaves 

        int total_count = 0;
        for (int i = 1; i < p; i++) {
            int buff = 0;
            MPI_Recv(&buff, 1, MPI_INT, i, 3, MPI_COMM_WORLD, &status);

            total_count += buff;
        }
        
        printf("\n Master announces total prime numbers count is: %d\n", total_count);

        
        
    } else {

        MPI_Recv(&partition, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&x, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&y, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);

        
        int my_x = x+((my_rank-1)*partition);
        int my_y = x+(my_rank*partition)-1;
        
        if(my_rank == p-1) {
            my_y += ((y-x)+1)%(p-1);

        }

        for (int i = my_x; i <= my_y; i++) {
            int prime = 1;

            if(i <= 1) {
                continue;
            }

            if(i == 2) {
                my_count++;
                continue;
            }

            for (int j = 2; j <= (int)sqrt(i); j++) {

                if(i%j == 0) {
                    prime = 0;
                    break;
                }
                
            }

            if(prime) {
                my_count++;
                // printf("%d -> ", i);

            }
        }

        printf("slave%d: counting primes from %d to %d .... count: %d.\n", my_rank, my_x, my_y, my_count); 

        MPI_Send(&my_count, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);

    }


    // MPI_Reduce(&my_count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // if(my_rank == 0) {
    //     printf("\n Master announces total prime numbers count is: %d\n", total_count);

    // }


    MPI_Finalize();
}


