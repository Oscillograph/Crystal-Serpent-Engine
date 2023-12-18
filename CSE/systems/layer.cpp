#include <CSE/systems/layer.h>
#include <CSE/systems/renderer.h>
#include <CSE/systems/viewport.h>

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
			if (layer->OnAttach())
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
			if (layer->OnDetach())
				return true;
		return false;
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
	
	bool Layer::Attach()
	{
		// CSE_CORE_LOG("Layer ", m_Name, " attached.");
		return true;
	}
	
	bool Layer::OnAttach()
	{
		return true;
	}
	
	bool Layer::Display()
	{
		Renderer::SetActiveCamera(m_Scene->GetActiveCamera());
		m_Scene->UpdateGraphics(CSE::Platform::GetTimeMs());
		
		return true;
	}
	
	bool Layer::OnDisplay()
	{
		return true;
	}
	
	bool Layer::OnEvent(SDL_Event* event)
	{
		CSE_CORE_LOG("Layer ", m_Name, " event: ", event->type);
		return false;
	}
	
	bool Layer::Update(TimeType time)
	{
		if (m_Scene != nullptr)
		{
			m_Scene->OnUpdate(time); // user-defined update scene function
			if (m_Scene->GetPhysicsProcessor() != nullptr)
				m_Scene->UpdatePhysics(time); // engine-defined physics update mechanic
		}
		return true;
	}
	
	bool Layer::OnUpdate(TimeType time)
	{
		return true;
	}
	
	bool Layer::LoadScene(Scene* scene)
	{
		m_Scene = scene;
		m_Scene->SetLayer(this);
		if (!m_Scene->IsInitialized())
		{
			m_Scene->Init();
			m_Scene->OnInitialized();
		}
		m_Scene->Load();
		m_Scene->OnLoaded();
		return true;
	}
	
	bool Layer::UnloadScene(Scene* scene)
	{
		scene->SetLayer(nullptr);
		m_Scene = nullptr;
		scene->Unload();
		scene->OnUnloaded();
		return true;
	}
	
	bool Layer::Detach()
	{
		// CSE_CORE_LOG("Layer ", m_Name, " detached.");
		UnloadScene(m_Scene);
		return true;
	}
	
	bool Layer::OnDetach()
	{
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


