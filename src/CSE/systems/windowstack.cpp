#include <CSE/systems/windowstack.h>

namespace CSE
{
	WindowStack::WindowStack()
	{
	}
	
	WindowStack::~WindowStack()
	{
	}
	
	void WindowStack::Push(Window* window)
	{
		m_Windows.emplace(m_Windows.begin() + m_WindowInsertIndex, window);
		m_WindowInsertIndex++;
	}
	
	void WindowStack::Pop(Window* window)
	{
		if (window != nullptr)
		{
			auto it = std::find(m_Windows.begin(), m_Windows.end(), window);
			if (it != m_Windows.end())
			{
				m_Windows.erase(it);
				delete window;
				m_WindowInsertIndex--;
			}
		}
	}
}
