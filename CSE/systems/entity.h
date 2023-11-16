#ifndef CSE_ENTITY_H
#define CSE_ENTITY_H

#include <CSE/core.h>
#include <CSE/systems/scene.h>

#include <CSE/vendor/entt/entt.hpp>

namespace CSE
{
	// It is basically a couple of variables with an interface to the scene's registry
	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity& other) = default; // copy constructor
		Entity(entt::entity entityID, Scene* scene)
		: m_EntityID(entityID), m_Scene(scene)
		{}
		
		~Entity()
		{
			m_Scene = nullptr;
		}
		
		inline entt::entity GetID(){ return m_EntityID; }
		inline Scene* GetScene(){ return m_Scene; };
		
		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			CSE_CORE_ASSERT(!HasComponent<T>(), "Entity # already has the component.");
			return m_Scene->GetRegistry().emplace<T>(this->m_EntityID, std::forward<Args>(args)...);
		}
		
		template <typename T>
		T& GetComponent()
		{
			// BSE_ASSERT(HasComponent<T>(), "Entity doesn't have the component.");
			return m_Scene->GetRegistry().get<T>(this->m_EntityID);
		}
		
		template <typename T>
		bool HasComponent()
		{
			return m_Scene->GetRegistry().all_of<T>(this->m_EntityID);
		}
		
		template <typename T>
		void RemoveComponent()
		{
			CSE_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have the component.");
			return m_Scene->GetRegistry().erase<T>(this->m_EntityID);
		}
		
		// check if exists
		operator bool() 
		{ 
			return (this->m_EntityID != entt::null); 
		}
		
		// comparing entities
		bool operator==(Entity* other) 
		{
			return ((this->m_EntityID == other->GetID()) && (this->m_Scene == other->GetScene()));
		};
		bool operator!=(Entity* other) 
		{
			return !(this == other);
		};
		bool operator==(Entity& other) 
		{
			return ((m_EntityID == other.GetID()) && (m_Scene == other.GetScene()));
		};
		bool operator!=(Entity& other) 
		{
			return !(*this == other);
		};
		
	private:
		entt::entity m_EntityID;
		Scene* m_Scene = nullptr;
	};
}

#endif
