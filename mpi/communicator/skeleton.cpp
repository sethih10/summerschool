#include <cstdio>
#include <vector>
#include <mpi.h>

#define NTASKS 4

void init_buffers(std::vector<int> &sendbuffer, std::vector<int> &recvbuffer);
void print_buffers(std::vector<int> &buffer);


int main(int argc, char *argv[])
{
    int ntasks, rank, color;
    std::vector<int> sendbuf(2 * NTASKS), recvbuf(2 * NTASKS);

    MPI_Comm sub_comm;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (ntasks != NTASKS) {
        if (rank == 0) {
            fprintf(stderr, "Run this program with %i tasks.\n", NTASKS);
        }
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    /* Initialize message buffers */
    init_buffers(sendbuf, recvbuf);

    /* Print data that will be sent */
    print_buffers(sendbuf);

    /* TODO: create a new communicator and
     *       use a single collective communication call
     *       (and maybe prepare some parameters for the call)
     */

    if (rank < (ntasks/2))
        color = 1;
    else
        color = 2;

    MPI_Comm subcomp;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &subcomp);

    int subrank;

    MPI_Comm_rank(subcomp, &subrank);

    MPI_Reduce(sendbuf.data(), recvbuf.data(), recvbuf.size(), MPI_INT, MPI_SUM, 0, subcomp);

    printf("Rank %d in Comm world but rank %d in subcomp %d\n", rank, subrank, color);


    /* Print data that was received */
    print_buffers(recvbuf);

    MPI_Finalize();
    return 0;
}


void init_buffers(std::vector<int> &sendbuffer, std::vector<int> &recvbuffer)
{
    int rank;
    int buffersize = sendbuffer.size();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < buffersize; i++) {
        recvbuffer[i] = -1;
        sendbuffer[i] = i + buffersize * rank;
    }
}


void print_buffers(std::vector<int> &buffer)
{
    int rank, ntasks;
    int buffersize = buffer.size();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    std::vector<int> printbuffer(buffersize * ntasks);

    MPI_Gather(buffer.data(), buffersize, MPI_INT,
               printbuffer.data(), buffersize, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int j = 0; j < ntasks; j++) {
            printf("Task %2i:", j);
            for (int i = 0; i < buffersize; i++) {
                printf(" %2i", printbuffer[i + buffersize * j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}
