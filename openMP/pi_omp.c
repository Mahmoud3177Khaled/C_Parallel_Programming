#include <stdio.h>
#include <omp.h>

int main() {
    long num_steps = 100000000; // Increase for more accuracy
    double step = 1.0 / (double)num_steps;
    double sum = 0.0;

    #pragma omp parallel
    {
        double x;
        double local_sum = 0.0;

        #pragma omp for
        for (long i = 0; i < num_steps; i++) {
            x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }

        #pragma omp atomic
        sum += local_sum;
    }

    double pi = step * sum;
    printf("Approximated pi = %.15f\n", pi);
    return 0;
}
