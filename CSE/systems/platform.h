#ifndef CSE_PLATFORM_H
#define CSE_PLATFORM_H

#include <CSE/core.h>

// Simple Direct media Library
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

namespace CSE
{
	typedef uint64_t TimeType;
	
	// SDL_Renderer* Renderer::Create(int rendererFlags){}
	class Platform
	{
	public:
		static int InitDefault();
		static int Init(uint32_t platformFlags, uint32_t rendererFlags, uint32_t winFlags, uint32_t imgFlags);
		static int Shutdown();
		
		// Windowing
		static SDL_Window* CreateWindow(const char* title, int x, int y, int width, int height, uint32_t winFlags);
		
		// Events
		// inline static SDL_Event* GetEventListener() { return m_EventListener; }
		inline static SDL_Event* GetEventListener() { return new SDL_Event; }
		
		// Renderer
		static SDL_Renderer* InitRenderer(SDL_Window* window);
		static SDL_Surface* GetWindowSurface(SDL_Window* window);
		
		// Utils
		inline static uint64_t GetTimeMs() { return SDL_GetTicks64(); }
		
	private:
		static uint32_t m_PlatformFlags;
		static uint32_t m_RendererFlags;
		static uint32_t m_WindowFlags;
		static uint32_t m_ImgFlags;
		static SDL_Event* m_EventListener;
	};
}

#endif
