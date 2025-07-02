#include <iostream>
#include <sycl/sycl.hpp>
using namespace sycl;

int main() {
  // Set up queue on any available device
  //TODO 
  
  std::cout<<"\tChecking for GPUs\n"<<std::endl;

  auto gpu_devices = sycl::device::get_devices(sycl::info::device_type::gpu);
  auto n_gpus = size(gpu_devices);

  std::cout<< "\tThere are "<<n_gpus<<" GPUs\n"<<std::endl;

  if(n_gpus > 0)
  {
    queue q{gpu_selector_v};
  }

  else
  {
    std::cout<<"\tThere are no GPUs found. \n Exiting"<<std::endl;
    exit(1);
  }

  // Initialize input and output memory on the host
  constexpr size_t N = 25600;
  std::vector<int> x(N),y(N);
  int a=4;
  std::fill(x.begin(), x.end(), 1);
  std::fill(y.begin(), y.end(), 2);

  {
   // Create buffers for the host data or allocate memory usinggUSM
   // If USM + malloc_device() is used add the copy operations
   // TODO

   sycl::buffer<int, 1> x_buf(x.data(), sycl::range<1>(N));
   sycl::buffer<int, 1> y_buf(y.data(), sycl::range<1>(N));

    // Submit the kernel to the queue
    q.submit([&](handler& h) {
      // Create accessors if necessary
      //TODO

      h.parallel_for(
        //The kernel as a lambda
        //TODO
      );
    });

      //TODO after the submission works
      //Checking the result inside the scope of the buffers using host_accessors
  }
  // If USM + malloc_device() is used add the copy operations 
  // TODO
  // Check that all outputs match expected value

  // If USM is used free the device memory
  // TODO
  // Check that all outputs match expected value
  bool passed = std::all_of(y.begin(), y.end(),
                            [a](int val) { return val == a * 1 + 2; });
  std::cout << ((passed) ? "SUCCESS" : "FAILURE")
            << std::endl;
  return (passed) ? 0 : 1;
}
