#ifndef CSE_SCENE_H
#define CSE_SCENE_H

#include <CSE/core.h>

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
		
		virtual void OnLoaded(Layer* layer);
		virtual void OnUnloaded(Layer* layer);
		
		// Picks every entity it has and updates their components depending on the system involved.
		virtual void Update(float sceneTime);
		
		Entity* CreateEntity(const std::string& name = "");
		Entity* CreateEntityWithUUID(const std::string& name = "", uint64_t uuid = 0);
		void DestroyEntity(Entity& entity);
		
		inline entt::registry& GetRegistry() { return m_Registry; };
		inline bool HasEntity(uint32_t entityID) { return m_Registry.valid((entt::entity)entityID); };
		
	private:
		Layer* m_Layer; // so that the scene knows where it is loaded
		uint32_t m_SceneID;
		entt::registry m_Registry;
	};
}


#endif
