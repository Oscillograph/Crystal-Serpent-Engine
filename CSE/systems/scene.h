#ifndef CSE_SCENE_H
#define CSE_SCENE_H

#include <CSE/core.h>

#include <CSE/systems/platform.h>
#include <CSE/systems/physics.h>

#include <CSE/vendor/entt/entt.hpp>

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
		
		virtual void Init(); // user-defined, should be called after constructor
		virtual void OnInitialized();
		virtual void Load(); // user-defined
		virtual void OnLoaded();
		virtual void Unload(); // user-defined
		virtual void OnUnloaded();
		virtual void OnUpdate(TimeType sceneTime); // before the update happens
		
		// TODO: scene start/stop/pause/resume
		virtual void Start(); // run the scene
		virtual void Stop(); // stop the scene
		virtual void Pause(); // pause the scene (only if it is running)
		virtual void Resume(); // unpause the scene (only if it is running)
		
		inline bool IsRunning() { return m_Running; }
		inline bool IsPaused() { return m_Paused; }
		
		inline bool IsInitialized() { return m_Initialized; }
		inline void SetLayer(Layer* layer) { m_Layer = layer; }
		inline Layer* GetLayer() { return m_Layer; }
		inline PhysicsProcessor* GetPhysicsProcessor() { return m_PhysicsProcessor; }
		inline Camera2D* GetDefaultCamera() { return m_SceneCamera; }
		inline Camera2D* GetActiveCamera() { return (m_ActiveCamera == nullptr) ? GetDefaultCamera() : m_ActiveCamera; }
		inline void SetCamera(Camera2D* camera) { m_ActiveCamera = camera; }
		inline void SetCameraDefault() { m_ActiveCamera = nullptr; } // there must be always a camera
		
		// Picks every entity it has and updates their components depending on the system involved.
		// void Input();
		// no need - virtual void UpdateGraphics(TimeType sceneTime);
		// no need - void Animate(TimeType sceneTime); // updates AnimationComponent
		// no need - void Draw(); // draw anything with SpriteComponent
		void UpdatePhysics(TimeType sceneTime); // calls physics processor general routine
		
		Entity* CreateEntity(const std::string& name = "");
		Entity* CreateEntityWithUUID(const std::string& name = "", uint64_t uuid = 0);
		Entity* CreatePhysicalEntity(const std::string& name = "");
		void DestroyEntity(Entity& entity);
		
		inline entt::registry& GetRegistry() { return m_Registry; };
		inline bool HasEntity(uint32_t entityID) { return m_Registry.valid((entt::entity)entityID); };
		
	protected:
		Layer* m_Layer; // so that the scene knows where it is loaded
		uint32_t m_SceneID;
		Camera2D* m_SceneCamera = nullptr;
		Camera2D* m_ActiveCamera = nullptr; // should be set from the client app
		bool m_Running = false;
		bool m_Paused = false;
		bool m_Initialized = false;
		entt::registry m_Registry;
		PhysicsProcessor* m_PhysicsProcessor;
		World* m_CurrentWorld = nullptr;
	};
}


#endif
