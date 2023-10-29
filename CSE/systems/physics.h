#ifndef CSE_PHYSICS_H
#define CSE_PHYSICS_H

#include <CSE/core.h>

namespace CSE
{
	class PhysicsProcessor
	{
	public:
		static void CollisionSelection();
		static void CollisionDetection();
		static void CollisionReaction();
		
	private:
	};
}

#endif
