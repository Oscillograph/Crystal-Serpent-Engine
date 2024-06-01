#ifndef CSE_SCENE_H
#define CSE_SCENE_H

#include <CSE/core.h>

#include <CSE/systems/physics.h> // PhysicSystem enum, PhysicsProcessor

#include <entt/entt.hpp> // entt::registry

namespace CSE
{
	class Layer;
	class Entity;
	class Camera2D;
	class World;
	
	// TODO: Scene serialization mechanism
	// TODO: Scene reset mechanism
	// TODO: Scene store/load/unload content mechanism
	// TODO: Canban board to implement communication between scenes from different layers
	// TODO: Special kind of scenes for UI or HUD
	
	// Scene is meant to be loaded onto a layer.
	// Only one scene per layer!
	class Scene {
	public:
		Scene();
		Scene(const PhysicsSystem& physicsSystem);
		virtual ~Scene();
		
		// Routines and their callbacks
		virtual void OnInit(); // user-defined, should be called after constructor
		virtual void Init(); 
		virtual void OnInitialized(); // user-defined, should be called after initialization
		virtual void Load();
		virtual void OnLoaded(); // user-defined
		virtual void Unload(); 
		virtual void OnUnloaded(); // user-defined
		virtual void Update(TimeType sceneTime);
		virtual void OnUpdate(TimeType sceneTime);
		
		// TODO: scene start/stop/pause/resume
		virtual void Start(); // run the scene
		virtual void Stop(); // stop the scene
		virtual void Pause(); // pause the scene (only if it is running)
		virtual void Resume(); // unpause the scene (only if it is running)
		
		// Scene status checks
		inline bool IsRunning() { return m_Running; }
		inline bool IsPaused() { return m_Paused; }
		inline bool IsInitialized() { return m_Initialized; }
		
		// Physics processor API
		inline PhysicsProcessor* GetPhysicsProcessor() { return m_PhysicsProcessor; }
		void PhysicsInit(const PhysicsSystem& physicsSystem = PhysicsSystem::None, bool reconfigureCamera = true);
		void PhysicsOn();
		void PhysicsOff();
		void PhysicsShutdown();
		void UpdatePhysics(TimeType sceneTime); // calls physics processor general routine
		inline PhysicsSystem GetPhysicsSystem() { return m_PhysicsSystem; }
		
		// Camera API
		inline Camera2D* GetDefaultCamera() { return m_SceneCamera; }
		inline Camera2D* GetActiveCamera() { return (m_ActiveCamera == nullptr) ? GetDefaultCamera() : m_ActiveCamera; }
		inline void SetCamera(Camera2D* camera) { m_ActiveCamera = camera; }
		inline void SetCameraDefault() { m_ActiveCamera = nullptr; } // there must be always a camera
		
		// Picks every entity it has and updates their components depending on the system involved.
		// void Input();

		// ECS mechanics
		Entity* CreateEntity(const std::string& name = "");
		Entity* CreateEntityWithUUID(const std::string& name = "", uint64_t uuid = 0);
		Entity* CreatePhysicalEntity(const std::string& name = "");
		void DestroyEntity(Entity& entity);
		// EnTT specific methods
		inline entt::registry& GetRegistry() { return m_Registry; };
		inline bool HasEntity(uint32_t entityID) { return m_Registry.valid((entt::entity)entityID); };
		
	protected:
		uint32_t m_SceneID;
		Camera2D* m_SceneCamera = nullptr;
		Camera2D* m_ActiveCamera = nullptr; // should be set from the client app
		bool m_Running = false;
		bool m_Paused = false;
		bool m_Initialized = false;
		bool m_PhysicsOn = false;
		entt::registry m_Registry;
		PhysicsSystem m_PhysicsSystem;
		PhysicsProcessor* m_PhysicsProcessor;
		World* m_CurrentWorld = nullptr;
	};
}


#endif
