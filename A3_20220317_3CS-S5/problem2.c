#include<stdio.h>
#include<omp.h>
#include<math.h>


void main(int argc, char** argv) {

    int n;
    double mean = 0;
    double variance = 0;
    double stdDeviation = 0;


    printf("Please enter array size: \n");
    scanf("%d", &n);

    int arr[n];

    printf("Pleae enter array elemnts of size %d: \n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    #pragma omp parallel for reduction(+:mean)
    for (int i = 0; i < n; i++) {
        printf("%d working...\n", omp_get_thread_num());
        mean += arr[i];
    }
    mean = mean/n;
    // printf("%f\n", mean);

    #pragma omp parallel for reduction(+:variance)
    for (int i = 0; i < n; i++) {
        printf("%d working...\n", omp_get_thread_num());
        variance += pow(arr[i]-mean, 2);
    }    
    variance = variance/n;
    stdDeviation = sqrt(variance);

    printf("Standard deviation of those %d elements = %f\n", n, sqrt(variance));

    
    
}