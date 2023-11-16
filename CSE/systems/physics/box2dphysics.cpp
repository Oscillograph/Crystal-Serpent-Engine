#include <CSE/systems/physics/box2dphysics.h>

#include <CSE/vendor/box2d/b2_world.h>

namespace CSE
{
	// b2World Box2DPhysics::m_Box2DWorld;
	
	Box2DPhysics::Box2DPhysics()
	{
	}
	
	Box2DPhysics::~Box2DPhysics()
	{
	}
	
	void Box2DPhysics::CreateWorld(const WorldProperties& props)
	{
		b2Vec2 box2DGravity = { props.gravity.x, props.gravity.y };
		m_Box2DWorld = new b2World(box2DGravity);
	}
	
	void Box2DPhysics::DestroyWorld(int worldID) 
	{
		if (m_Box2DWorld != nullptr)
			delete m_Box2DWorld;
		m_Box2DWorld = nullptr;
	}
	
	void Box2DPhysics::RegisterEntity(Entity* A) // make the entity seen by the physics processor
	{
	}
	
	void Box2DPhysics::UnregisterEntity(Entity* A) // make it invisible again
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
