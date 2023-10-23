#ifndef CSE_LAYER_H
#define CSE_LAYER_H

#include <CSE/core.h>
#include <CSE/systems/platform.h>
#include <CSE/systems/scene.h>

namespace CSE
{
	class Window; // forward declaration so that a layer can store which window it belongs to.
	
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
		virtual bool OnDisplay();
		virtual bool OnEvent(SDL_Event* event); // TODO: My own events wrapper around SDL_Event so that I can store events and decide when to pass them further 
		virtual bool OnUpdate(float time);
		virtual bool LoadScene(Scene* scene);
		virtual bool UnloadScene(Scene* scene);
		virtual bool OnDetach();
		
		// graphic user interface API
		virtual void Begin();
		virtual void OnGuiRender(float time);
		virtual void End();
		
		// maintenance
		inline std::string GetName() { return m_Name; }
		
		inline bool IsEnabled() { return m_Enabled; }
		inline void Enable() { m_Enabled = true; }
		inline void Disable() { m_Enabled = false; }
		
		inline void SetWindow(Window* window) { m_Window = window; }
		inline Window* GetWindow() { return m_Window; }
		
	protected:
		std::string m_Name;
		bool m_Enabled;
		Window* m_Window = nullptr;
		Scene* m_Scene = nullptr;
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
