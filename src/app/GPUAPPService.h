#ifndef _GPU_APP_SERVICE_H
#define _GPU_APP_SERVICE_H


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


using namespace common;
using namespace std;
using namespace cl::sycl;
using namespace sycl;


namespace app {

class GPUAPPService {

	public:
		GPUAPPService(int64_t numOfMessages, queue& queue,
		std::vector<MarketData, usm_allocator<MarketData, usm::alloc::shared>>& gpuMarketDataList):m_numOfMessages(numOfMessages),m_queue(queue),m_gpuMarketDataList(gpuMarketDataList) {
		   marketDataBuilderPtr = std::make_shared<MarketDataBuilder>(numOfMessages, m_gpuMarketDataList);
		}

		~GPUAPPService(){}

		/**
		 *
		 *Create queue and set accelerator.
		 */
		void init() {
		    

	            try {

                        // Create a queue using default device
                        // Set the SYCL_DEVICE_FILTER, we are using PI_OPENCL environment variable

                        // Default queue, set accelerator choice above.
                        //queue q (default_selector{},dpc_common::exception_handler);
                      //  m_queue = queue(default_selector{}, exception_handler);
                        //queue q (accelerator_selector{},dpc_common::exception_handler);
                        // Display the device info
                        ShowDevice(m_queue);

			//usm_allocator<MarketData, usm::alloc::shared> allocator(m_queue);

                        std::cout << "Device: " << m_queue.get_device().get_info<info::device::name>() << std::endl;

                       //Unified Shared Memory q
		       //Allocation enables data access on host and device
		       //m_gpuMarketDataList = std::vector<MarketData, usm_allocator<MarketData, usm::alloc::shared>>(allocator);
                       m_gpuMarketDataList.reserve(m_numOfMessages);

		       //marketDataBuilderPtr->setDataHolderForGPU(m_gpuMarketDataList);

                        marketDataBuilderPtr->build();
   
                    } catch (...) {
                        // some other exception detected
                        cout << "Initialization failed \n";
                    }
                }

	
		void start() {
                    executeOnGPU();
		    defaultProcessing();
		}
		void stop() {}
		void oncallbackEvent() {
		     std::vector<MarketData> markDataList = marketDataBuilderPtr->getMarketDataList();
                        
		}

	        void defaultProcessing() {
                    std::vector<MarketData>& marketDataList = marketDataBuilderPtr->getMarketDataList();
		    const int32_t& size = marketDataList.size();
		    int32_t index = 0;
		    std::cout<<"Processing for total entry :"<<size<<std::endl;
		    MyTimer t_ser;
		    while(index < size) {
			    marketDataList[index].productId *= 10;
			    ++index;
		    }

		    Duration serial_time = t_ser.elapsed();
                    std::cout << std::setw(20) << "CPU execution time: " << serial_time.count() << "s\n";
                    std::cout<<"End of default process operation"<<std::endl;

		}

		void executeOnGPU() {

		    std::cout<<"Started excuting on GPU"<<std::endl;
		    //const int32_t& size = sizeof(MarketData); 


      
	            //m_queue.submit([&] (handler &h){

                    //sycl::stream out(1024, 256, h);
		    const int& totalElements = m_gpuMarketDataList.size();
		    std::cout<<"Processing for total entry on GPU :"<<totalElements<<std::endl;
                    MyTimer t_ser;
		    m_queue.parallel_for(range<1>(totalElements), [=,ptr = &m_gpuMarketDataList[0]] (id<1> i){
                         
				    //int size = i *  184;
				    //std::cout<<"GPU"<<ptr->productId<<std::endl;
				    //out << "size"<<size<<"i"<<i<<sycl::endl;
				    (ptr + i)->productId *= 10;
				    //out << "ptr "<<(ptr + i)->productId<< sycl::endl;

				    //++ptr; 

                      }).wait();
		     //m_queue.wait();
		     //});

		    Duration serial_time = t_ser.elapsed();
		    std::cout << std::setw(20) << "GPU execution time: " << serial_time.count() << "s\n";
		    std::cout<<"End of GPU operation"<<std::endl;
		    //for(int i=0; i<20; i++) {
		//	std::cout<<m_gpuMarketDataList[i].productId<<std::endl;
		  //  }


		}



	private:
		std::shared_ptr<MarketDataBuilder> marketDataBuilderPtr;
		int64_t m_numOfMessages{0};
		queue& m_queue;
                std::vector<MarketData, usm_allocator<MarketData, usm::alloc::shared>>& m_gpuMarketDataList;



		void ShowDevice(queue &q) {
                    // Output platform and device information.
                    auto device = q.get_device();
                    auto p_name = device.get_platform().get_info<info::platform::name>();
                    cout << std::setw(20) << "Platform Name: " << p_name << "\n";
                    auto p_version = device.get_platform().get_info<info::platform::version>();
                    cout << std::setw(20) << "Platform Version: " << p_version << "\n";
                    auto d_name = device.get_info<info::device::name>();
                    cout << std::setw(20) << "Device Name: " << d_name << "\n";
                    auto max_work_group = device.get_info<info::device::max_work_group_size>();
                    cout << std::setw(20) << "Max Work Group: " << max_work_group << "\n";
                    auto max_compute_units = device.get_info<info::device::max_compute_units>();
                    cout << std::setw(20) << "Max Compute Units: " << max_compute_units << "\n";
                }



};

}

#endif
