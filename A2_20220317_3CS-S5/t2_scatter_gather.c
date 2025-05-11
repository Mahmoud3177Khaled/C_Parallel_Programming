#include<stdio.h>
#include"mpi.h"
#include<string.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char** argv) {

    // mpi variables
    int my_rank;
    int p;
    int tag = 0;
    MPI_Status status;
    
    // initializing parallelism.
    MPI_Init(&argc, &argv);

    // assigning each process a rank and informing it of how many there are.
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    // problem variables
        // matrices to store input and output.
        int A[p][p];
        int B[p][p];
        int C[p][p];

        // vectors to hold temp row wise calculations.
        int rowA[p];
        int rowB[p];
        int rowC[p];

    // master does:
    if(my_rank == 0) {

        int input_choice;
        printf("select input method: \n1. Manual input\n2. Random input\n");
        scanf("%d", &input_choice);

        if(input_choice == 1) {

            // takes input for matrix A element by element.
            printf("Enter matrix A of %dx%d: \n", p, p);
            for (int i = 0; i < p; i++) {
                // printf("Row%d: ", i);
                for (int j = 0; j < p; j++) {
                    scanf("%d", &A[i][j]);
                }
            
            }

            // takes input for matrix B element by element.
            printf("Enter matrix B of %dx%d: \n", p, p);
            for (int i = 0; i < p; i++) {
                // printf("Row%d: ", i);
                for (int j = 0; j < p; j++) {
                    scanf("%d", &B[i][j]);
                }
                
            }
            
        } else {

            srand(time(NULL));

            // takes input for matrix A element by element.
            for (int i = 0; i < p; i++) {
                // printf("Row%d: ", i);
                for (int j = 0; j < p; j++) {
                    A[i][j] = rand() % 10;
                }
            
            }
    
            // takes input for matrix B element by element.
            for (int i = 0; i < p; i++) {
                // printf("Row%d: ", i);
                for (int j = 0; j < p; j++) {
                    B[i][j] = rand() % 10;
                }
                
            }

        }
        
    }
    
    // everyone does: 
    
    // scattering matrix A's rows on p processes, each getting a row of p elements,
    // and same with B.
    MPI_Scatter(A, p, MPI_INT, rowA, p, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, p, MPI_INT, rowB, p, MPI_INT, 0, MPI_COMM_WORLD);

    // each proccess loops on its row elements, adds rowA and rowB, and stores the result in rowC,
    // which will form the C matrix later...
    for (int i = 0; i < p; i++) {
        rowC[i] = rowA[i] + rowB[i];
    }
    
    // after all processes finish calculating thier rowC... we gather them all into one matrix C held by the master.
    MPI_Gather(rowC, p, MPI_INT, C, p, MPI_INT, 0, MPI_COMM_WORLD);

    // afterwards, master prints the results:    (which took me longer than the actual problem for some reason :/ )
    if(my_rank == 0) {

        
        printf("Result:\n");
        
        // looping on each row of the 3 matrices, since tey are all the same size
        for (int i = 0; i < p; i++) {

            // printing ith row of matrix A (holding the input)
            for (int j = 0; j < p; j++) {
                printf("%d", A[i][j]);
                if(j != p-1) {
                    printf(" | ");
                }
            }

            // printing a '+' sign in the middle of the matrices, else just spaces
            if(i == p/2) {
                printf("   +   ");

            } else {
                printf("       ");
            }
            
            // printing ith row of matrix B (holding the input)
            for (int j = 0; j < p; j++) {
                printf("%d", B[i][j]);
                if(j != p-1) {
                    printf(" | ");
                }
            }

            // printing a '=' sign in the middle of the matrices, else just spaces
            if(i == p/2) {
                printf("   =   ");

            } else {
                printf("       ");

            }

            // printing ith row of matrix C (holding the result)
            for (int j = 0; j < p; j++) {
                printf("%d", C[i][j]);
                if(j != p-1) {
                    printf(" | ");
                }
            }

            // new line after each row
            printf("\n");

        }
        
    }
    
        
    // finally finish the parallelism...
    MPI_Finalize();

    // Thank you! :)


}