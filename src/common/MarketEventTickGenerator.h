#ifndef _M ARKET_EVENT_TICK_GENERATOR_H
#define _MARKET_EVENT_TICK_GENERATOR_H

namespace common {

	std::function<void(int)> eventCallback;

class MarketEventTickGenerator {
	public:
		MarketEventTickGenerator(int numOfEvents):m_numOfEvents(numOfEvents) {} 
		~MarketEventTickGenerator() {}

		template <typename T>
		void registerCallback(T callback) {
			eventCallback = callback;
		}

		void start() {
                    std::thread tickThread(run);
		    std::detach(tickThread);
		}

		void run() {  
                    int index = 0;
		    while (index < m_numOfEvents || m_numOfEvents == -1) {
		        eventCallback(0);
		        ++index;
	            }
		}

	private:
		std::int64_t m_numOfEvents;
		
};	

};

#endif    
