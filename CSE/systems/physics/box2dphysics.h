#ifndef CSE_PHYSICS_BOX2DPHYSICS_H
#define CSE_PHYSICS_BOX2DPHYSICS_H

#include <CSE/core.h>
#include <CSE/systems/physics.h>

#include <CSE/vendor/box2d/box2d.h>

namespace CSE
{
	class Box2DPhysics : public PhysicsAPI 
	{
	public:
		Box2DPhysics();
		~Box2DPhysics();
		
		virtual void CreateWorld(const WorldProperties& props) override;
		virtual void DestroyWorld(int worldID) override;
		
		virtual void RegisterEntity(Entity* A) override; // make the entity seen by the physics processor
		virtual void UnregisterEntity(Entity* A) override; // make it invisible again
		
		// collisions
		virtual void CollisionSelection(Entity* A, Entity* B) override;
		virtual void CollisionDetection(Entity* A, Entity* B) override;
		virtual void CollisionReaction(Entity* A, Entity* B) override;
		
		// reactions
		
		// processors
		virtual void GeneralRoutine(Scene* scene) override;
		
		// general
		virtual void Shutdown() override;
		
	private:
		b2World* m_Box2DWorld = nullptr; // according to the tutorial, Box2D world must be in scope as it is defined in the stack, we just follow the example here until we feel strong enough to create our own magic
	};
}

#endif
