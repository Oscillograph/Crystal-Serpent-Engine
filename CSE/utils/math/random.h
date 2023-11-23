#ifndef MYRANDOMENGINE_H
#define MYRANDOMENGINE_H

#include <random>
#include <time.h>

namespace CSE
{
	namespace Math
	{
		// An equidistributed random number generator set in interval between m_min and m_max.
		// Based on rand() function.
		// Useful in many role-playing or other turn-based board games.
		template <typename T>
		class Random
		{
		public:
			Random()
			: m_min(static_cast<T>(0)), m_max((T)(1))
			{
				srand((uint32_t)time(0));
				m_scale = (m_max - m_min) / (float)RAND_MAX;
			}
			
			Random(T min, T max)
			: m_min(min), m_max(max)
			{
				srand((uint32_t)time(0));
				m_scale = (m_max - m_min) / (float)RAND_MAX;
			}
			
			~Random() 
			{
			}
			
			T& NewNumber()
			{
				m_number = (T)(floorf(m_min + rand() * m_scale));
				
				return m_number;
			}
			
			T LastNumber()
			{
				return m_number;
			}
			
		private:
			float m_scale = 1.0f;
			T m_number = 0;
			T m_min;
			T m_max;
		};
		
		typedef Random<int>		RandomI;
		typedef Random<float>	RandomF;
	}
}

#endif
