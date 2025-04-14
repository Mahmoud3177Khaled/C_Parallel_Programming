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
    int mode = 0;
    int read_mode = 0;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(my_rank == 0) {

        printf("\nHello from master process!\n\n");
        printf("Number of slave processes is %d\n\n", p-1);

        char full_str[100];
        char full_str_enc[100];

        printf("Please select mode: \n1. Encrypt\n2. Decrypt\n\n");
        scanf("%d", &mode);

        printf("Please select input mode: \n1. File\n2. Console\n\n");
        scanf("%d", &read_mode);

        // flush \n from scanf to not ruin fgets
        getchar();
        
        if(read_mode == 1) {

            char file_name[50];

            printf("\nFile name: \n");
            scanf("%s", &file_name);

            FILE* file = fopen(file_name, "r");
            fgets(full_str, sizeof(full_str), file);
            
        } else {
            // read string elements - master
            printf("Please enter the string: \n");
    
            fgets(full_str, sizeof(full_str), stdin);
            
        }
        printf("\n");

        // plus one because i wanna the \0 to be preserved in partitioning
        full_str_size = strlen(full_str)+1;
        partition_size = (full_str_size/(p-1)); // p-1 as master wont get a partition

        // distribute string size and partitions to slaves - master
        for (int i = 1; i < p; i++) {  

            if(i == p-1) {

                // last process gets the normal partition size + all thats left ... remainder 
                int old_partition_size = partition_size;
                partition_size += (full_str_size % (p-1));

                printf("\nPartitions (all | last): %d | %d", old_partition_size, partition_size-2);

                MPI_Send(&full_str_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&partition_size, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
                MPI_Send(&full_str[(i-1) * old_partition_size], partition_size, MPI_CHAR, i, 2, MPI_COMM_WORLD);

                // return partition size to normal for next calculations
                partition_size = old_partition_size;

                MPI_Send(&mode, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
                break;    
            }

            MPI_Send(&full_str_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&partition_size, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&full_str[(i-1) * partition_size], partition_size, MPI_CHAR, i, 2, MPI_COMM_WORLD);

            MPI_Send(&mode, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
        }

        // ------------------------------ after slaves processing ---------------------------------------


        // recive substrings from slaves - master
        for (int i = 1; i < p; i++) {

            if(i == p-1) {
                //if last, we take add remander to partition size as it might be bigger than normal partitions
                MPI_Recv(&full_str_enc[(i-1) * partition_size], partition_size+(full_str_size % (p-1)), MPI_CHAR, i, 3, MPI_COMM_WORLD, &status);
                break;
            }

            // recieve a normal partition and add it to the full processed string
            MPI_Recv(&full_str_enc[(i-1) * partition_size], partition_size, MPI_CHAR, i, 3, MPI_COMM_WORLD, &status);
        }

        printf("\nMaster process announce the final string is '%s'\n",  full_str_enc);
        printf("\nThanks for using our program :)\n");


    } else {
        
        // recieve the string size and partition size fom master - slaves
        MPI_Recv(&full_str_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&partition_size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        // recive the string partition elements - slaves
        // size is partition +1 cause i'll add a \0 to the partition to be printed in slave message correctly
        char my_str[partition_size+1];
        char my_str_enc[partition_size+1];
        MPI_Recv(my_str, partition_size, MPI_CHAR, 0, 2, MPI_COMM_WORLD, &status);

        // recieve mode to encrypt or decrypt
        MPI_Recv(&mode, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);

        //remove the \n captured by fgets if any
        my_str[strcspn(my_str, "\n")] = '\0';

        if(mode == 1) {
            // encrypt the partition - slaves
            for (int i = 0; i < partition_size; i++) {
                // dont encrypt null tereminator
                if(my_str[i] == '\0') {
                    my_str_enc[i] = my_str[i];
                    break;
                }
    
                my_str_enc[i] = my_str[i] + 3;
            }

        } else {
            // decrypt the partition - slaves
            for (int i = 0; i < partition_size; i++) {
                // dont decrypt null tereminator
                if(my_str[i] == '\0') {
                    my_str_enc[i] = my_str[i];
                    break;
                }
    
                my_str_enc[i] = my_str[i] - 3;
            }

        }

        // put a \0 after the partition to be printed properly
        my_str[partition_size] = '\0';
        my_str_enc[partition_size] = '\0';

        printf("Hello from slave#%d, i turned '%s' into '%s'\n.", my_rank, my_str, my_str_enc);

        // send encrypted string back to master to be combined with others - slaves
        MPI_Send(&my_str_enc, partition_size, MPI_CHAR, 0, 3, MPI_COMM_WORLD);
    }

    MPI_Finalize();

}