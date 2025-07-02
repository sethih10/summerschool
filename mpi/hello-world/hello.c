#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

    // TODO: say hello! in parallel
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Hello from rank %d of size %d\n", rank, size);

    int len;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(name, &len);

    printf("Node name %s with length %d\n", name, len);

    if (rank == 0)
        printf("Total number of MPI processes %d\n", size);
    if (rank == size-1)
        printf("I am the last but not least\n");
    if (rank == 42)
        printf("I'm the Answer to the Ultimate Question of Life, the Universe, and Everything!\n");
    MPI_Finalize();
}
