#include <cstdio>
#include <vector>
#include <mpi.h>

#define MAX_PRINT_SIZE 12

void init_buffer(std::vector<int> &buffer);
void print_buffer(std::vector<int> &buffer);


int main(int argc, char *argv[])
{
    int size, rank, buf_size=10000;
    std::vector<int> buf(buf_size);
    std::vector<int> recv_buf(buf_size);

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
    // TODO: Implement the broadcast of the array buf

    //int count = buf_size/size;
    //recv_buf.resize(count);
    //int* actual_recv = recv_buf.data();
    //if (rank == 0)
    //{
    //    //actual_recv = MPI_IN_PLACE;
    //    MPI_Scatter(buf.data(), count, MPI_INT, MPI_IN_PLACE, count, MPI_INT, 0, MPI_COMM_WORLD);
//
    //}
    //else
    //    MPI_Scatter(buf.data(), count, MPI_INT, buf.data(), count, MPI_INT, 0, MPI_COMM_WORLD);


    int index = buf_size/size;
    if (rank == 0)
    {
        for(int i = 1; i<size; i++)
        {
            printf("Send rank %d with buffer size %d\n", i, index);
            MPI_Send(buf.data() + i*index, index, MPI_INT, i, 111, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(buf.data(), index, MPI_INT, MPI_ANY_SOURCE, 111, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    //MPI_Bcast(buf.data(), buf.size(), MPI_INT, 0, MPI_COMM_WORLD);

    //if (rank == 0)
    //{
    //    for(int i = 1; i<size; i++)
    //    {
    //        MPI_Send(buf.data(), buf.size(), MPI_INT, i, 111, MPI_COMM_WORLD);
    //    }
    //}
    //else
    //{
    //    MPI_Recv(buf.data(), buf.size(), MPI_INT, MPI_ANY_SOURCE, 111, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //}

    /* End timing */
    double t1 = MPI_Wtime();

    /* Print data that was received */
    print_buffer(buf);
    if (rank == 0) {
        printf("Time elapsed: %6.8f s\n", t1 - t0);
    }
    //print_buffer(recv_buf);
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
