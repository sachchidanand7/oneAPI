



#include <oneapi/dpl/algorithm>
#include <oneapi/dpl/execution>
#include <oneapi/dpl/iterator>
#include <oneapi/dpl/numeric>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <CL/sycl.hpp>
#include <fstream>
#include <experimental/filesystem>

// boost
#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/scope_exit.hpp>

#include<MarketData.h>
#include<MarketDataBuilder.h>
#include<dpc_common.hpp>
#include<GPUAPPService.h>

using namespace app;
using namespace common;

int main(int argc, char *argv[]) {
//if (argc != 1) {
//Usage(argv[0]);
// }


	                    try {

                        // Create a queue using default device
                        // Set the SYCL_DEVICE_FILTER, we are using PI_OPENCL environment variable

                        // Default queue, set accelerator choice above.
                        //queue q (default_selector{},dpc_common::exception_handler);
                        queue q = queue(default_selector{}, exception_handler);
                        //queue q (accelerator_selector{},dpc_common::exception_handler);
                        // Display the device info
                        //ShowDevice(m_queue);

                        usm_allocator<MarketData, usm::alloc::shared> allocator(q);

                        std::cout << "Device: " << q.get_device().get_info<info::device::name>() << std::endl;

                       //Unified Shared Memory q
                       //Allocation enables data access on host and device
                       std::vector<MarketData, usm_allocator<MarketData, usm::alloc::shared>> gpuMarketDataList(allocator);
                       //m_gpuMarketDataList.reserve(m_numOfMessages);

                       //marketDataBuilderPtr->setDataHolderForGPU(m_gpuMarketDataList);
                     GPUAPPService gpuAppService(1000000, q, gpuMarketDataList);
		     gpuAppService.init();
                     gpuAppService.start();



                    } catch (...) {
                        // some other exception detected
                        cout << "Initialization failed \n";
                    }

        //GPUAPPService gpuAppService(100000);
	//gpuAppService.start();
  return 0;

}
