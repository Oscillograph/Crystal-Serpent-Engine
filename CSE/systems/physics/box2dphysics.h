#ifndef CSE_PHYSICS_BOX2DPHYSICS_H
#define CSE_PHYSICS_BOX2DPHYSICS_H

#include <CSE/core.h>
#include <CSE/systems/physics.h>

#include <CSE/vendor/box2d/include/box2d.h>

namespace CSE
{
	class Box2DPhysics : public PhysicsAPI 
	{
	public:
		virtual static void CreateWorld(const WorldProperties& props) override;
		virtual static void DestroyWorld(World* world) override;
		
		// collisions
		virtual static void CollisionSelection(Entity* A, Entity* B) override;
		virtual static void CollisionDetection(Entity* A, Entity* B) override;
		virtual static void CollisionReaction(Entity* A, Entity* B) override;
		
		// reactions
		
		// processors
		virtual static void GeneralRoutine(Scene* scene) override;
		
		// general
		virtual static void Shutdown() override;
		
	private:
		b2World m_Box2DWorld; // according to the tutorial, Box2D world must be in scope as it is defined in the stack, we just follow the example here until we feel strong enough to create our own magic
	};
	
	PhysicsAPI* CreatePhysicsProcessor()
	{
		return Box2DPhysics();
	}
}

#endif
