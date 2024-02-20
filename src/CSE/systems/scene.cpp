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
		
		PhysicsInit(PhysicsSystem::None);
	}
	
	Scene::Scene(const PhysicsSystem& physicsSystem)
	{
		m_SceneCamera = new Camera2D();
		
		PhysicsInit(physicsSystem);
	}
	
	Scene::~Scene()
	{
		m_Layer = nullptr;
		
		CSE_CORE_ASSERT(m_SceneCamera, "Scene destructor: m_SceneCamera was a null pointer.");
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
			PhysicsInit(m_PhysicsSystem, false);
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
		PhysicsShutdown();
		
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
	
	void Scene::PhysicsInit(const PhysicsSystem& physicsSystem, bool reconfigureCamera)
	{
		CSE_CORE_LOG("Scene: Initialize physics system of type ", (int)physicsSystem);
		if (physicsSystem != PhysicsSystem::None)
		{
			m_PhysicsSystem = physicsSystem;
			m_PhysicsProcessor = new PhysicsProcessor(physicsSystem);
			
			m_CurrentWorld = m_PhysicsProcessor->AccessWorld(0);
			if (reconfigureCamera)
			{
				m_SceneCamera->SetWorldConstraints({m_CurrentWorld->properties.size.x, m_CurrentWorld->properties.size.y});
				m_SceneCamera->Retarget({
					-m_CurrentWorld->properties.size.x/10,
					-m_CurrentWorld->properties.size.y/10,
					m_CurrentWorld->properties.size.x/10,
					m_CurrentWorld->properties.size.y/10
				});
			}
			PhysicsOn();
		} else {
			m_PhysicsSystem = PhysicsSystem::None;
			m_PhysicsProcessor = new PhysicsProcessor(PhysicsSystem::None);
			
			m_CurrentWorld = nullptr;
			
			if (reconfigureCamera)
			{
				m_SceneCamera->SetWorldConstraints({0.0f, 0.0f});
				m_SceneCamera->Retarget({
					0,
					0,
					0,
					0,
				});
			}
			PhysicsOff();
		}
	}
	
	void Scene::PhysicsOn()
	{
		CSE_CORE_LOG("Scene: physics turn ON");
		m_PhysicsOn = true;
	}
	
	void Scene::PhysicsOff()
	{
		CSE_CORE_LOG("Scene: physics turn OFF");
		m_PhysicsOn = false;
	}
	
	void Scene::PhysicsShutdown()
	{
		CSE_CORE_LOG("Scene: shutdown physics");
		if (m_PhysicsProcessor != nullptr)
			delete m_PhysicsProcessor;
		m_PhysicsProcessor = nullptr;
		PhysicsOff();
		CSE_CORE_LOG("- physics processor unloaded");
	}
	
	void Scene::UpdatePhysics(TimeType sceneTime) // calls physics processor general routine
	{
		if ((m_PhysicsProcessor != nullptr) && m_PhysicsOn)
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


