#ifndef CSE_APPLICATION_H
#define CSE_APPLICATION_H

#include <CSE/core.h>
#include <CSE/window.h>

namespace CSE
{
	// Main CSE application which can have windows
	class Application
	{
	public:
		Application();
		Application(const WindowPrefs& prefs = {CSE::EngineInfo::Name, 0, 0, 600, 480, 1});
		~Application();
		int Run();
		
	private:
		bool m_Quit = false;
		Window* m_Window = nullptr;
	};
		
}

#endif
