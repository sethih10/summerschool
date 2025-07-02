#include <hip/hip_runtime.h>
#include <math.h>
#include <stdio.h>
#include <vector>

#include "../../../error_checking.hpp"


static inline int divup(int a, int b)
{
    return (a+b-1)/b;
}

// Copy all elements using threads in a 2D grid
__global__ void copy2d(double *src, double *dst /*TODO: add arguments*/) {
    // TODO: compute row and col using
    // - threadIdx.x, threadIdx.y
    // - blockIdx.x, blockIdx.y
    // - blockDim.x, blockDim.y

    const int row = threadIdx.x;
    const int col = threadIdx.y;
    const int num_cols = blockIdx.y; 
    const int num_rows = blockIdx.x;

    // TODO: Make sure there's no out-of-bounds access
    // row must be < number of rows
    // col must be < number of columns

    if (row > num_rows || col > num_cols)
    {
        printf("Number of rows must be less than %d and number of cols must be less than %d", num_rows, num_cols);
        return;
    }

    // We're computing 1D index from a 2D index and copying from src to dst
    const size_t index = row * num_cols + col;
    dst[index] = src[index];
}

int main() {
    static constexpr size_t num_cols = 600;
    static constexpr size_t num_rows = 400;
    static constexpr size_t num_values = num_cols * num_rows;
    static constexpr size_t num_bytes = sizeof(double) * num_values;
    std::vector<double> x(num_values);
    std::vector<double> y(num_values, 0.0);

    // Initialise data
    for (size_t i = 0; i < num_values; i++) {
        x[i] = static_cast<double>(i) / 1000.0;
    }

    // TODO: Allocate + copy initial values to GPU

    double *src = nullptr;
    double *dst = nullptr;
    HIP_ERRCHK(hipMalloc(&src, num_bytes));
    HIP_ERRCHK(hipMalloc(&dst, num_bytes));

    HIP_ERRCHK(hipMemcpy(src,x.data(), num_bytes, hipMemcpyDefault));

    // TODO: Define grid dimensions
    // Use dim3 structure for threads and blocks
    dim3 threads(32, 32,1);
    dim3 blocks(divup(num_rows, threadIdx.x),divup(num_cols, threadIdx.y),1);



    //threads(32, 32,1);
    //blocks(divup(num_rows, threadIdx.x),divup(num_cols, threadIdx.y),1);


    // TODO: launch the device kernel
    LAUNCH_KERNEL(copy2d, blocks, threads, 0, 0, src, dst /*the_arguments_for_the_kernel*/);

    // TODO: Copy results back to the CPU vector y

    HIP_ERRCHK(hipMemcpy(y.data(), dst, num_bytes, hipMemcpyDefault));

    // TODO: Free device memory
    HIP_ERRCHK(hipFree(src));
    HIP_ERRCHK(hipFree(dst));

    // Check result of computation on the GPU
    double error = 0.0;
    for (size_t i = 0; i < num_values; i++) {
        error += abs(x[i] - y[i]);
    }

    printf("total error: %f\n", error);
    printf("  reference: %f at (42,42)\n", x[42 * num_rows + 42]);
    printf("     result: %f at (42,42)\n", y[42 * num_rows + 42]);

    return 0;
}

/*
 * Bonus:
 * Once you get the 2D copy kernel working,
 * try to implement it by reusing threads in the kernel.
 * In other words, write a double for loop inside the kernel
 * such that each thread may process multiple elements.
 * You can write is as a new kernel (e.g. copy2dForLoop)
 * so you don't need to change the main function except for
 * the kernel launch.
 *
 * If you need some help, check out the solution OR check
 * the 1D for-loop exercise under fundamentals.
 *
 * Extra bonus:
 * Try to implement a 3D copy with and without a triple loop.
 * */
