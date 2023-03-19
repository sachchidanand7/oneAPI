#ifndef _MARKETDATA_BUILDER_H
#define _MARKETDATA_BUILDER_H

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

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

#include<MarketData.h>

using namespace cl::sycl;

namespace common {
class MarketDataBuilder {

	public:
		MarketDataBuilder(int64_t numOfMessages, std::vector<MarketData, usm_allocator<MarketData, usm::alloc::shared>>& gpuMarketDataList):m_numOfMessages(numOfMessages), m_gpuMarketDataList(gpuMarketDataList) {}
		~MarketDataBuilder() {} 
		void build() {

		    std::cout<<"Building messages for "<<m_numOfMessages<<std::endl;
                    for (int i = 0; i < m_numOfMessages - 1; i++) {
			  MarketData data;
			  data.productId = i + 1;
			  m_gpuMarketDataList.push_back(data);
			  marketDataList.push_back(data);
		    }

		}
		vector<MarketData>& getMarketDataList() {
			return marketDataList;
		}
	private:
		std::vector<MarketData> marketDataList;
		int64_t m_numOfMessages;
		std::vector<MarketData, usm_allocator<MarketData, usm::alloc::shared>>& m_gpuMarketDataList;

};
}


#endif
