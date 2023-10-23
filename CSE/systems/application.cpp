#include <CSE/systems/application.h>

namespace CSE
{
	Application* Application::m_ApplicationInstance = nullptr;
	
	Application::Application()
	{
		Init();
		// m_WindowStack.Push(new Window());
	}
	
	Application::Application(const WindowPrefs& prefs)
	{
		Init();
		
		// m_WindowStack.Push(new Window(prefs));
		
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
		// some internal pointers memory deallocating
		CSE_CORE_LOG("Internal systems pointers deleted.");
		
		Platform::Shutdown();
		CSE_CORE_LOG("Platform shutdown complete.");
	}
	
	bool Application::AttachLayer(Window* window, Ref<Layer> layer)
	{
		layer->SetWindow(window);
		return window->AttachLayer(layer);
	}
	
	bool Application::DetachLayer(Window* window, Ref<Layer> layer)
	{
		layer->SetWindow(nullptr);
		return window->DetachLayer(layer);
	}
	
	int Application::Init()
	{
		CSE_CORE_ASSERT((m_ApplicationInstance == nullptr), "This application already exists!");
		return Platform::InitDefault();
	}
	
	void Application::LimitFPS(float fps)
	{
		m_TimeDeltaLimit = (fps * 1000);
	}
	
	
	int Application::Run()
	{
		Texture* broscillograph = nullptr;
		
		for (Window* window : m_WindowStack)
		{
			Renderer::SetActiveRenderer(window->GetRenderer());
			Renderer::SetBackgroundColor({30, 50, 90, 255});
		}
		
		broscillograph = new Texture("./CSE/assets/CSE_logo.png", (m_WindowStack.GetContents())[0]->GetRenderer());
		SDL_FRect stretchBro = {
			0,
			0,
			0,
			0
		};
		SDL_Rect srcRect = {0, 0, 80, 30};
		
		CSE_CORE_LOG("Starting FPS timer.");
		m_TimeLastFrame = 0;
		m_TimeThisFrame = 0;
		
		CSE_CORE_LOG("Entering the main loop.");
		SDL_Event event; // for events
		while (m_Running){
			// 0. Start collecting debug information
			// 1. Input management system
			// 2. Events system
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					m_Running = false;
					break;
				}
				
				if (event.type == SDL_KEYUP)
				{
					if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					{
						m_Running = false;
						break;
					}
				}
				
				if (event.type == SDL_WINDOWEVENT)
				{
					for (Window* window : m_WindowStack)
					{
						if (event.window.event == SDL_WINDOWEVENT_CLOSE)
						{
							if (event.window.windowID == window->GetNativeWindowID())
							{
								m_WindowStack.Pop(window);
								break;
							}
						}
						
						if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
						{
							int newWidth, newHeight;
							SDL_GetWindowSize(window->GetNativeWindow(), &newWidth, &newHeight);
							window->SetScale({
								(float)newWidth / window->GetPrefs().width,
								(float)newHeight / window->GetPrefs().height,
								1.0f
							});
						}
						
						if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
						{
							window->SetFocus(true);
						}
						
						if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
						{
							window->SetFocus(false);
						}
					}
				}
				
				for (Window* window : m_WindowStack)
				{
					if (window->IsFocused())
					{
						for (Ref<Layer> layer : window->GetLayers())
						{
							if (layer->IsEnabled())
							{
								if (layer->OnEvent(&event))
									break;
							}
						}
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
				
				for (Window* window : m_WindowStack)
				{
					window->ShowFPSInTitle(fpsCount);
					
					if (window->GetRenderer() != Renderer::GetActiveRenderer())
					{
						Renderer::SetActiveRenderer(window->GetRenderer());
					}
					Renderer::ClearScreen();
					
					// display every layer
					for (Ref<Layer> layer : window->GetLayers())
					{
						if (layer->IsEnabled())
						{
							layer->OnDisplay();
						}
					}
					
					/*
					stretchBro.w = window->GetPrefs().width;
					stretchBro.h = window->GetPrefs().height;

					Renderer::DrawTexture(
						broscillograph->GetTexture(), 
						&stretchBro, 
						NULL, 
						window->GetScale().x, 
						window->GetScale().y
						);
					*/
					
					Renderer::Update();
				}
				m_TimeLastFrame = m_TimeThisFrame;
			}
			
			
			// 10. File I/O system
			// 11. Log system
			// 12. FPS Count
			
			if (m_WindowStack.GetContents().size() == 0)
				m_Running = false;
		}
		CSE_CORE_LOG("Exit from App main loop.");
		
		if (broscillograph != nullptr)
		{
			delete broscillograph; 
			broscillograph = nullptr;
		}
		// CSE_CORE_LOG("Texture deleted.");
		
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
	CSE_CORE_LOG("Engine offline.");
	
	return 0;
}
// ======================== ENTRY POINT ======================== //
