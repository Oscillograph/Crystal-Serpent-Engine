#include <CSE/systems/scene.h>
#include <CSE/systems/entity.h>
#include <CSE/systems/components.h>

namespace CSE
{
	Scene::Scene()
	{
	}
	
	Scene::~Scene()
	{
		m_Layer = nullptr;
	}
	
	void Scene::OnLoaded(Layer* layer)
	{
		m_Layer = layer;
	}
	
	void Scene::OnUnloaded(Layer* layer)
	{
		if (m_Layer != nullptr)
			m_Layer = nullptr;
	}
	
	void Scene::Update(float sceneTime)
	{
		// TODO: Scene Update mechanism
		/*
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc){
			if (!nsc.Instance){
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->m_Entity = new Entity(entity, this); // WHAT IS IT
				nsc.Instance->OnCreate();
			}
			nsc.Instance->OnUpdate(sceneTime);
		});
		*/
	}
	
	Entity* Scene::CreateEntity(const std::string& name)
	{
		Entity* entity = new Entity( m_Registry.create(), this );
		
		// TODO: create default components
		auto& nameComponent = entity->AddComponent<NameComponent>();
		nameComponent.value = name.empty() ? "Nameless entity" : name;
		// entity->AddComponent<TransformComponent>();
		return entity; 
	}
	
	Entity* Scene::CreateEntityWithUUID(const std::string& name, uint64_t uuid)
	{
		Entity* entity = CreateEntity(name);
		
		if (uuid == 0) // assume that 0 is not a valid uuid
			uuid = Utils::UUID::Generate();
		auto& uuidComponent = entity->AddComponent<UUIDComponent>(uuid);
		
		return entity; 
	}
	
	void Scene::DestroyEntity(Entity& entity)
	{
		m_Registry.destroy(entity.GetID());
	}
}


