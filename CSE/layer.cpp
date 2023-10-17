#include <CSE/layer.h>

namespace CSE
{
	// ================= LayerStack =================
	LayerStack::LayerStack()
	{
		
	}
	
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			delete layer;
		}
	}
	
	bool LayerStack::Attach(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		return layer->OnAttach();
	}
	
	bool LayerStack::Detach(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
		return layer->OnDetach();
	}
	
	// ================= Layer =================
	Layer::Layer(const std::string& name)
	{
		m_Name = name;
		m_Enabled = true;
	}
	
	Layer::~Layer(){
		
	}
	
	bool Layer::OnAttach(){
		CSE_CORE_LOG("Layer ", m_Name, " attached.");
		return true;
	}
	
	bool Layer::OnDisplay(){
		return true;
	}
	
	bool Layer::OnEvent(SDL_Event* event){
		CSE_CORE_LOG("Layer ", m_Name, " event: ", event->type);
		return false;
	}
	
	bool Layer::OnUpdate(float time){
		return true;
	}
	
	bool Layer::OnDetach(){
		CSE_CORE_LOG("Layer ", m_Name, " detached.");
		return true;
	}
	
	void Layer::Begin(){
		
	}
	
	void Layer::OnGuiRender(float time){
		
	}
	
	void Layer::End(){
		
	}
}


