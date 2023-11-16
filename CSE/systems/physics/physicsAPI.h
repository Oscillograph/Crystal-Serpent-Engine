#ifndef CSE_PHYSICS_PHYSICSAPI_H
#define CSE_PHYSICS_PHYSICSAPI_H

#include <CSE/core.h>
#include <CSE/systems/physics/world.h>

namespace CSE
{
	// forward declarations
	class Scene;
	class Entity;
	// --------------------
	
	enum class PhysicsSystem 
	{
		None		= 0,
		CSE	 		= 1,
		Box2D		= 2
	};
	
	// This is an API to use any suitable physics engine.
	// The need for it arised from that physics engines are very complex, and their management parts
	// are not to interfere with other game engine systems.
	class PhysicsAPI
	{
	public:
		// virtual ~PhysicsAPI() = 0;
		
		virtual void CreateWorld(const WorldProperties& props) = 0;
		virtual void DestroyWorld(int worldID) = 0;
		
		virtual void RegisterEntity(Entity* A) = 0; // make the entity seen by the physics processor
		virtual void UnregisterEntity(Entity* A) = 0; // make it invisible again
		
		// collisions
		virtual void CollisionSelection(Entity* A, Entity* B) = 0;
		virtual void CollisionDetection(Entity* A, Entity* B) = 0;
		virtual void CollisionReaction(Entity* A, Entity* B) = 0;
		
		// reactions
		
		// processors
		virtual void GeneralRoutine(Scene* scene) = 0;
		
		// general
		virtual void Shutdown() = 0;
	};
	
	PhysicsAPI* CreatePhysicsProcessor(PhysicsSystem psID);
}

#endif
