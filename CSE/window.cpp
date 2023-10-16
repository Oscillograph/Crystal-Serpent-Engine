#include <CSE/window.h>

namespace CSE
{
	Window::Window(const WindowPrefs& prefs)
	{
		Init(prefs);
	}
	
	Window::~Window()
	{
		CSE_LOG("Destroy the window");
		Close();
	}
	
	void Window::Init(const WindowPrefs& prefs){ // constructor
		m_Prefs = prefs;
		m_Scale = {1.0f, 1.0f, 1.0f};
		
		int winFlags = SDL_WINDOW_RESIZABLE + SDL_WINDOW_SHOWN + SDL_WINDOW_OPENGL;
		int renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
		int imgFlags = IMG_INIT_PNG + IMG_INIT_JPG;
		
		// begin SDL initialization
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
		{
			CSE_CORE_LOG("Error initializing SDL");
		} else {
			CSE_CORE_LOG("SDL initialized");
			// Create the main window
			m_Window = SDL_CreateWindow( 
				m_Prefs.title.c_str(), 
				m_Prefs.x, 
				m_Prefs.y, 
				(int)round(m_Prefs.width * m_Scale.x), 
				(int)round(m_Prefs.height * m_Scale.y), 
				winFlags 
				);
			
			// Make sure creating the window succeeded
			if (m_Window == NULL) 
			{
				CSE_CORE_LOG("Error creating window: ", SDL_GetError());
			} else {
				CSE_CORE_LOG("Created a window");
				//Create renderer for window 
				m_Renderer = SDL_CreateRenderer(m_Window, -1, renderFlags); 
				
				if (m_Renderer == NULL)
				{ 
					CSE_CORE_LOG("Renderer could not be created! SDL Error: ", SDL_GetError());
				} else {
					CSE_CORE_LOG("Renderer online.");
					// Initialize PNG loading
					if (!(IMG_Init(imgFlags) & imgFlags))
					{
						CSE_CORE_LOG("SDL_image could not initialize! SDL_image Error: ", IMG_GetError());
					} else {
						CSE_CORE_LOG("SDL_image online.");
						//Initialize SDL_mixer 
						if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) 
						{ 
							CSE_CORE_LOG( "SDL_mixer could not initialize! SDL_mixer Error: ", Mix_GetError() ); 
						}
						CSE_CORE_LOG("SDL_mixer online.");
						// Get the surface from the window
						m_WindowSurface = SDL_GetWindowSurface(m_Window);
						
						// Make sure getting the surface succeeded
						if (m_WindowSurface == NULL) 
						{
							CSE_CORE_LOG("Error getting surface: ", SDL_GetError());
						} else {
							// Surprising, but it's not enough to declare e.
							// Without this line the program will likely crash with c0000005.
							m_Events = new SDL_Event;
							CSE_CORE_LOG("SDL events listener online.");
							
							// pass pointers to important objects to CSE subsystems
							
							CSE_LOG("Internal systems pointers configured.");
							
							// initialize randomizer
							srand(SDL_GetTicks64());
						}
					}
				}
			}
		}
	}
	
	void Window::Close()
	{
		// destroy surfaces
		if (m_WindowSurface != nullptr){
			SDL_FreeSurface(m_WindowSurface);
			m_WindowSurface = nullptr;
		}
		CSE_CORE_LOG("Window surface memory freed.");
		
		// destroy renderer and the window
		if (m_Renderer != nullptr){
			SDL_DestroyRenderer(m_Renderer);
			m_Renderer = nullptr;
		}
		CSE_CORE_LOG("Renderer destroyed.");
		
		if (m_Window != nullptr){
			SDL_DestroyWindow(m_Window);
			m_Window = nullptr;
		}
		CSE_CORE_LOG("Window destroyed.");
		
		delete m_Events;
		m_Events = nullptr;
		CSE_CORE_LOG("Internal systems pointers deleted.");
		
		// quit systems
		IMG_Quit();
		CSE_CORE_LOG("SDL_image offline.");
		Mix_Quit();
		CSE_CORE_LOG("SDL_mixer offline.");
		SDL_Quit();
		CSE_CORE_LOG("SDL offline.");
	}
}
