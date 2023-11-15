#include <CSE/systems/physics/box2dphysics.h>

namespace CSE
{
	void Box2DPhysics::CreateWorld(const WorldProperties& props)
	{
		b2Vec2 box2DGravity = { props.gravity.x, props.gravity.y };
		m_Box2DWorld(box2DGravity);
	}
	
	void Box2DPhysics::DestroyWorld(World* world) 
	{
	}
	
	// collisions
	void Box2DPhysics::CollisionSelection(Entity* A, Entity* B) 
	{
	}
	
	void Box2DPhysics::CollisionDetection(Entity* A, Entity* B) 
	{
	}
	
	void Box2DPhysics::CollisionReaction(Entity* A, Entity* B) 
	{
	}
	
	// reactions
	
	// processors
	void Box2DPhysics::GeneralRoutine(Scene* scene) 
	{
	}
	
	// general
	void Box2DPhysics::Shutdown()
	{
	}
}
