#ifndef CSE_SCENE_H
#define CSE_SCENE_H

#include <CSE/core.h>

#include <CSE/systems/platform.h>

#include <CSE/vendor/entt/entt.hpp>

namespace CSE
{
	class Layer;
	class Entity;
	
	// Scene is meant to be loaded onto a layer.
	// Only one scene per layer!
	class Scene {
	public:
		Scene();
		virtual ~Scene();
		
		virtual void OnLoaded();
		virtual void OnUnloaded();
		virtual void OnUpdate(TimeType sceneTime); // before the update happens
		
		// TODO: scene start/stop/pause/resume
		virtual void Start(); // run the scene
		virtual void Stop(); // stop the scene
		virtual void Pause(); // pause the scene (only if it is running)
		virtual void Resume(); // unpause the scene (only if it is running)
		
		inline bool IsRunning() { return m_Running; }
		inline bool IsPaused() { return m_Paused; }
		
		inline void SetLayer(Layer* layer) { m_Layer = layer; }
		inline Layer* GetLayer() { return m_Layer; }
		
		// Picks every entity it has and updates their components depending on the system involved.
		virtual void UpdateGraphics(TimeType sceneTime);
		// void Input();
		void Animate(TimeType sceneTime); // updates AnimationComponent
		void Draw(); // draw anything with SpriteComponent
		
		Entity* CreateEntity(const std::string& name = "");
		Entity* CreateEntityWithUUID(const std::string& name = "", uint64_t uuid = 0);
		void DestroyEntity(Entity& entity);
		
		inline entt::registry& GetRegistry() { return m_Registry; };
		inline bool HasEntity(uint32_t entityID) { return m_Registry.valid((entt::entity)entityID); };
		
	protected:
		Layer* m_Layer; // so that the scene knows where it is loaded
		uint32_t m_SceneID;
		bool m_Running = false;
		bool m_Paused = false;
		entt::registry m_Registry;
	};
}


#endif
