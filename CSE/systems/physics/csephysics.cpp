#include <CSE/systems/physics/csephysics.h>

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
	void CSEPhysics::GeneralRoutine(Scene* scene) 
	{
	}
	
	// general
	void CSEPhysics::Shutdown()
	{
	}
}
