#ifndef CSE_LAYER_H
#define CSE_LAYER_H

#include <CSE/core.h>
#include <CSE/platform.h>

namespace CSE
{
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
		virtual bool OnDetach();
		
		// graphic user interface API
		virtual void Begin();
		virtual void OnGuiRender(float time);
		virtual void End();
		
		// maintenance
		inline bool IsEnabled() { return m_Enabled; }
		inline void Enable() { m_Enabled = true; }
		inline void Disable() { m_Enabled = false; }
		inline std::string GetName() { return m_Name; }
		
	protected:
		std::string m_Name;
		bool m_Enabled;
	};
	
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();
		
		bool Attach(Layer* layer);
		bool Detach(Layer* layer);
		
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end()   { return m_Layers.end();   }
		
	protected:
		std::vector<Layer*> m_Layers;
		uint32_t m_LayerInsertIndex = 0;
	};
}

#endif
