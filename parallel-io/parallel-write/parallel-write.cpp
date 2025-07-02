#include <vector>
#include <cstdio>
#include <string>

#include <mpi.h>


// How many integers to write, total from all MPI processes
static constexpr size_t numElements = 32;

/* Enables or disables debug printing of file contents. Printing is not practical for large files,
so we enable/disable this based on 'numElements'. */
static constexpr bool doDebugPrint = (numElements <= 100);


// Debugging helper, prints out file contents. You don't have to modify this
void debug_read_file(const char* filename);

void single_writer(const std::vector<int>& localData, const char* filename) {
    // Gets called from all MPI ranks. 'localData' contains different data on each rank.
    // TODO: Gather contents of 'localData' to one MPI process and write it all to file 'filename' ("spokesperson" strategy).
    // The output should be ordered such that data from rank 0 comes first, then rank 1, and so on



    int rank, ntasks;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);


    

    if (rank==0)
    {
        FILE *fp = fopen(filename, "w");
        fwrite(localData.data(), sizeof(int), localData.size(), fp);
        for(int i = 1; i<ntasks;i++)
        {
            
            std::vector<int> recv_buffer(localData.size());
            MPI_Recv(recv_buffer.data(), recv_buffer.size(), MPI_INT,i, 111, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            fwrite(recv_buffer.data(), sizeof(int), recv_buffer.size(), fp);
            
        }
        fclose(fp);
        
    }
    else
    {
        MPI_Request request;
        MPI_Isend(localData.data(), localData.size(), MPI_INT, 0, 111, MPI_COMM_WORLD, &request);
    }

    

    // You can assume that 'localData' has same length in all MPI processes:
    const size_t numElementsPerRank = localData.size();

}

void collective_write(const std::vector<int>& localData, const char* filename) {
    // TODO: Like single_writer(), but implement a parallel write using MPI_File_write_at_all()

    int rank, ntasks;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    MPI_File file;

    MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

    MPI_File_set_size(file, 0);
    int numElementsPerRank = localData.size();

    MPI_Offset offset = (MPI_Offset)(rank*numElementsPerRank*sizeof(int));

    MPI_File_write_at_all(file, offset, localData.data(), numElementsPerRank, MPI_INT, MPI_STATUS_IGNORE);
    MPI_File_close(&file);

}

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);

    int rank, ntasks;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    if (numElements % ntasks != 0) {
        if (rank == 0) {
            fprintf(stderr, "numElements (%zu) must be divisible by the number of MPI tasks (%d)!\n", numElements, ntasks);
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    const size_t numElementsPerRank = numElements / ntasks;

    // Create data array, each element initialized to value 'rank'
    std::vector<int> localData(numElementsPerRank, rank);

    // Print some statistics
    if (rank == 0) {
        printf("Writing total of %zu integers, %zu from each rank.\n", numElements, numElementsPerRank);
        const size_t bytes = numElements * sizeof(int);
        printf("Total bytes to write: %zu (%zu MB)\n", bytes, bytes / 1024 / 1024);
        fflush(stdout);
    }


    // ########## "Spokesperson" write
    std::string filename = "single_writer.dat";

    single_writer(localData, filename.c_str());

    if (rank == 0 && doDebugPrint) {
        printf("[%s] file contents:\n", filename.c_str());
        debug_read_file(filename.c_str());
    }

    // ########## Collective write
    filename = "collective_write.dat";

    collective_write(localData, filename.c_str());

    if (rank == 0 && doDebugPrint) {
        printf("[%s] file contents:\n", filename.c_str());
        debug_read_file(filename.c_str());
    }

    //~

    MPI_Finalize();
    return 0;
}


void debug_read_file(const char* filename) {

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        FILE *fileptr = fopen(filename, "rb");

        if (fileptr != NULL) {

            int value;
            while (fread(&value, sizeof(int), 1, fileptr) == 1) {
                printf("%d", value);
            }

            fclose(fileptr);

            printf("\n");
            fflush(stdout);
        } else {
            fprintf(stderr, "Failed to open file %s for debug printing!\n", filename);
        }
    }
}
