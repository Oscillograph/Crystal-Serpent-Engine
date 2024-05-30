#ifndef CSE_WINDOWSTACK_H
#define CSE_WINDOWSTACK_H

#include <CSE/core.h>

namespace CSE 
{
	class Window;
	
	class WindowStack
	{
	public:
		WindowStack();
		~WindowStack();
		
		void Push(Window* window);
		void Pop(Window* window);
		
		inline const std::vector<Window*>& GetContents() const { return m_Windows; }
		inline size_t Size() { return m_Windows.size(); }
		
		std::vector<Window*>::iterator begin() { return m_Windows.begin(); }
		std::vector<Window*>::iterator end()   { return m_Windows.end();   }
		
	protected:
		std::vector<Window*> m_Windows;
		uint32_t m_WindowInsertIndex = 0;
	};
}

#endif
