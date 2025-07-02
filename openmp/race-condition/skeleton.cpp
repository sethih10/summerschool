#include <cstdio>
#include <omp.h>


#define NX 102400

int main(void)
{
    long vecA[NX];
    long sum, psum, sumex;
    int i;

    /* Initialization of the vectors */
    for (i = 0; i < NX; i++) {
        vecA[i] = (long) i + 1;
    }

    sumex = (long) NX * (NX + 1) / ((long) 2);
    printf("Arithmetic sum formula (exact):                  %ld\n",
           sumex);

    fflush(stdout);
    sum = 0.0;
    /* TODO: Parallelize computation */
    //#pragma omp parallel for 
    //for (i = 0; i < NX; i++) {
    //    sum += vecA[i];
    //}

    



    #pragma omp parallel
    {
    
    int num_threads = omp_get_num_threads();
    printf("Num threads %d\n", num_threads);
    long sum_new = 0;
    int count = 0;

    printf("NX is %d\n", (NX/num_threads));

    #pragma omp for private(i)
    for(i = 0; i<NX; i++) // It automatically splits into different threads
    {
        int thread_num = omp_get_thread_num();
        //printf("thread num: %d\n", thread_num);

        //int index = thread_num*(NX/num_threads) + i;
        
        //printf("Index = %d\n", index);
        count += 1;
        fflush(stdout);
        sum_new += vecA[i];
        //sum_new[thread_num] += vecA[index];
       
    }
    printf("Count %d\n", count);
     #pragma omp critical
     sum += sum_new;

    

    }
    printf("Sum: %ld\n", sum);

    return 0;
}
