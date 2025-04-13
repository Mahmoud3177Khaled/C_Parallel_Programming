#include <stdio.h>
#include "mpi.h"

void main(int argc, char** argv) {
    
    // mpi variables
    int my_rank;
    int p;
    int tag = 0;
    MPI_Status status;
 
    // problem variables
    int full_arr_size = 0;
    int partition_size = 0;
    int max_index_on_this_proc = 0;
    int max_on_this_proc = 0;



    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(my_rank == 0) {

        printf("\nHello from master process!\n\n");
        printf("Number of slave processes is %d\n\n", p-1);

        // read array size - master
        printf("Please enter size of array: \n");
        scanf("%d", &full_arr_size);
        int full_arr[full_arr_size];

        partition_size = (full_arr_size/(p-1)); // master wont get a partition

        // read array elements - master
        printf("Please enter array elements: \n");
        for (int i = 0; i < full_arr_size; i++) {
            scanf("%d", &full_arr[i]);
        }

        
        // distribute array size and partitions to slaves - master
        printf("partition size is %d\n\n", partition_size);
        for (int i = 1; i < p; i++) {  // p-1 as master wont get a partition

            if(i == p-1) {
                int old_partition_size = partition_size;
                partition_size += (full_arr_size % (p-1));

                MPI_Send(&full_arr_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&partition_size, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
                MPI_Send(&full_arr[(i-1) * old_partition_size], partition_size, MPI_INT, i, 2, MPI_COMM_WORLD);
                break;    
            }

            MPI_Send(&full_arr_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&partition_size, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&full_arr[(i-1) * partition_size], partition_size, MPI_INT, i, 2, MPI_COMM_WORLD);
        }

        int indeces_arr[p];  // index 0 will be empty
        int maxs_arr[p];     // index 0 will be empty

        // recive maxs and thier indices from slaves - master
        for (int i = 1; i < p; i++) {
            MPI_Recv(&maxs_arr[i], 1, MPI_INT, i, 3, MPI_COMM_WORLD, &status);
            MPI_Recv(&indeces_arr[i], 1, MPI_INT, i, 4, MPI_COMM_WORLD, &status);
        }
        
        // get max from the formed array - master
        for (int i = 1; i < p; i++) {
            if(max_on_this_proc < maxs_arr[i]) {
                max_on_this_proc = maxs_arr[i];
                max_index_on_this_proc = indeces_arr[i];
            }
        }
        
        printf("Master process announce the final max which is %d and its index is %d\n", max_on_this_proc, max_index_on_this_proc);
        printf("\nThanks for using our program :)\n");


    } else {
        
        // recieve the array size and partition size fom master - slaves
        MPI_Recv(&full_arr_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&partition_size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        // recive the array partition elements - slaves
        int my_arr[partition_size];
        MPI_Recv(my_arr, partition_size, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);

        // find max in the partition - slaves
        for (int i = 0; i < partition_size; i++) {
            if(max_on_this_proc < my_arr[i]) {
                max_on_this_proc = my_arr[i];
                max_index_on_this_proc = i;
            }
        }

        // max_on_this_proc = my_arr[0];
        // max_index_on_this_proc = 0;

        printf("Hello from slave#%d Max number in my partition is %d and index is %d\n.", my_rank, max_on_this_proc, max_index_on_this_proc);

        // get the max index in the whole array not just in the partition - slaves
        max_index_on_this_proc += ((my_rank-1)*(partition_size-(full_arr_size % (p-1))));

        // send max and its index in the full array back to master to be comapaired with others - slaves
        MPI_Send(&max_on_this_proc, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
        MPI_Send(&max_index_on_this_proc, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
    }

    MPI_Finalize();



}