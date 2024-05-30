#ifndef CSE_WINDOW_H
#define CSE_WINDOW_H

#include <CSE/core.h>
#include <CSE/systems/platform.h>
#include <CSE/systems/layer.h>
#include <CSE/systems/layerstack.h>

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
		
		inline WindowPrefs GetPrefs() { return m_Prefs; }
		
		inline void SetScale(const WindowScale& scale) { m_Scale = scale; }
		inline WindowScale GetScale() { return m_Scale; }
		
		
		// Set a title but don't save in prefs.
		// Useful when a title updates frequently.
		void SetTitle(const std::string& title); 
		
		// Set a title and save in prefs.
		// Useful when a title is not to be updated frequently.
		void NewTitle(const std::string& title);
		
		// Get title from prefs (not the current one).
		inline std::string GetBaseTitle() { return m_Prefs.title; }
		
		// Get title from the titlebar.
		inline std::string GetTitle() { return std::string(SDL_GetWindowTitle(m_NativeWindow)); }
		
		void ShowFPSInTitle(uint64_t fpsCount);
		
		inline SDL_Event* GetEvents() { return m_Events; }
		inline SDL_Renderer* GetRenderer() { return m_Renderer; }
		inline SDL_Surface* GetSurface() { return m_WindowSurface; }
		inline SDL_Window* GetNativeWindow() { return m_NativeWindow; }
		inline uint32_t GetNativeWindowID() { return m_NativeWindowID; }
		
		// Layers operation
		inline LayerStack& GetLayers() { return m_LayerStack; }
		bool AttachLayer(Ref<Layer> layer);
		bool DetachLayer(Ref<Layer> layer);
	
		bool IsFocused() { return m_Focused; }
		void SetFocus(bool focus) { m_Focused = focus; }
		
	private:
		WindowPrefs m_Prefs;
		WindowScale m_Scale;
		uint32_t m_NativeWindowID; // SDL_GetWindowID()
		
		SDL_Event* m_Events = nullptr; // event queue
		SDL_Window* m_NativeWindow = nullptr; // the window
		SDL_Renderer* m_Renderer = nullptr; // the window renderer
		SDL_Surface* m_WindowSurface = nullptr; // the window surface
		
		LayerStack m_LayerStack;
		
		bool m_Focused = false;
		
		char m_NewTitle[512];
	};
}

#endif
