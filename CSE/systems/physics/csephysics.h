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
		
		virtual void ChangeType(Entity* A) override;
		// there is no need to register/unregister entities, 
		// because CSE Physics utilizes the same components the rest of CSE uses
		virtual void RegisterEntity(Entity* A) override; // make the entity seen by the physics processor
		virtual void UnregisterEntity(Entity* A) override; // make it invisible again
		
		// collisions
		virtual void CollisionSelection(Entity* A, Entity* B) override;
		virtual void CollisionDetection(Entity* A, Entity* B) override;
		virtual void CollisionReaction(Entity* A, Entity* B) override;
		
		// reactions
		
		// processors
		virtual void GeneralRoutine(Scene* scene, TimeType sceneTime) override;
		virtual void ApplyForce(Entity e, glm::vec2 vector, float amplitude) override;
		virtual void ApplyImpulse(Entity e, glm::vec2 vector, float amplitude) override;
		
		// general
		virtual void Shutdown() override;
		
	private:
		// CSEWorld* m_Box2DWorld = nullptr; // according to the tutorial, Box2D world must be in scope as it is defined in the stack, we just follow the example here until we feel strong enough to create our own magic
		
		float m_TimeStep = 0.01f; // in seconds
		float m_TimeBuffer = 0.0f; // where the unprocessed time is stored
		float m_TimePassedTotal = 0.0f; // how much time has passed since Physics turned on
		float m_TimeAcceleration = 1; // multiply m_TimeStep to slow things down or rush
		// WARNING! Setting acceleration to values bigger than 1 might invoke tunnel effect
		// Use this feature only to slow things down and back to normal
		
		inline float TimeTypeToSeconds(TimeType sceneTime) 
		{
			return (float)sceneTime / 1000; // sceneTime is in milliseconds, return seconds
		}
	};
}

#endif
