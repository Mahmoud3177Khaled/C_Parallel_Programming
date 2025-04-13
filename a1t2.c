#include <stdio.h>
#include "mpi.h"
#include <string.h>

void main(int argc, char** argv) {
    
    // mpi variables
    int my_rank;
    int p;
    int tag = 0;
    MPI_Status status;
 
    // problem variables
    int full_str_size = 0;
    int partition_size = 0;
    // int max_index_on_this_proc = -999999;
    // int max_on_this_proc = -999999;



    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(my_rank == 0) {

        printf("\nHello from master process!\n\n");
        printf("Number of slave processes is %d\n\n", p-1);

        // read string size - master
        printf("Please enter size of string: \n");
        scanf("%d", &full_str_size);
        char full_str[full_str_size];
        char full_str_enc[full_str_size];

        
        // read string elements - master
        printf("Please enter the string: \n");
        scanf("%s", full_str);
        
        full_str_size = strlen(full_str)+1;
        partition_size = (full_str_size/(p-1)); // master wont get a partition

        // distribute string size and partitions to slaves - master
        printf("partition size is %d\n\n", partition_size);
        for (int i = 1; i < p; i++) {  // p-1 as master wont get a partition

            if(i == p-1) {
                int old_partition_size = partition_size;
                partition_size += (full_str_size % (p-1));

                printf("%d ||| %d", old_partition_size, partition_size);

                MPI_Send(&full_str_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&partition_size, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
                MPI_Send(&full_str[(i-1) * old_partition_size], partition_size, MPI_CHAR, i, 2, MPI_COMM_WORLD);
                partition_size = old_partition_size;
                break;    
            }

            MPI_Send(&full_str_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&partition_size, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&full_str[(i-1) * partition_size], partition_size, MPI_CHAR, i, 2, MPI_COMM_WORLD);
        }

        // ------------------------------ after slaves processing ---------------------------------------


        // recive substrings from slaves - master
        for (int i = 1; i < p; i++) {
            if(i == p-1) {
                MPI_Recv(&full_str_enc[(i-1) * partition_size], partition_size+(full_str_size % (p-1)), MPI_CHAR, i, 3, MPI_COMM_WORLD, &status);
                break;
            }
            MPI_Recv(&full_str_enc[(i-1) * partition_size], partition_size, MPI_CHAR, i, 3, MPI_COMM_WORLD, &status);
        }

        printf("\nMaster process announce the final string is %s\n",  full_str_enc);
        printf("\nThanks for using our program :)\n");


    } else {
        
        // recieve the string size and partition size fom master - slaves
        MPI_Recv(&full_str_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&partition_size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        // recive the string partition elements - slaves
        char my_str[partition_size];
        char my_str_enc[partition_size];
        MPI_Recv(my_str, partition_size, MPI_CHAR, 0, 2, MPI_COMM_WORLD, &status);

        // encrypt the partition - slaves
        for (int i = 0; i < partition_size; i++) {
            if(my_str[i] == '\0') {
                my_str_enc[i] = my_str[i];
                break;
            }

            my_str_enc[i] = my_str[i] + 3;
        }
        
        printf("Hello from slave#%d, i encrypted %s to %s\n.", my_rank, my_str+'\0', my_str_enc+'\0');

        // send encrypted string back to master to be combined with others - slaves
        MPI_Send(&my_str_enc, partition_size, MPI_CHAR, 0, 3, MPI_COMM_WORLD);
    }

    MPI_Finalize();



}