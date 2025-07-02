// Main solver routines for heat equation solver

#include <mpi.h>

#include "heat.hpp"

// Exchange the boundary values
void exchange(Field& field, const ParallelData parallel)
{

    double* sbuf;
    double* rbuf;
    // TODO start: implement halo exchange

    // You can utilize the data() method of the Matrix class to obtain pointer
    // to element, e.g. field.temperature.data(i, j)

    sbuf = field.temperature.data(1, 0);
    rbuf = field.temperature.data(field.nx+1,0);

    // Send to up, receive from down
    MPI_Send(sbuf, field.ny + 2, MPI_DOUBLE, parallel.nup, 111, MPI_COMM_WORLD);
    MPI_Recv(rbuf, field.ny + 2, MPI_DOUBLE, parallel.ndown,111, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    sbuf = field.temperature.data(field.nx, 0);
    rbuf = field.temperature.data(0,0);

    // Send to down, receive from up
    MPI_Send(sbuf, field.ny + 2, MPI_DOUBLE, parallel.ndown, 000, MPI_COMM_WORLD);
    MPI_Recv(rbuf, field.ny + 2, MPI_DOUBLE, parallel.nup, 000, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


    // TODO end
}

// Update the temperature values using five-point stencil */
void evolve(Field& curr, const Field& prev, const double a, const double dt)
{

  // Compilers do not necessarily optimize division to multiplication, so make it explicit
  auto inv_dx2 = 1.0 / (prev.dx * prev.dx);
  auto inv_dy2 = 1.0 / (prev.dy * prev.dy);

  // Determine the temperature field at next time step
  // As we have fixed boundary conditions, the outermost gridpoints
  // are not updated.
  for (int i = 1; i < curr.nx + 1; i++) {
    for (int j = 1; j < curr.ny + 1; j++) {
            curr(i, j) = prev(i, j) + a * dt * (
	       ( prev(i + 1, j) - 2.0 * prev(i, j) + prev(i - 1, j) ) * inv_dx2 +
	       ( prev(i, j + 1) - 2.0 * prev(i, j) + prev(i, j - 1) ) * inv_dy2
               );
    }
  }

}
