#include <CSE/systems/physics/csephysics.h>

#include <CSE/systems/entity.h>

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
