#ifndef CSE_PLATFORM_H
#define CSE_PLATFORM_H

#include <CSE/core.h>

// Simple Direct media Library
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

namespace CSE
{
	// SDL_Renderer* Renderer::Create(int rendererFlags){}
	class Platform
	{
	public:
		static int InitDefault();
		static int Init(uint32_t platformFlags, uint32_t rendererFlags, uint32_t winFlags, uint32_t imgFlags);
		static SDL_Renderer* InitRenderer(SDL_Window* window);
		static SDL_Window* CreateWindow(const char* title, int x, int y, int width, int height, uint32_t winFlags);
		static SDL_Surface* GetWindowSurface(SDL_Window* window);
		inline static SDL_Event* GetEventListener() { return m_EventListener; }
		static int Shutdown();
		
	private:
		static uint32_t m_PlatformFlags;
		static uint32_t m_RendererFlags;
		static uint32_t m_WindowFlags;
		static uint32_t m_ImgFlags;
		static SDL_Event* m_EventListener;
	};
}

#endif
