#include <cstdio>
#include <omp.h>
int main()
{
    printf("Hello world!\n");
    int num_threads = omp_get_num_threads();
    printf("Number of threads %d\n", num_threads);
#pragma omp parallel
    {
        int thread_num = omp_get_thread_num();
        printf("X, thread_num %d\n", thread_num);
    }
    return 0;
}
