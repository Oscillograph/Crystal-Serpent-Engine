#include <CSE/platform.h>

namespace CSE
{
	// default settings
	uint32_t Platform::m_PlatformFlags = SDL_INIT_EVERYTHING;
	uint32_t Platform::m_RendererFlags = SDL_WINDOW_RESIZABLE + SDL_WINDOW_SHOWN + SDL_WINDOW_OPENGL;
	// uint32_t Platform::m_RendererFlags = SDL_WINDOW_RESIZABLE + SDL_WINDOW_SHOWN;
	uint32_t Platform::m_WindowFlags = SDL_RENDERER_ACCELERATED + SDL_RENDERER_PRESENTVSYNC + SDL_RENDERER_TARGETTEXTURE;
	uint32_t Platform::m_ImgFlags = IMG_INIT_PNG + IMG_INIT_JPG;
	
	SDL_Event* Platform::m_EventListener = nullptr;
	
	int Platform::InitDefault()
	{
		return Init(m_PlatformFlags, m_RendererFlags, m_WindowFlags, m_ImgFlags);
	}
	
	int Platform::Init(uint32_t platformFlags, uint32_t rendererFlags, uint32_t winFlags, uint32_t imgFlags)
	{
		m_PlatformFlags = platformFlags;
		m_RendererFlags = rendererFlags;
		m_WindowFlags = winFlags;
		m_ImgFlags = imgFlags;
		
		// CSE_CORE_LOG("Renderer flags on Init: ", m_RendererFlags);
		
		/*
		* - `SDL_INIT_TIMER`: timer subsystem
		* - `SDL_INIT_AUDIO`: audio subsystem
		* - `SDL_INIT_VIDEO`: video subsystem; automatically initializes the events
		*   subsystem
		* - `SDL_INIT_JOYSTICK`: joystick subsystem; automatically initializes the
		*   events subsystem
		* - `SDL_INIT_HAPTIC`: haptic (force feedback) subsystem
		* - `SDL_INIT_GAMECONTROLLER`: controller subsystem; automatically
		*   initializes the joystick subsystem
		* - `SDL_INIT_EVENTS`: events subsystem
		* - `SDL_INIT_EVERYTHING`: all of the above subsystems
		* - `SDL_INIT_NOPARACHUTE`: compatibility; this flag is ignored
		*/
		
		// Initialize SDL Core
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			CSE_CORE_LOG("Error initializing SDL");
			std::exit(-1);
		}
		CSE_CORE_LOG("SDL initialized.");
		
		// Initialize SDL Image
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			CSE_CORE_LOG("SDL_image could not initialize! SDL_image Error: ", IMG_GetError());
			SDL_Quit();
			std::exit(-2);
		}
		CSE_CORE_LOG("SDL_image online.");
		
		// Initialize SDL_mixer 
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) 
		{ 
			CSE_CORE_LOG( "SDL_mixer could not initialize! SDL_mixer Error: ", Mix_GetError() );
			IMG_Quit();
			SDL_Quit();
			std::exit(-3);
		}
		CSE_CORE_LOG("SDL_mixer online.");
		
		m_EventListener = new SDL_Event;
		CSE_CORE_LOG("SDL events listener online.");
		
		// CSE_CORE_LOG("Number of available video drivers: ", SDL_GetNumRenderDrivers());
		// SDL_GetRendererInfo();
		
		return 0;
	}
	
	SDL_Window* Platform::CreateWindow(const char* title, int x, int y, int width, int height, uint32_t winFlags)
	{
		SDL_Window* window = SDL_CreateWindow(title, x, y, width, height, winFlags);
		CSE_CORE_ASSERT(window != NULL, "Error creating window: ", SDL_GetError());
		
		CSE_CORE_LOG("Created a window");
		return window;
	}
	
	SDL_Surface* Platform::GetWindowSurface(SDL_Window* window)
	{
		SDL_Surface* surface = SDL_GetWindowSurface(window);
		CSE_CORE_ASSERT(surface != NULL, "Error getting surface: ", SDL_GetError());
		
		CSE_CORE_LOG("Window surface access check passed.");
		
		return surface;
	}
	
	SDL_Renderer* Platform::InitRenderer(SDL_Window* window)
	{
		int index = SDL_GetNumRenderDrivers();
		CSE_CORE_LOG("Total render drivers availiable: ", index);
		for (int i = 0; i < index; i++)
		{
			SDL_RendererInfo* info;
			if (SDL_GetRenderDriverInfo(i, info) >= 0)
			{
				CSE_CORE_LOG("Render driver info: #", i);
				CSE_CORE_LOG("Name: ", info->name);
				CSE_CORE_LOG(" ");
			}
		}
		// CSE_CORE_LOG("Renderer flags on InitRenderer: ", m_RendererFlags);
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, m_RendererFlags);
		// CSE_CORE_LOG("Adress of a window: ", window);
		if (renderer == NULL)
		{
			CSE_CORE_LOG("Renderer could not be created! SDL Error: ", SDL_GetError());
			std::exit(-5);
		}
		return renderer;
	}
	
	int Platform::Shutdown()
	{
		delete m_EventListener;
		m_EventListener = nullptr;
		
		// quit systems
		Mix_Quit();
		CSE_CORE_LOG("SDL_mixer offline.");
		IMG_Quit();
		CSE_CORE_LOG("SDL_image offline.");
		SDL_Quit();
		CSE_CORE_LOG("SDL offline.");
	}
}
