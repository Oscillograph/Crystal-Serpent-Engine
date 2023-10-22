#ifndef CSE_APPLICATION_H
#define CSE_APPLICATION_H

#include <CSE/core.h>

// platform-dependent systems
#include <CSE/systems/platform.h> // SDL
#include <CSE/systems/window.h> // CSE interfaces are based on gui
#include <CSE/systems/renderer.h> // graphics

// architecture abstractions
#include <CSE/systems/layer.h>
#include <CSE/systems/scene.h>

#include <CSE/systems/entity.h>
#include <CSE/systems/components.h>

namespace CSE
{
	// Main CSE application which can have windows
	class Application
	{
	public:
		Application();
		Application(const WindowPrefs& prefs = {CSE::EngineInfo::Name, 0, 0, 620, 480});
		virtual ~Application();
		
		int Init();
		int Run();
		
		void LimitFPS(float fps);
		
		// layers functionality
		// TODO: DANGER! one layer can be attached to two different windows. Need smart pointers.
		bool AttachLayer(Window* window, Ref<Layer> layer);
		bool DetachLayer(Window* window, Ref<Layer> layer);
		
		// multiple windows functionality
		inline WindowStack& GetWindows() { return m_WindowStack; }
		
		// only one application allowed to run at a time
		inline static Application* Get() { return m_ApplicationInstance; }
		inline static void Set(Application* app) { 
			if (m_ApplicationInstance != nullptr) 
				delete m_ApplicationInstance; 
			m_ApplicationInstance = app; 
		}
		
	protected:
		bool m_Running = true;
		WindowStack m_WindowStack;
		
	protected:
		static Application* m_ApplicationInstance;
		
		uint64_t m_TimeLastFrame = 0;
		uint64_t m_TimeThisFrame = 0;
		uint64_t fpsCount;
		float m_TimeDelta = 0;
		float m_TimeDeltaLimit = 0.0f;
	};
	
	Application* CreateApplication();
}

#endif
