#ifndef CSE_WINDOW_H
#define CSE_WINDOW_H

#include <CSE/core.h>

// Simple Direct media Library
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

namespace CSE 
{
	struct ScaleWindow {
		float x;
		float y;
		float z;
	};
	
	struct WindowPrefs {
		std::string title;
		int x;
		int y;
		int width;
		int height;
		int layers;
	};
	
	class Window
	{
	public:
		Window(const WindowPrefs& prefs = {CSE::EngineInfo::Name, 0, 0, 600, 480, 1});
		~Window();
		
		void Init(const WindowPrefs& prefs);
		void Close();
		
		inline SDL_Event* GetEvents() { return m_Events; }
		inline SDL_Renderer* GetRenderer() { return m_Renderer; }
		inline SDL_Window* GetWindow() { return m_Window; }
	
	private:
		WindowPrefs m_Prefs;
		ScaleWindow m_Scale;
		
		SDL_Event* m_Events; // event queue
		SDL_Window* m_Window; // the window
		SDL_Renderer* m_Renderer; // the window renderer
		SDL_Surface* m_WindowSurface; // the window surface
	};
}

#endif
