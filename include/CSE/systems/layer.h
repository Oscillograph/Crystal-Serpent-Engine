#ifndef CSE_LAYER_H
#define CSE_LAYER_H

#include <CSE/core.h>
#include <CSE/systems/platform.h>
// #include <CSE/systems/input.h>

namespace CSE
{
	class Window; // forward declaration so that a layer can store which window it belongs to.
	class Viewport;
	class Scene;
	struct InputKeyEvent;
	
	// Every window is associated with its own layers stack.
	// Every layer represents an abstraction capable of receiving and processing events.
	// Layers are organized so they can pass events down the stack.
	// Henceforth, I need my own Event class to wrap around SDL_Event.
	
	class Layer 
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		
		// general API
		virtual bool OnAttach();
		bool Attach();
		virtual bool OnDisplay();
		bool Display();
		virtual bool OnEvent(SDL_Event* event); // TODO: My own events wrapper around SDL_Event so that I can store events and decide when to pass them further
		virtual bool OnEvent(InputKeyEvent* event);
		virtual bool OnUpdate(TimeType time);
		bool Update(TimeType time);
		bool LoadScene(Scene* scene);
		bool UnloadScene(Scene* scene);
		virtual bool OnDetach();
		bool Detach();
		
		// graphic user interface API
		virtual void Begin();
		virtual void OnGuiRender(float time);
		virtual void End();
		
		// general routines
		void Animate(TimeType sceneTime); // updates AnimationComponent
		void Draw(); // draw anything with SpriteComponent
		
		// maintenance
		inline std::string GetName() { return m_Name; }
		
		inline void SetWindow(Window* window) { m_Window = window; }
		inline Window* GetWindow() { return m_Window; }
		
		inline bool HasScene() { return (m_Scene != nullptr); }
		inline Scene* GetScene() { return m_Scene; }
		inline void SetScene(Scene* scene) { m_Scene = scene; }
		inline bool IsEnabled() { return m_Enabled; }
		inline void Enable() { m_Enabled = true; }
		inline void Disable() { m_Enabled = false; }
		
		inline void SetViewport(Viewport* viewport) { m_Viewport = viewport; }
		inline Viewport* GetViewport() { return m_Viewport; }
	protected:
		std::string m_Name;
		bool m_Enabled;
		Window* m_Window = nullptr;
		Scene* m_Scene = nullptr;
		Viewport* m_Viewport = nullptr; // TODO: allow a collection of viewports be set up for a single layer
	};
	
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();
		
		bool Attach(Ref<Layer> layer);
		bool Detach(Ref<Layer> layer);
		
		inline const std::vector<Ref<Layer>>& GetContents() const { return m_Layers; }
		
		std::vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<Ref<Layer>>::iterator end()   { return m_Layers.end();   }
		
	protected:
		std::vector<Ref<Layer>> m_Layers;
		uint32_t m_LayerInsertIndex = 0;
	};
}

#endif
