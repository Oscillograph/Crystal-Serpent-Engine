#ifndef CSE_PHYSICS_PHYSICSAPI_H
#define CSE_PHYSICS_PHYSICSAPI_H

#include <CSE/core.h>

namespace CSE
{
	// This is an API to use any suitable physics engine.
	// The need for it arised from that physics engines are very complex, and their management parts
	// are not to interfere with other game engine systems.
	class PhysicsAPI
	{
	public:
		virtual static void CreateWorld(const WorldProperties& props) = 0;
		virtual static void DestroyWorld(int worldID) = 0;
		
		// collisions
		virtual static void CollisionSelection(Entity* A, Entity* B) = 0;
		virtual static void CollisionDetection(Entity* A, Entity* B) = 0;
		virtual static void CollisionReaction(Entity* A, Entity* B) = 0;
		
		// reactions
		
		// processors
		virtual static void GeneralRoutine(Scene* scene) = 0;
		
		// general
		virtual static void Shutdown() = 0;
	};
	
	extern PhysicsAPI* CreatePhysicsProcessor();
}

#endif
