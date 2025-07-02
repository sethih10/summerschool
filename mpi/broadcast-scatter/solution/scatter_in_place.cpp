#include <cstdio>
#include <vector>
#include <mpi.h>

#define MAX_PRINT_SIZE 12

void init_buffer(std::vector<int> &buffer);
void print_buffer(std::vector<int> &buffer);


int main(int argc, char *argv[])
{
    int size, rank, buf_size=12;
    std::vector<int> buf(buf_size);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Initialize message buffer */
    init_buffer(buf);

    /* Print data that will be sent */
    print_buffer(buf);

    /* Start timing */
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    /* Send everywhere */
    if (buf_size % size != 0) {
        if (rank == 0) {
            fprintf(stderr, "Buffer size not divisible by the number of tasks. This program will fail.\n");
        }
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    int block_size = buf_size/size;
    int root = 0;
    if (rank == root) {
        // recvbuf = MPI_IN_PLACE -> recvcount and recvtype ignored
        MPI_Scatter(buf.data(), block_size, MPI_INT,
                    MPI_IN_PLACE, 0, 0,
                    root, MPI_COMM_WORLD);
    } else {
        // sendbuf, sendcount, and sendtype significant only on root
        MPI_Scatter(NULL, 0, 0,
                    buf.data(), block_size, MPI_INT,
                    root, MPI_COMM_WORLD);
    }

    /* End timing */
    double t1 = MPI_Wtime();

    /* Print data that was received */
    print_buffer(buf);
    if (rank == 0) {
        printf("Time elapsed: %6.8f s\n", t1 - t0);
    }

    MPI_Finalize();
    return 0;
}


void init_buffer(std::vector<int> &buffer)
{
    int rank;
    int buffersize = buffer.size();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for (int i = 0; i < buffersize; i++) {
            buffer[i] = i;
        }
    } else {
        for (int i = 0; i < buffersize; i++) {
            buffer[i] = -1;
        }
    }
}


void print_buffer(std::vector<int> &buffer)
{
    int rank, size;
    int buffersize = buffer.size();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> printbuffer(buffersize * size);

    MPI_Gather(buffer.data(), buffersize, MPI_INT,
               printbuffer.data(), buffersize, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int j = 0; j < size; j++) {
            printf("Task %2i:", j);
            for (int i = 0; i < MAX_PRINT_SIZE; i++) {
                printf(" %2i", printbuffer[i + buffersize * j]);
            }
            if (MAX_PRINT_SIZE < buffersize) {
                printf(" ...");
            }
            printf("\n");
        }
        printf("\n");
    }
}
