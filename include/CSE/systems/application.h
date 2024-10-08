#ifndef CSE_APPLICATION_H
#define CSE_APPLICATION_H

#include <CSE/core.h>

// platform-dependent systems
#include <CSE/systems/event.h>
#include <CSE/systems/platform.h> // SDL
#include <CSE/systems/window.h> // CSE interfaces are based on gui
#include <CSE/systems/windowstack.h> // WindowStack
#include <CSE/systems/renderer.h> // graphics
#include <CSE/systems/input.h> // keyboard and other manipulators input

// architecture abstractions
#include <CSE/systems/layer.h>
#include <CSE/systems/viewport.h>

#include <CSE/systems/entity.h>
#include <CSE/systems/components.h>

#include <CSE/systems/canban.h>

namespace CSE
{
	class Scene;
	class SceneStack;
//	class Event;
	
	// Main CSE application which can have windows
	class Application
	{
	public:
		Application();
		Application(const WindowPrefs& prefs);
		virtual ~Application();
		
		int Init();
		int Run();
		
		void LimitFPS(float fps);
		
		// Events processing
		void EventProcessor(Event* event);
		
		// layers functionality
		// TODO: DANGER! one layer can be attached to two different windows. Need smart pointers.
		bool AttachLayer(Window* window, Ref<Layer> layer);
		bool DetachLayer(Window* window, Ref<Layer> layer);
		
		// multiple windows functionality
		inline WindowStack& GetWindows() { return m_WindowStack; }
		Window* NewWindow(const WindowPrefs& prefs) { Window* window = new CSE::Window(prefs); GetWindows().Push(window); return window; }
		
		// multiple scenes functionality
		inline SceneStack* GetScenes() { return m_SceneStack; }
		Scene* NewScene(Scene* scene);
		
		// only one application allowed to run at a time
		inline static Application* Get() { return m_ApplicationInstance; }
		inline static void Set(Application* app) { 
			if (m_ApplicationInstance != nullptr) 
				delete m_ApplicationInstance; 
			m_ApplicationInstance = app; 
		}
		inline static bool IsRenderWireframes() { return m_RenderWireframes; }
		
	protected:
		bool m_Running = true;
		bool m_TaskToDo = false;
		WindowStack m_WindowStack;
		SceneStack* m_SceneStack = nullptr;
		
	protected:
		static Application* m_ApplicationInstance;
		
		uint64_t m_TimeLastFrame = 0;
		uint64_t m_TimeThisFrame = 0;
		uint64_t fpsCount;
		uint64_t m_frameCounter; // to know how many frames passed
		float m_TimeDelta = 0;
		float m_TimeDeltaLimit = 0.0f;
		// float m_TimeDeltaLimit = CSE_FPS60;
		
		static bool m_RenderWireframes;
	};
	
	Application* CreateApplication();
}

#endif
