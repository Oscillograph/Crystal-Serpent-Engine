#ifndef CSE_LOGGER_H
#define CSE_LOGGER_H

#include <CSE/protocore.h>

// comment this line if you want something simple like std::cout
#define CSE_LOGGER_CLASS

namespace CSE
{
	typedef std::basic_stringstream<char32_t> u32stringstream;
	
	class Logger
	{
	public:
		static void Flush(int mode = 0, int color = -1);
		
		template<typename... Args>
		inline static void Text(Args... args)
		{
			m_u8stream.str(std::string()); // clear utf8 buffer
			m_u32string = U""; // clear utf32 string
			
			// add args to utf8 string stream
			m_u8stream << "[" << Utils::GetDateAndTime() << "] ";
			(u8add(std::forward<Args>(args)), ...);
			m_u8stream << "\n";
			m_u8string = m_u8stream.str();
			
			// convert it to utf32
			utf8::utf8to32(m_u8string.begin(), m_u8string.end(), std::back_inserter(m_u32string));
			
			m_u32stream << m_u32string << "\n";
		}
		
	private:
		static void Reset();
		
		template <typename T>
		inline static void u8add(T t) { m_u8stream << t; }
		
		template <typename T>
		inline static void u32add(T t) { m_u32stream << t; }
		
		static std::stringstream m_u8stream;
		static std::string m_u8string;
		static std::u32string m_u32string;
		static u32stringstream m_u32stream;
	};
}

#ifndef CSE_LOGGER_CLASS
	#define CSE_CORE_LOG(...)			std::cout << "CSE: " << __VA_ARGS__ << "\n";
	#define CSE_CORE_DEBUG(...)			std::cout << "CSE: " << __VA_ARGS__ << "\n";
	#define CSE_CORE_ASSERT(x, ...)		{ if (!(x)) { CSE_CORE_LOG(__VA_ARGS__); std::exit(); } }

	#define CSE_LOG(...) 				std::cout << "App: " << __VA_ARGS__ << "\n";
	#define CSE_DEBUG(...)				std::cout << "App: " << __VA_ARGS__ << "\n";
	#define CSE_ASSERT(x, ...)			{ if (!(x)) { CSE_LOG(__VA_ARGS__); std::exit(); } }
#else
	#define CSE_CORE_LOG(...)			::CSE::Logger::Text("CSE: ", __VA_ARGS__); ::CSE::Logger::Flush(0, 0);
	#define CSE_CORE_DEBUG(...)			::CSE::Logger::Text("CSE: ", __VA_ARGS__); ::CSE::Logger::Flush(1, 0);
	#define CSE_CORE_ASSERT(x, ...)		{ if (!(x)) { CSE_CORE_LOG(__VA_ARGS__); std::exit(-4); } }

	#define CSE_LOG(...)				::CSE::Logger::Text("App: ", __VA_ARGS__); ::CSE::Logger::Flush(0, 1);
	#define CSE_DEBUG(...)				::CSE::Logger::Text("App: ", __VA_ARGS__); ::CSE::Logger::Flush(1, 1);
	#define CSE_ASSERT(x, ...)			{ if (!(x)) { CSE_LOG(__VA_ARGS__); std::exit(-4); } }
#endif

#endif

