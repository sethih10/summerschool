#include <cstdio>
#include <cmath>
#include <mpi.h>

constexpr int n = 8400;

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);
  //printf("Computing approximation to pi with N=%d\n", n);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
    printf("Computing approximation to pi with N=%d\n", n);
  int istart, istop;

  // istart = 
  istart = rank*(n/size) + 1;
  istop = (rank + 1)*(n/size);

  

  double pi = 0.0;
  for (int i=istart; i <= istop; i++) {
    double x = (i - 0.5) / n;
    pi += 1.0 / (1.0 + x*x);
  }

  //pi *= (size*4.0) / n;
  pi *= 4.0 / n;

  double send_buff;
  double recv_buff;

  send_buff = pi;
  recv_buff = -1;

  //MPI_Reduce(&send_buff, &recv_buff, 1, MPI_DOUBLE,  MPI_SUM, 0, MPI_COMM_WORLD);
  //MPI_Allreduce(&send_buff, &recv_buff, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce(MPI_IN_PLACE, &send_buff, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  

  //if (rank == 0)
  //{
  //  double temp;
  //  for(int i = 1; i<size;i++)
  //  {
  //    MPI_Recv(&temp, 1, MPI_DOUBLE,i, 111, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  //    pi += temp;
  //    //pi = pi/2;
  //  }
  //  pi = pi/size;
  //  
  //}
//
  //else
  //{
  //  MPI_Send(&pi, 1, MPI_DOUBLE, 0, 111, MPI_COMM_WORLD);
  //}


  MPI_Finalize();
  //printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", pi, M_PI);
  //printf("Start %d, end %d\n", istart, istop);

  //printf("Approximate pi=%18.16f from rank%d\n", pi, rank);

  //if (rank == 0)
  //{
  //  printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", pi, M_PI);
  //  
  //}
  
  //if (rank == 0)
    //printf("Approximate pi=%18.16f (exact pi=%10.8f)\n from rank %d\n", recv_buff, M_PI, rank);
    printf("Approximate pi=%18.16f (exact pi=%10.8f)\n from rank %d\n", send_buff, M_PI, rank);


}
