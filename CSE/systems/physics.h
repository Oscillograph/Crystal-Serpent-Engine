#ifndef CSE_PHYSICS_H
#define CSE_PHYSICS_H

#include <CSE/core.h>
#include <CSE/systems/physics/world.h>

// implementation-specific
#include <CSE/systems/physics/physicsAPI.h>


namespace CSE
{
	// forward declarations
	class Scene;
	class Entity;
	// --------------------
	
	// An entity can have a component PhysicsComponent.
	// PhysicsComponent ties the entity to a specific world.
	// Every world is basically a collection with a set of rules applied to all entities in it.
	// We don't need to check scene registry for entities if we have this collection.
	
	class PhysicsProcessor
	{
	public:
		// world management
		static World* CreateWorld();
		static World* CreateWorld(const WorldProperties& props);
		static void DestroyWorld(World* world);
		static World* AccessWorld(int id);
		
		// collisions
		static void CollisionSelection(Entity* A, Entity* B);
		static void CollisionDetection(Entity* A, Entity* B);
		static void CollisionReaction(Entity* A, Entity* B);
		
		// reactions
		
		// processors
		static void GeneralRoutine(Scene* scene);
		
		// general
		static void Init();
		static void Shutdown();
		
	private:
		static std::vector<World*> m_Worlds;
		static PhysicsSystem m_PhysicsSystemSelected;
		static PhysicsAPI* m_API; 
		
		static void Move(Entity* A);
		static void UnMove(Entity* A);
	};
}

#endif
