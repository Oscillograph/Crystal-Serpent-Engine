#include <CSE/window.h>

namespace CSE
{
	Window::Window()
	{
		Open(
			{CSE::EngineInfo::Name, 0, 0, 600, 480, 1},
			{1.0f, 1.0f, 1.0f},
			(SDL_WINDOW_RESIZABLE + SDL_WINDOW_SHOWN + SDL_WINDOW_OPENGL)
			);
	}
	
	Window::Window(const WindowPrefs& prefs)
	{
		Open(
			prefs,
			{1.0f, 1.0f, 1.0f},
			(SDL_WINDOW_RESIZABLE + SDL_WINDOW_SHOWN + SDL_WINDOW_OPENGL)
			);
	}
	
	Window::Window(const WindowPrefs& prefs, const WindowScale& scale, uint32_t winFlags)
	{
		Open(prefs, scale, winFlags);
	}
	
	Window::~Window()
	{
		Close();
	}
	
	void Window::Open(const WindowPrefs& prefs, const WindowScale& scale, uint32_t winFlags){ // constructor
		m_Prefs = prefs;
		m_Scale = {1.0f, 1.0f, 1.0f};
		
		// create a new window with its own renderer and surface
		m_NativeWindow = Platform::CreateWindow(
			m_Prefs.title.c_str(), 
			m_Prefs.x, 
			m_Prefs.y, 
			(int)round(m_Prefs.width * m_Scale.x), 
			(int)round(m_Prefs.height * m_Scale.y), 
			winFlags
			);
		
		m_Renderer = Platform::InitRenderer(m_NativeWindow);
		m_WindowSurface = Platform::GetWindowSurface(m_NativeWindow);
		m_Events = Platform::GetEventListener();
		
		CSE_CORE_LOG("Window opened.");
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
		CSE_CORE_LOG("Window renderer destroyed.");
		
		if (m_NativeWindow != nullptr){
			SDL_DestroyWindow(m_NativeWindow);
			m_NativeWindow = nullptr;
		}
		CSE_CORE_LOG("Window destroyed.");
	}
	
	void Window::SetTitle(const std::string& title)
	{
		// SDL_GetWindowTitle(m_NativeWindow);
		SDL_SetWindowTitle(m_NativeWindow, title.c_str());
	}
}
