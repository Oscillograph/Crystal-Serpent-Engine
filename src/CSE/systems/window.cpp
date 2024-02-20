#include <CSE/systems/window.h>

namespace CSE
{
	Window::Window()
	{
		Open(
			{CSE::EngineInfo::Name, 0, 0, 600, 480},
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
		// m_Events = Platform::GetEventListener();
		m_Events = nullptr;
		m_NativeWindowID = SDL_GetWindowID(m_NativeWindow);
		
		CSE_CORE_LOG("Window opened.");
	}
	
	void Window::Close()
	{
		// destroy surfaces
		if (m_WindowSurface != nullptr){
			SDL_FreeSurface(m_WindowSurface);
			m_WindowSurface = nullptr;
		}
		CSE_CORE_LOG("Window \"", m_Prefs.title, "\" surface memory freed.");
		
		// destroy renderer and the window
		if (m_Renderer != nullptr){
			// CSE_CORE_LOG("Window", m_Prefs.title, " renderer is to be destroyed.");
			SDL_DestroyRenderer(m_Renderer);
			m_Renderer = nullptr;
		} else {
			// CSE_CORE_LOG("Window", m_Prefs.title, " renderer is null somehow.");
		}
		CSE_CORE_LOG("Window \"", m_Prefs.title, "\" renderer destroyed.");
		
		if (m_NativeWindow != nullptr){
			SDL_DestroyWindow(m_NativeWindow);
			m_NativeWindow = nullptr;
		}
		CSE_CORE_LOG("Window \"", m_Prefs.title, "\" destroyed.");
	}
	
	void Window::SetTitle(const std::string& title)
	{
		SDL_SetWindowTitle(m_NativeWindow, title.c_str());
	}
	
	void Window::NewTitle(const std::string& title)
	{
		SetTitle(title);
		m_Prefs.title = title;
	}
	
	void Window::ShowFPSInTitle(uint64_t fpsCount)
	{
		// TODO: find a way around stringstream leaking memory per << operator.
		// std::stringstream newTitle;
		// newTitle.str(std::string());
		// newTitle << GetBaseTitle() << " - FPS: " << fpsCount;
		// SetTitle(newTitle.str());
		
		sprintf(m_NewTitle, "%s - FPS: %d", m_Prefs.title.c_str(), fpsCount);
		std::string fpsString(m_NewTitle);
		SetTitle(fpsString);
	}
	
	bool Window::AttachLayer(Ref<Layer> layer)
	{
		return m_LayerStack.Attach(layer);
	}
	
	bool Window::DetachLayer(Ref<Layer> layer)
	{
		return m_LayerStack.Detach(layer);
	}
	
	//
	// ================= WindowStack =================
	WindowStack::WindowStack()
	{
	}
	
	WindowStack::~WindowStack()
	{
	}
	
	void WindowStack::Push(Window* window)
	{
		m_Windows.emplace(m_Windows.begin() + m_WindowInsertIndex, window);
		m_WindowInsertIndex++;
	}
	
	void WindowStack::Pop(Window* window)
	{
		if (window != nullptr)
		{
			auto it = std::find(m_Windows.begin(), m_Windows.end(), window);
			if (it != m_Windows.end())
			{
				m_Windows.erase(it);
				delete window;
				m_WindowInsertIndex--;
			}
		}
	}
}
