#include<stdio.h>
#include"mpi.h"
#include<omp.h>
#include<math.h>


void main(int argc, char** argv) {

    int my_rank;
    int p;
    int tag = 0;
    MPI_Status status;

    int n = 15;
    int partition_size;
    int sum = 0;
    
    
    MPI_Init(&argc, &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    if(my_rank == 0) {
        printf("Please enter array size: \n");
        scanf("%d", &n);

    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
     
    if(n%p != 0) {
        if(my_rank == 0) {
            printf("Array size should be divisible ny processes number...\n");
        }
        MPI_Finalize();
        return;
    }

    partition_size = n/p;
    int my_arr[partition_size];
    int arr[n];
    int total_sum = 0;

    if(my_rank == 0) {

        printf("Pleae enter array elemnts of size %d: \n", n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }
    
    }

    MPI_Scatter(arr, partition_size, MPI_INT, my_arr, partition_size, MPI_INT, 0, MPI_COMM_WORLD);

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < partition_size; i++) {
        sum += my_arr[i];
        printf(" | thread %d working...", omp_get_thread_num());
    }
    printf("  ---->  Process %d summed: %d\n", my_rank, sum);


    MPI_Reduce(&sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(my_rank == 0) {
        printf("Master announces that the total sum is: %d\n", total_sum);
    }
    
    MPI_Finalize();
    
    
}