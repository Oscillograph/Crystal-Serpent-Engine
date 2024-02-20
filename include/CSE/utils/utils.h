#ifndef CSE_UTILS_H
#define CSE_UTILS_H

#include <CSE/protocore.h>

namespace CSE
{
	namespace Utils
	{
		static std::string GetDateAndTime() 
		{
			time_t 		timeNow = time(0);
			tm  		timeStruct;
			char 		charBuffer[80];
			
			// get current time
			timeStruct = *localtime(&timeNow);
			// convert it to a string
			strftime(charBuffer, sizeof(charBuffer), "%Y.%m.%d - %X", &timeStruct);
			
			return charBuffer;
		}
		
		namespace UUID {
			static std::random_device g_RandomDevice;
			static std::mt19937_64 g_RandomEngine(g_RandomDevice());
			static std::uniform_int_distribution<uint64_t> g_UniformDistribution;
			
			static uint64_t Generate() {
				return g_UniformDistribution(g_RandomEngine);
			}
		}
	}
}

#endif
