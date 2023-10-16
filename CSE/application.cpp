#include <CSE/application.h>

namespace CSE
{
	Application::Application()
	{
		Init();
		m_Window = new Window();
	}
	
	Application::Application(const WindowPrefs& prefs)
	{
		Init();
		m_Window = new Window(prefs);
	}
	
	Application::~Application()
	{
		if (m_Window != nullptr)
		{
			delete m_Window;
			m_Window = nullptr;
		}
		
		if (m_Events != nullptr)
		{
			delete m_Events;
			m_Events = nullptr;
		}
		CSE_CORE_LOG("Internal systems pointers deleted.");
		
		Platform::Shutdown();
	}
	
	int Application::Init()
	{
		Platform::InitDefault();
		m_Events = Platform::GetEventListener();
		
		// initialize randomizer
		srand(SDL_GetTicks64());
	}
	
	int Application::Run()
	{
		CSE_CORE_LOG("Entering the main loop");
		while (!m_Quit){
			// 0. Start collecting debug information
			// 1. Input management system
			while (SDL_PollEvent(m_Window->GetEvents()))
			{
				if (m_Window->GetEvents()->type == SDL_QUIT){
					m_Quit = true;
					break;
				}
			}
			// 2. Events system
			// 3. App Reaction system
			// 4. Game Object Management System (GOMS)
			// 5. World progress system
			// 		- physics and other world rules subsystems management
			//    control time flow and then update world
			// 6. Asset management system
			// 7. Sound system
			// 8. Show debug information as text over the screen
			// 9. Graphic system
			SDL_SetRenderDrawColor(m_Window->GetRenderer(), 30, 50, 90, 255);
			SDL_RenderClear(m_Window->GetRenderer());
			SDL_RenderPresent(m_Window->GetRenderer());
			// 10. File I/O system
			// 11. Log system
			// 12. FPS Count
			
			// CSE_LOG("This is a main loop");
			if (m_Quit)
				return 0;
		}
	}
}
