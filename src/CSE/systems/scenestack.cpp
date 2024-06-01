#include <CSE/systems/scenestack.h>
#include <CSE/systems/scene.h>

namespace CSE
{
	SceneStack::SceneStack()
	{
		
	}
	
	SceneStack::~SceneStack()
	{
		for (Scene* scene : m_Scenes)
		{
			delete scene;
			scene = nullptr;
		}
	}
	
	void SceneStack::Load(Scene* scene)
	{
		m_Scenes.emplace(m_Scenes.begin() + m_SceneInsertIndex, scene);
		if (!scene->IsInitialized())
		{
			scene->Init();
		}
		scene->Load();
		
		m_SceneInsertIndex++;
	}
	
	void SceneStack::Unload(Scene* scene)
	{
		scene->Unload();
	}
	
	void SceneStack::Remove(Scene* scene)
	{
		Unload(scene);
		
		auto it = std::find(m_Scenes.begin(), m_Scenes.end(), scene);
		if (it != m_Scenes.end())
		{
			m_Scenes.erase(it);
			delete scene;
			m_SceneInsertIndex--;
		}
	}
}


