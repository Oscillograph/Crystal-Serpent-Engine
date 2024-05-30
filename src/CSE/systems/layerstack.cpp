#include <CSE/systems/layerstack.h>

namespace CSE
{
	// ================= LayerStack =================
	LayerStack::LayerStack()
	{
		
	}
	
	LayerStack::~LayerStack()
	{
		/*
		for (Ref<Layer> layer : m_Layers)
		{
			delete layer;
		}
		*/
	}
	
	bool LayerStack::Attach(Ref<Layer> layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		if (layer->Attach())
			return true;
		return false;
	}
	
	bool LayerStack::Detach(Ref<Layer> layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
		
		if (layer->Detach())
			return true;
		return false;
	}
}


