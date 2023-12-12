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
				m_number = (T)(roundf(m_min + rand() * m_scale));
				
				return m_number;
			}
			
			T LastNumber()
			{
				return m_number;
			}
			
			T GetMinimum()
			{
				return m_min;
			}
			
			T GetMaximum()
			{
				return m_max;
			}
		private:
			float m_scale = 1.0f;
			T m_number = 0;
			T m_min;
			T m_max;
		};
		
		typedef Random<int>		RandomI;
		typedef Random<float>	RandomF;
		
		class Dice
		{
		public:
			Dice()
			{
				Init();
			};
			
			Dice(int count)
			{
				Init(count);
			};
			
			~Dice()
			{
				delete m_RandomEngine;
			};
			
			int Roll()
			{
				if (m_RandomEngine != nullptr)
					return (m_Dice[m_RandomEngine->NewNumber()]);
				return -1;
			};
			
			int Get()
			{
				if (m_RandomEngine != nullptr)
					return m_Dice[m_RandomEngine->LastNumber()];
				return -1;
			};
			
		private:
			std::vector<int> m_Dice = {};
			RandomI* m_RandomEngine = nullptr;
			
			void Init(int count = 0)
			{
				switch(count)
				{
				case 2:
					m_Dice = {1, 2};
					break;
				case 3:
					m_Dice = {1, 2, 3};
					break;
				case 4:
					m_Dice = {1, 2, 3, 4};
					break;
				case 6:
					m_Dice = {1, 2, 3, 4, 5, 6}; 
					break;
				case 7:
					m_Dice = {1, 2, 3, 4, 5, 6, 7};
					break;
				case 8:
					m_Dice = {1, 2, 3, 4, 5, 6, 7, 8};
					break;
				case 10:
					m_Dice = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
					break;
				case 12:
					m_Dice = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
					break;
				case 16:
					m_Dice = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
					break;
				case 20:
					m_Dice = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
					break;
				case 100:
					for (int i = 1; i <= count; i++)
						m_Dice.push_back(i);
					break;
				default:
					m_Dice = {1, 2, 3, 4, 5, 6};
				}
				m_RandomEngine = new RandomI(0, (m_Dice.size() - 1));
				printf("Randomizer min and max: %d, %d\n", m_RandomEngine->GetMinimum(), m_RandomEngine->GetMaximum());
				printf("Dice at 0 and last: %d, %d\n", m_Dice[0], m_Dice[m_Dice.size()-1]);
				Roll(); // without this initial roll any first roll would be the same
			};
		};
	}
}

#endif
