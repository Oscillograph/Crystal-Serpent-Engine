#ifndef CSE_APPLICATION_H
#define CSE_APPLICATION_H

#include <CSE/core.h>

// platform-dependent systems
#include <CSE/platform.h> // SDL
#include <CSE/window.h> // CSE interfaces are based on gui
#include <CSE/renderer.h> // graphics

// architecture abstractions
#include <CSE/layer.h>

namespace CSE
{
	// Main CSE application which can have windows
	class Application
	{
	public:
		Application();
		Application(const WindowPrefs& prefs = {CSE::EngineInfo::Name, 0, 0, 600, 480, 1});
		virtual ~Application();
		
		int Init();
		int Run();
		
		void LimitFPS(float fps);
		
		// layers functionality
		// TODO: Move it into a window
		bool AttachLayer(Layer* layer);
		bool DetachLayer(Layer* layer);
		
		// only one application allowed to run at a time
		inline static Application* Get() { return m_ApplicationInstance; }
		inline static void Set(Application* app) { 
			if (m_ApplicationInstance != nullptr) 
				delete m_ApplicationInstance; 
			m_ApplicationInstance = app; 
		}
		
	protected:
		LayerStack m_LayerStack;
		
		bool m_Running = true;
		
		// assume now we have only one window, but remember: that's not the limit!
		Window* m_Window = nullptr;
		
		SDL_Event* m_Events = nullptr; // TODO: separate and wrap into a custom Event class
		
	private:
		static Application* m_ApplicationInstance;
		
		uint64_t m_TimeLastFrame = 0; // SDL_GetTicks64()
		uint64_t m_TimeThisFrame = 0;
		float m_TimeDeltaLimit = 0.0f;
	};
	
	Application* CreateApplication();
}

#endif
