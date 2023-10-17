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
	}
}

#endif
