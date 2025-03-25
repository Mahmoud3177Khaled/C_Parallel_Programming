#include <stdio.h>
#include <stdlib.h>

int main() {
    int rows = 3000, cols = 3000;

    // Allocate memory for matrices
    int **arr = (int**)malloc(rows * sizeof(int *));
    int **result = (int**)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        arr[i] = (int*)malloc(cols * sizeof(int));
        result[i] = (int*)malloc(cols * sizeof(int));
    }

    // Initialize matrix with example values
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            arr[i][j] = i * cols + j;
        }
    }

    // Perform matrix multiplication
    for (int i = 0; i < rows; i++) {
        for (int k = 0; k < cols; k++) {
            // result[i][j] = 0;
            for (int j = 0; j < cols; j++) {
                result[i][j] += arr[i][k] * arr[k][j];
            }
        }
    }

    // // Print result matrix
    // printf("Result Matrix:\n");
    // for (int i = 0; i < rows; i++) {
    //     for (int j = 0; j < cols; j++) {
    //         printf("%d ", result[i][j]);
    //     }
    //     printf("\n");
    // }

    // Free allocated memory
    for (int i = 0; i < rows; i++) {
        free(arr[i]);
        free(result[i]);
    }
    free(arr);
    free(result);

    return 0;
}
