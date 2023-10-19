#include <CSE/systems/application.h>

namespace CSE
{
	Application* Application::m_ApplicationInstance = nullptr;
	// extern Application* CreateApplication();
	
	Application::Application()
	{
		Init();
		m_Window = new Window();
	}
	
	Application::Application(const WindowPrefs& prefs)
	{
		Init();
		
		m_Window = new Window(prefs);
		
		m_Events = nullptr;
		// m_Events = Platform::GetEventListener();
		
		// initialize randomizer
		tm randomTime;
		srand(
			randomTime.tm_sec + 60*randomTime.tm_min + 
			60*60*randomTime.tm_hour + 60*60*24*randomTime.tm_mday + 
			60*60*24*30*randomTime.tm_mon + 60*60*24*30*12*randomTime.tm_year
			);
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
		CSE_CORE_LOG("Platform shutdown complete.");
	}
	
	bool Application::AttachLayer(Window* window, Layer* layer)
	{
		return window->AttachLayer(layer);
	}
	
	bool Application::DetachLayer(Window* window, Layer* layer)
	{
		return window->DetachLayer(layer);
	}
	
	int Application::Init()
	{
		CSE_CORE_ASSERT((m_ApplicationInstance == nullptr), "This application already exists!");
		Platform::InitDefault();
	}
	
	void Application::LimitFPS(float fps)
	{
		m_TimeDeltaLimit = (fps * 1000);
	}
	
	
	int Application::Run()
	{
		Renderer::SetActiveRenderer(m_Window->GetRenderer());
		Renderer::SetBackgroundColor({30, 50, 90, 255});
		
		Texture* broscillograph = new Texture("./CSE/assets/CSE_logo.png", Renderer::GetActiveRenderer());
		
		CSE_CORE_LOG("Starting FPS timer.");
		m_TimeLastFrame = 0;
		m_TimeThisFrame = 0;
		
		CSE_CORE_LOG("Entering the main loop.");
		while (m_Running){
			// 0. Start collecting debug information
			// 1. Input management system
			// 2. Events system
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT){
					m_Running = false;
					break;
				}
				
				if (event.type == SDL_KEYUP){
					if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					{
						m_Running = false;
						break;
					}
				}
				
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					int newWidth, newHeight;
					SDL_GetWindowSize(m_Window->GetNativeWindow(), &newWidth, &newHeight);
					m_Window->SetScale({
						(float)newWidth / m_Window->GetPrefs().width,
						(float)newHeight / m_Window->GetPrefs().height,
						1.0f
					});
				}
				
				for (Layer* layer : m_Window->GetLayers())
				{
					if (layer->IsEnabled())
					{
						if (layer->OnEvent(&event))
							break;
					}
				}
			}
			// 3. App Reaction system
			// 4. Game Object Management System (GOMS)
			// 5. World progress system
			// 		- physics and other world rules subsystems management
			//    control time flow and then update world
			// 6. Asset management system
			// 7. Sound system
			// 8. Show debug information as text over the screen
			// 9. Graphic system
			 
			m_TimeThisFrame = SDL_GetTicks64();
			m_TimeDelta = (float)(m_TimeThisFrame - m_TimeLastFrame);
			if (m_TimeDelta >= m_TimeDeltaLimit)
			{
				
				if (m_TimeDeltaLimit > 0.1f)
				{
					fpsCount = (uint64_t)round(1000 / m_TimeDeltaLimit);
				} else {
					if (m_TimeDelta > 0.0f)
					{
						fpsCount = (uint64_t)round(1000 / m_TimeDelta);
					} else {
						fpsCount = 9001; // it's over nine thousand!
					}
				}
				
				m_Window->ShowFPSInTitle(fpsCount);
				
				Renderer::ClearScreen();
				
				SDL_FRect stretchBro = {
					0, 
					0, 
					m_Window->GetPrefs().width, 
					m_Window->GetPrefs().height
				};
				
				SDL_Rect srcRect = {0, 0, 80, 30};
				
				Renderer::DrawTexture(
					broscillograph->GetTexture(), 
					&stretchBro, 
					NULL, 
					m_Window->GetScale().x, 
					m_Window->GetScale().y
				);
				
				Renderer::Update();
				
				m_TimeLastFrame = m_TimeThisFrame;
			}
			
			
			// 10. File I/O system
			// 11. Log system
			// 12. FPS Count
		}
		CSE_CORE_LOG("Exit from App main loop.");
		
		delete broscillograph; 
		broscillograph = nullptr;
		
		return 0;
	}
}

// ======================== ENTRY POINT ======================== //
extern CSE::Application* CSE::CreateApplication();

int main (int argv, char** argc)
{
	CSE_CORE_LOG("This is an engine logger.");
	CSE_LOG("And this is an app logger.");
	
	// auto* app = new CSE::Application({"My First App", 100, 100, 400, 400, 1});
	auto app = CSE::CreateApplication();
	CSE_LOG("App set up.");
	app->Run();
	delete app;
	CSE_LOG("App destroyed.");
	
	return 0;
}
// ======================== ENTRY POINT ======================== //
