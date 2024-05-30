#ifndef CSE_LAYERSTACK_H
#define CSE_LAYERSTACK_H

#include <CSE/core.h> // STL, basic CSE level
#include <CSE/systems/layer.h> // Layer

namespace CSE
{
	// Every window is associated with its own layers stack.
	// Every layer represents an abstraction capable of receiving and processing events.
	// Layers are organized so they can pass events down the stack.
	// Henceforth, I need my own Event class to wrap around SDL_Event.
	
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
