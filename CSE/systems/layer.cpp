#include <CSE/systems/layer.h>
#include <CSE/systems/renderer.h>

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
		return layer->OnAttach();
	}
	
	bool LayerStack::Detach(Ref<Layer> layer)
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
	
	Layer::~Layer()
	{
		m_Window = nullptr;
	}
	
	bool Layer::OnAttach()
	{
		// CSE_CORE_LOG("Layer ", m_Name, " attached.");
		return true;
	}
	
	bool Layer::OnDisplay()
	{
		Renderer::SetActiveCamera(&(m_Scene->GetCamera()));
		m_Scene->UpdateGraphics(CSE::Platform::GetTimeMs());
		
		return true;
	}
	
	bool Layer::OnEvent(SDL_Event* event)
	{
		CSE_CORE_LOG("Layer ", m_Name, " event: ", event->type);
		return false;
	}
	
	bool Layer::OnUpdate(TimeType time)
	{
		if (m_Scene != nullptr)
		{
			m_Scene->OnUpdate(time);
		}
		return true;
	}
	
	bool Layer::LoadScene(Scene* scene)
	{
		m_Scene = scene;
		m_Scene->SetLayer(this);
		m_Scene->Init();
		m_Scene->OnLoaded();
		return true;
	}
	
	bool Layer::UnloadScene(Scene* scene)
	{
		scene->SetLayer(nullptr);
		m_Scene = nullptr;
		scene->OnUnloaded();
		return true;
	}
	
	bool Layer::OnDetach()
	{
		// CSE_CORE_LOG("Layer ", m_Name, " detached.");
		UnloadScene(m_Scene);
		return true;
	}
	
	void Layer::Begin()
	{
	}
	
	void Layer::OnGuiRender(float time)
	{
	}
	
	void Layer::End()
	{
	}
}


