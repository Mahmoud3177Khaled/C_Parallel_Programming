#include<stdio.h>
#include<omp.h>

void main(int argc, char** argv) {

    int n;

    printf("Please enter matrix/vector size: \n");
    scanf("%d", &n);

    int matrix[n][n];
    int vector[n];
    int result[n];
    int row_result = 0;

    printf("please enter elements of a %dx%d matrix: \n", n, n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &matrix[i][j]);
        }
        
    }

    printf("Now please enter vector of length %d elements: \n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &vector[i]);
    }
    
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //         printf("%d ", matrix[i][j]);
    //     }
    //     printf("\n");
        
    // }

    
    // for (int i = 0; i < n; i++) {
    //     printf("%d", vector[i]);
    // }
    // printf("\n");

    #pragma omp parallel for shared(result)
    for (int i = 0; i < n; i++) {
        printf("%d working...\n", omp_get_thread_num());
        row_result = 0;

        for (int j = 0; j < n; j++) {
            row_result += vector[j]*matrix[i][j];
        }

        result[i] = row_result;

    }

    
    for (int i = 0; i < n; i++) {
        printf("%d ", result[i]);

    }
    printf("\n");


    
    
}