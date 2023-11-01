#ifndef CSE_PHYSICS_H
#define CSE_PHYSICS_H

#include <CSE/core.h>

namespace CSE
{
	class Scene;
	class Entity;
	
	class PhysicsProcessor
	{
	public:
		static void CollisionSelection(Entity* A, Entity* B);
		static void CollisionDetection(Entity* A, Entity* B);
		static void CollisionReaction(Entity* A, Entity* B);
		
		static void GeneralRoutine(Scene* scene);
		
	private:
	};
}

#endif
