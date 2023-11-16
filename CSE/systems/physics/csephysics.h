#ifndef CSE_PHYSICS_CSEPHYSICS_H
#define CSE_PHYSICS_CSEPHYSICS_H

#include <CSE/core.h>
#include <CSE/systems/physics.h>

namespace CSE
{
	
	// native physics system
	
	class CSEPhysics : public PhysicsAPI 
	{
	public:
		CSEPhysics();
		~CSEPhysics();
		
		virtual void CreateWorld(const WorldProperties& props) override;
		virtual void DestroyWorld(int worldID) override;
		
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
		// CSEWorld* m_Box2DWorld = nullptr; // according to the tutorial, Box2D world must be in scope as it is defined in the stack, we just follow the example here until we feel strong enough to create our own magic
	};
}

#endif
