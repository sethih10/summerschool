/*
 * This code uses default stream
 * Task is to 
 *   - create a stream
 *   - copy memory to/from device with that stream
 *   - launch kernel using that stream
 *   - destroy the stream
 */
#include <stdio.h>
#include <stdlib.h>
#include "../../error_checking.hpp"

// GPU kernel definition
__global__ void kernel(float *a, int n)
{
  int tid = threadIdx.x + blockIdx.x * blockDim.x;
  int stride = gridDim.x * blockDim.x;

  if (tid < n) {
    float x = (float)tid;
    float s = sinf(x);
    float c = cosf(x);
    a[tid] = a[tid] + sqrtf(s*s+c*c);
  }
}

float max_error(float *a, int n)
{
  float max_err = 0;
  for (int i = 0; i < n; i++) {
    float error = fabs(a[i]-1.0f);
    if (i<10) printf("%f ", a[i]);
    if (error > max_err) max_err = error;
  }
  printf("\n");
  return max_err;
}

int main() {
  const size_t N = 1<<9;

  constexpr int blocksize = 256;
  constexpr int gridsize =(N-1+blocksize)/blocksize;
  constexpr size_t N_bytes = N*sizeof(float);

  float *a;
  float *d_a;

  a = (float*) malloc(N_bytes);
  //HIP_ERRCHK(hipMalloc((void**)&d_a, N_bytes));

  hipStream_t stream;

  HIP_ERRCHK(hipStreamCreate(&stream));


  HIP_ERRCHK(hipMallocAsync((void**)&d_a, N_bytes, stream));

  memset(a, 0, N_bytes);

  //HIP_ERRCHK(hipMemcpy(d_a, a, N_bytes, hipMemcpyHostToDevice));
  HIP_ERRCHK(hipMemcpyAsync(d_a, a, N_bytes, hipMemcpyHostToDevice, stream));
  kernel<<<gridsize, blocksize,0,0>>>(d_a, N);
  HIP_ERRCHK(hipGetLastError());
  //HIP_ERRCHK(hipMemcpy(a, d_a, N_bytes, hipMemcpyDeviceToHost));
  HIP_ERRCHK(hipMemcpyAsync(a, d_a, N_bytes, hipMemcpyDeviceToHost, stream));

  HIP_ERRCHK(hipStreamSynchronize(stream));
  HIP_ERRCHK(hipStreamDestroy(stream));

  printf("error: %f", max_error(a, N));
  HIP_ERRCHK(hipFree(d_a));
  free(a);

}
