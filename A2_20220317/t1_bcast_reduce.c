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
    int total_count = 0; 
    int x = 0;
    int y = 0;
    
    // initializing parallelism.
    MPI_Init(&argc, &argv);

    // assigning each process a rank, and telling it how many else there are.
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    // master does:
    if(my_rank == 0) {
        

        printf("Hello from master process.\n");

        // problem input
        printf("Enter start x: \n");
        scanf("%d", &x);
        
        printf("Enter end y: \n");
        scanf("%d", &y);

        // hard coded values for acurate time measurments.
        // x = 1;
        // y = 10000000;

        // calculating partition size for each slave to work on.
        partition = floor((double)((y-x)+1)/(p-1));
        
        // debugging partition print
        // printf("partition size is: %d", partition);
        
        
    } 
    
    // bradcasting partition size, start x, and end y calculated by the master to all slaves to use.
    MPI_Bcast(&partition, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&y, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // slaves do:
    if(my_rank != 0) {
        
        // calculate the slave's x and y range it will find primes in using the partition size
        int my_x = x+((my_rank-1)*partition);
        int my_y = x+(my_rank*partition)-1;
        
        // add the remainder of numbers division by slaves number to the last slave, to handle undivisable cases
        // of numbers range and availabe slaves.
        if(my_rank == p-1) {
            my_y += ((y-x)+1)%(p-1);

        }

        // looping on each number in the slave's rnage to check if its prime or not.
        for (int i = my_x; i <= my_y; i++) {
            // assume it is a prime
            int prime = 1;

            // one and zero are never primes, and 2 is an exeption prime.
            if(i <= 1) {
                continue;
            }
            if(i == 2) {
                my_count++;
                continue;
            }

            // looping from 2 to sqrt(n) trying to find numbers cleanly dividing i.
            for (int j = 2; j <= (int)sqrt(i); j++) {

                // if so, this i can't be a prime, and we don's need to check furthur.
                if(i%j == 0) {
                    prime = 0;
                    break;
                }
                
            }

            // afterwards, if no such number was found, then i has to be a prime.
            if(prime) {
                my_count++;
                // printf("%d -> ", i);

            }
        }

        // each slave reports it calculates range, and the count it made, in it's range ofc.
        printf("slave%d: counting primes from %d to %d .... count: %d.\n", my_rank, my_x, my_y, my_count); 

    }

    // after all slaves have counted primes in thier ranges, we reduce add them all, 
    // and save the result in the master's total_count.
    MPI_Reduce(&my_count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // finally, the master prints the total_count after reduction to the console.
    if(my_rank == 0) {
        printf("\n Master announces total prime numbers count is: %d\n", total_count);

    }

    // and we finish parallelism...
    MPI_Finalize();

    // Thank you! :)
}


