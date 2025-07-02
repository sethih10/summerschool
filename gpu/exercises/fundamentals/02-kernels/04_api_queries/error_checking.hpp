/*
 * This header includes error checking functions and macros
 * that can be used in the exercises here.
 * Just #include this file.
 * */

#include <hip/hip_runtime.h>

#define LAUNCH_KERNEL(kernel, ...)                                             \
    launch_kernel(#kernel, __FILE__, __LINE__, kernel, __VA_ARGS__)
template <typename... Args>
void launch_kernel(const char *kernel_name, const char *file, int32_t line,
                   void (*kernel)(Args...), dim3 blocks, dim3 threads,
                   size_t num_bytes_shared_mem, hipStream_t stream,
                   Args... args) {
    int32_t device = 0;
    [[maybe_unused]] auto result = hipGetDevice(&device);

    // Helper lambda for querying device attributes
    auto get_device_attribute = [&device](hipDeviceAttribute_t attribute) {
        int32_t value = 0;
        [[maybe_unused]] const auto result =
            hipDeviceGetAttribute(&value, attribute, device);
        return value;
    };

    // Let's query from the API what's the maximum amount of shared memory per
    // block.
    const int32_t max_shared_memory_per_block = get_device_attribute(
        hipDeviceAttribute_t::hipDeviceAttributeMaxSharedMemoryPerBlock);

    // Next, let's make sure the number of bytes the user gives is not more than
    // the maximum. If it is, we'll print a helpful message and exit the program
    // immediately.
    if (num_bytes_shared_mem > max_shared_memory_per_block) {
        std::fprintf(stderr,
                     "Shared memory request too large: %ld > %d, for kernel "
                     "\"%s\" in %s on line %d\n",
                     num_bytes_shared_mem, max_shared_memory_per_block,
                     kernel_name, file, line);
        exit(EXIT_FAILURE);
    }

    // Check the requested number of threads per block is within the
    // correct limits. It must be greater than zero and less than or equal to
    // the maximum.
    const int max_threads_x = get_device_attribute(
        hipDeviceAttribute_t::hipDeviceAttributeMaxBlockDimX);
    if (threads.x <= 0 || max_threads_x < threads.x) {
        // TODO
        // The given threads.x is not within the correct limits.
        // Print error message and exit.
        // See above how it's done for the shared memory check.

        std::fprintf(stderr, 
                    "Invalid number of threads.x per block: %d, must be in range [1, %d] for kernel %s in %s on line %d", threads.x, max_threads_x, kernel_name, file, line);
                    exit(EXIT_FAILURE);
    }
    // TODO: Do the same for y and z dimensions.
    const int max_threads_y = get_device_attribute(
        hipDeviceAttribute_t::hipDeviceAttributeMaxBlockDimY);
    if (threads.y <=0 || max_threads_y < threads.y)
    {
         std::fprintf(stderr, 
                    "Invalid number of threads.y per block: %d, must be in range [1, %d] for kernel %s in %s on line %d", threads.y, max_threads_y, kernel_name, file, line);
                    exit(EXIT_FAILURE);
    }

    const int max_threads_z = get_device_attribute(
        hipDeviceAttribute_t::hipDeviceAttributeMaxBlockDimX);

    if (threads.z <=0 || max_threads_z < threads.z)
    {
         std::fprintf(stderr, 
                    "Invalid number of threads.z per block: %d, must be in range [1, %d] for kernel %s in %s on line %d", threads.z, max_threads_z, kernel_name, file, line);
                    exit(EXIT_FAILURE);
    }
    // TODO: Do the same for all dimensions of grid size.
    // Hint: hipDeviceAttribute_t::hipDeviceAttributeMaxGridDimX
    // Compare againts the input argument 'dim3 blocks'
    // Similarly to blocks, also the grid sizes must be greater than zero in all
    // dimensions


    const int max_blocks_x = get_device_attribute(
        hipDeviceAttribute_t::hipDeviceAttributeMaxGridDimX
    );

    if (blocks.x <=0 || max_blocks_x < threads.x)
    {
         std::fprintf(stderr, 
                    "Invalid number of blocks.x per grid: %d, must be in range [1, %d] for kernel %s in %s on line %d", blocks.x, max_blocks_x, kernel_name, file, line);
                    exit(EXIT_FAILURE);
    }


    const int max_blocks_y = get_device_attribute(
        hipDeviceAttribute_t::hipDeviceAttributeMaxGridDimY
    );

    if (blocks.y <=0 || max_blocks_y < threads.y)
    {
         std::fprintf(stderr, 
                    "Invalid number of blocks.y per grid: %d, must be in range [1, %d] for kernel %s in %s on line %d", blocks.y, max_blocks_y, kernel_name, file, line);
                    exit(EXIT_FAILURE);
    }

    const int max_blocks_z = get_device_attribute(
        hipDeviceAttribute_t::hipDeviceAttributeMaxGridDimZ
    );

    if (blocks.z <=0 || max_blocks_z < threads.z)
    {
         std::fprintf(stderr, 
                    "Invalid number of blocks.x per grid: %d, must be in range [1, %d] for kernel %s in %s on line %d", blocks.z, max_blocks_z, kernel_name, file, line);
                    exit(EXIT_FAILURE);
    }

    // TODO: Finally make sure the total number of threads per block is less
    // than the maximum: i.e.
    // hipDeviceAttribute_t::hipDeviceAttributeMaxThreadsPerBlock >=
    // threads.x * threads.y * threads.z

    const int max_threads_per_block = get_device_attribute(
        hipDeviceAttribute_t::hipDeviceAttributeMaxThreadsPerBlock
    );

    int total_threads = threads.x*threads.y*threads.z;

    if (total_threads<=0 || total_threads > max_threads_per_block)
    {
        std::fprintf(stderr, 
            "Invalid number of threads : %d in kernel %s in %s on line %d. Total number of threads should be %d", total_threads, kernel_name, file, line, max_threads_per_block);
    }

    // Reset the error variable to success.
    result = hipGetLastError();

    kernel<<<blocks, threads, num_bytes_shared_mem, stream>>>(args...);

    result = hipGetLastError();
    if (result != hipSuccess) {
        printf("Error with kernel \"%s\" in %s at line %d\n%s: %s\n",
               kernel_name, file, line, hipGetErrorName(result),
               hipGetErrorString(result));
        exit(EXIT_FAILURE);
    }
}
