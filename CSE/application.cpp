#include <CSE/application.h>

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
		
		m_Events = Platform::GetEventListener();
		
		// initialize randomizer
		srand(SDL_GetTicks64());
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
	
	bool Application::AttachLayer(Layer* layer)
	{
		return m_LayerStack.Attach(layer);
	}
	
	bool Application::DetachLayer(Layer* layer)
	{
		return m_LayerStack.Detach(layer);
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
		
		Texture* broscillograph = new Texture("./CSE/assets/Sprites.png", Renderer::GetActiveRenderer());
		SDL_FRect stretchBro = {0, 0, 320, 240};
		
		CSE_CORE_LOG("Starting FPS timer.");
		m_TimeLastFrame = 0;
		m_TimeThisFrame = 0;
		
		CSE_CORE_LOG("Entering the main loop.");
		while (m_Running){
			// 0. Start collecting debug information
			// 1. Input management system
			while (SDL_PollEvent(m_Window->GetEvents()))
			{
				if (m_Window->GetEvents()->type == SDL_QUIT){
					m_Running = false;
					break;
				}
				
				if (m_Window->GetEvents()->type == SDL_KEYUP){
					if (m_Window->GetEvents()->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					{
						m_Running = false;
						break;
					}
				}
				
				for (Layer* layer : m_LayerStack)
				{
					if (layer->IsEnabled())
					{
						if (layer->OnEvent(m_Window->GetEvents()))
							break;
					}
				}
			}
			// 2. Events system
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
			if (m_TimeThisFrame - m_TimeLastFrame >= (uint64_t)m_TimeDeltaLimit)
			{
				if (m_TimeDeltaLimit > 0.1f)
				{
					CSE_CORE_LOG("FPS: ", (uint64_t)round(1000 / m_TimeDeltaLimit));
				} else {
					CSE_CORE_LOG("FPS: ", (uint64_t)round((float)1000 / (m_TimeThisFrame - m_TimeLastFrame)));
				}
				
				Renderer::ClearScreen();
				
				Renderer::DrawTexture(broscillograph->GetTexture(), &stretchBro, NULL, 1.0f, 1.0f);
				Renderer::Update();
				
				m_TimeLastFrame = m_TimeThisFrame;
			}
			
			// 10. File I/O system
			// 11. Log system
			// 12. FPS Count
		}
		CSE_CORE_LOG("Exit from App main loop.");
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
