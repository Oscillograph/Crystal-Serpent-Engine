#ifndef CSE_WINDOW_H
#define CSE_WINDOW_H

#include <CSE/core.h>
#include <CSE/platform.h>

namespace CSE 
{
	struct WindowScale {
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
		Window();
		Window(const WindowPrefs& prefs);
		Window(const WindowPrefs& prefs, const WindowScale& scale, uint32_t winFlags);
		~Window();
		
		void Open(const WindowPrefs& prefs, const WindowScale& scale, uint32_t winFlags);
		void Close();
		
		void SetTitle(const std::string& title);
		inline std::string GetTitle() { return m_Prefs.title; }
		
		inline SDL_Event* GetEvents() { return m_Events; }
		inline SDL_Renderer* GetRenderer() { return m_Renderer; }
		inline SDL_Surface* GetSurface() { return m_WindowSurface; }
		inline SDL_Window* GetNativeWindow() { return m_NativeWindow; }
	
	private:
		WindowPrefs m_Prefs;
		WindowScale m_Scale;
		
		SDL_Event* m_Events = nullptr; // event queue
		SDL_Window* m_NativeWindow = nullptr; // the window
		SDL_Renderer* m_Renderer = nullptr; // the window renderer
		SDL_Surface* m_WindowSurface = nullptr; // the window surface
	};
}

#endif
