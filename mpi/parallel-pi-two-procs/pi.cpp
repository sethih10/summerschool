#include <cstdio>
#include <cmath>
#include <mpi.h>

constexpr int n = 840;

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);
  printf("Computing approximation to pi with N=%d\n", n);
  

  

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int istart = 1;
  int istop = n;

  // istart = 
  if (rank%2 == 0)
  {
    istart = 1;
    istop = n/2;
  }

  else
  {
    istart = n/2;
    istop = n;
  }
  

  double pi = 0.0;
  for (int i=istart; i <= istop; i++) {
    double x = (i - 0.5) / n;
    pi += 1.0 / (1.0 + x*x);
  }

  pi *= (2.0*4.0) / n;

  if (rank%2 == 0)
  {
    double temp;
    MPI_Recv(&temp, 1, MPI_DOUBLE, 1, 111, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    pi += temp;
    pi = pi/2;
  }

  else
  {
    MPI_Send(&pi, 1, MPI_DOUBLE, 0, 111, MPI_COMM_WORLD);
  }


  MPI_Finalize();
  //printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", pi, M_PI);

  if (rank%2 == 0)
  {
    printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", pi, M_PI);
    
  }

}
