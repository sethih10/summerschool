#include <stdio.h>

#define NX 102400

int main(void)
{
    double vecA[NX], vecB[NX], vecC[NX];

    /* Initialization of the vectors */
    for (int i = 0; i < NX; i++) {
        vecA[i] = 1.0 / ((double) (NX - i));
        vecB[i] = vecA[i] * vecA[i];
    }

    // TODO start: create a data region and offload the two computations
    // so that data is kept in the device between the computations

    #pragma omp target map(tofrom: vecA[:NX], vecB[:NX]) map(tofrom:vecC[:NX])
    {

        #pragma omp teams distribute parallel for 
        for (int i = 0; i < NX; i++) {
            vecC[i] = vecA[i] + vecB[i];
        }

    }
    double res = 0.0;

    #pragma omp target map(tofrom: res)
    {
        #pragma omp teams distribute parallel for reduction(+:res) //shared(res) //reduction(+:res)
        for (int i = 0; i < NX; i++) {
            res += vecC[i] * vecB[i];
    }
    }


    // TODO end

    double sum = 0.0;
    /* Compute the check value */
    for (int i = 0; i < NX; i++) {
        sum += vecC[i];
    }
    printf("Reduction sum: %18.16f\n", sum);
    printf("Dot product: %18.16f\n", res);

    return 0;
}
