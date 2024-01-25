#include <CSE/systems/scene.h>
#include <CSE/systems/entity.h>
#include <CSE/systems/components.h>
#include <CSE/systems/physics.h>

#include <CSE/systems/application.h>
#include <CSE/systems/renderer.h>
#include <CSE/systems/renderer/camera2d.h>
#include <CSE/systems/window.h>
#include <CSE/systems/layer.h>

namespace CSE
{
	Scene::Scene()
	{
		m_SceneCamera = new Camera2D();
		
		m_PhysicsSystem = PhysicsSystem::CSE;
		m_PhysicsProcessor = new PhysicsProcessor(PhysicsSystem::CSE);
		
		m_CurrentWorld = m_PhysicsProcessor->AccessWorld(0);
	}
	
	Scene::Scene(const PhysicsSystem& physicsSystem)
	{
		m_SceneCamera = new Camera2D();
		
		m_PhysicsSystem = physicsSystem;
		m_PhysicsProcessor = new PhysicsProcessor(physicsSystem);
		
		m_CurrentWorld = m_PhysicsProcessor->AccessWorld(0);
	}
	
	Scene::~Scene()
	{
		m_Layer = nullptr;
		
		if (m_SceneCamera != nullptr)
			delete m_SceneCamera;
		m_SceneCamera = nullptr;
	}
	
	void Scene::OnInit()
	{
	}
	
	void Scene::Init()
	{
		CSE_CORE_LOG("Scene: Init...");
		CSE_CORE_LOG("- initialized? Answer: ", m_Initialized);
		CSE_CORE_LOG("- user-defined OnInit");
		OnInit();
		
		// ...
		m_Initialized = true;
		CSE_CORE_LOG("- initialized");
		CSE_CORE_LOG("- initialized? Answer: ", m_Initialized);
		
		CSE_CORE_LOG("- user-defined OnInitialized");
		OnInitialized();
	}
	
	void Scene::OnInitialized()
	{
	}
	
	void Scene::Load()
	{
		// ...
		CSE_CORE_LOG("Scene: Load...");
		if (m_PhysicsProcessor == nullptr)
		{
			CSE_CORE_LOG("- initialize physics processor and access its world");
			m_PhysicsProcessor = new PhysicsProcessor(m_PhysicsSystem);
			m_CurrentWorld = m_PhysicsProcessor->AccessWorld(0);
		}
		CSE_CORE_LOG("- loaded");
		
		CSE_CORE_LOG("- user-defined OnLoaded");
		OnLoaded();
	}
	
	void Scene::OnLoaded()
	{
	}
	
	void Scene::Unload() // should be called before OnUnloaded
	{
		CSE_CORE_LOG("Scene: Unload...");
		// ...
		if (m_PhysicsProcessor != nullptr)
		{
			delete m_PhysicsProcessor;
			m_PhysicsProcessor = nullptr;
			CSE_CORE_LOG("- physics processor unloaded");
		}
		
		if (m_CurrentWorld != nullptr)
		{
			// delete m_CurrentWorld;
			m_CurrentWorld = nullptr;
			CSE_CORE_LOG("- current world is set to nullptr");
		}
		
		CSE_CORE_LOG("- unloaded");
		
		CSE_CORE_LOG("- user-defined OnUnloaded");
		OnUnloaded();
	}
	
	void Scene::OnUnloaded()
	{
	}
	
	void Scene::Update(TimeType sceneTime)
	{
		// . . .
		
		OnUpdate(sceneTime); // user-defined scene update function
		
		if (GetPhysicsProcessor() != nullptr)
			UpdatePhysics(sceneTime); // engine-defined physics update mechanic
		
		// . . .
	}
	
	void Scene::OnUpdate(TimeType sceneTime)
	{
	}
	
	void Scene::Start()
	{
	}
	
	void Scene::Stop()
	{
	}
	
	void Scene::Pause()
	{
	}
	
	void Scene::Resume()
	{
	}
	
	void Scene::UpdatePhysics(TimeType sceneTime) // calls physics processor general routine
	{
		if (m_PhysicsProcessor != nullptr)
		{
			m_PhysicsProcessor->GeneralRoutine(this, sceneTime);
			// CSE_CORE_LOG("Physics processor general routine called and processed");
		}
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
	
	Entity* Scene::CreatePhysicalEntity(const std::string& name)
	{
		Entity* entity = CreateEntity(name);
		entity->AddComponent<PhysicsComponent>();
		m_PhysicsProcessor->RegisterEntity(entity);
		
		return entity;
	}
	
	void Scene::DestroyEntity(Entity& entity)
	{
		if (entity.HasComponent<PhysicsComponent>())
			m_PhysicsProcessor->UnregisterEntity(&entity);
		
		m_Registry.destroy(entity.GetID());
	}
}


