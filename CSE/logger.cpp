#include <CSE/logger.h>

#include <CSE/vendor/color-console/color.hpp> // colorizing console output

namespace CSE
{
	std::string Logger::m_u8string = "";
	std::u32string Logger::m_u32string = U"";
	std::stringstream Logger::m_u8stream;
	u32stringstream Logger::m_u32stream;
	
	void Logger::Reset(){
		m_u8string = "";
		m_u32string = U"";
		m_u8stream.str(std::string());
		m_u32stream.str(std::u32string());
	}
	
	void Logger::Flush(int mode, int color){
		// flush into command line interface
		if (mode == 0) 
		{
			switch (color)
			{
				case 0: // engine log
				{
					std::cout << dye::light_yellow(m_u8string);
					break;
				} 
				
				case 1: // app log
				{
					std::cout << dye::white(m_u8string);
					break;
				}
				
				case 2: // dark red
				{
					std::cout << dye::red(m_u8string);
					break;
				}
				
				case 3: // light red
				{
					std::cout << dye::light_red(m_u8string);
					break;
				}
				
				default: // no idea who's log
				{
					std::cout << dye::grey(m_u8string);
				}
			}
		}
		
		// flush onto screen
		if (mode == 1) {}
		
		Reset();
	}
}
