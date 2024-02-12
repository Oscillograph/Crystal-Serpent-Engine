#include <CSE/systems/physics/csephysics.h>

#include <CSE/systems/entity.h>
#include <CSE/systems/components.h>
#include <CSE/systems/scene.h>

namespace CSE
{
	CSEPhysics::CSEPhysics()
	{
	}
	
	CSEPhysics::~CSEPhysics()
	{
	}
	
	void CSEPhysics::CreateWorld(const WorldProperties& props)
	{
	}
	
	void CSEPhysics::DestroyWorld(int worldID) 
	{
	}
	
	void CSEPhysics::RegisterEntity(Entity* A) // make the entity seen by the physics processor
	{
	}
	
	void CSEPhysics::ChangeType(Entity* A)
	{
		PhysicsComponent& physics = A->GetComponent<PhysicsComponent>();
		
		// reminder:
		// Banned  - no interaction possible
		// Static  - doesn't react to other bodies, but other bodies react on this one
		// Astral  - reacts to other bodies, but other bodies don't react to this one
		// Dynamic - full interaction with bodies
		
		switch (physics.bodyType)
		{
		case PhysicsDefines::BodyType::Banned:
			{
			}
			break;
		case PhysicsDefines::BodyType::Static:
			{
			}
			break;
		case PhysicsDefines::BodyType::Astral: // Box2D doesn't support this, so it is an alias for kinematic here
			{
			}
			break;
		case PhysicsDefines::BodyType::Dynamic:
			{
			}
			break;
		}
	}
	
	void CSEPhysics::UnregisterEntity(Entity* A) // make it invisible again
	{
	}
	
	// collisions
	void CSEPhysics::CollisionSelection(Entity* A, Entity* B) 
	{
	}
	
	void CSEPhysics::CollisionDetection(Entity* A, Entity* B) 
	{
	}
	
	void CSEPhysics::CollisionReaction(Entity* A, Entity* B) 
	{
	}
	
	// reactions
	
	// processors
	void CSEPhysics::GeneralRoutine(Scene* scene, TimeType sceneTime) 
	{
		m_TimeBuffer += TimeTypeToSeconds(sceneTime);
		while (m_TimeBuffer >= m_TimeStep)
		{
			// Calculate objects' physical parameters
			scene->GetRegistry().view<PhysicsComponent>().each([=](auto entity, PhysicsComponent& physics){
				physics.positionBackup = physics.position;
				physics.velocityBackup = physics.velocity;
				physics.accelerationBackup = physics.acceleration;
			});
			
			// Calculate collisions:
			// - CollisionSelection
			// - CollisionDetection
			// - CollisionReaction
			
			// Set back to backups if necessary
			scene->GetRegistry().view<PhysicsComponent>().each([=](auto entity, PhysicsComponent& physics){
				if (physics.restoreBackup)
				{
					physics.position = physics.positionBackup;
					physics.velocity = physics.velocityBackup;
					physics.acceleration = physics.accelerationBackup;
					physics.restoreBackup = false;
				}
			});
			
			m_TimeBuffer -= m_TimeStep * m_TimeAcceleration;
		}
	}
	
	void CSEPhysics::ApplyForce(Entity e, glm::vec2 vector, float amplitude)
	{
	}
	
	void CSEPhysics::ApplyImpulse(Entity e, glm::vec2 vector, float amplitude)
	{
	}
	
	// general
	void CSEPhysics::Shutdown()
	{
	}
}
