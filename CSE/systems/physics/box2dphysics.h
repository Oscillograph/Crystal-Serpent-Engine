#ifndef CSE_PHYSICS_BOX2DPHYSICS_H
#define CSE_PHYSICS_BOX2DPHYSICS_H

#include <CSE/core.h>
#include <CSE/systems/physics.h>
#include <CSE/systems/entity.h>

#include <CSE/vendor/box2d/box2d.h>

namespace CSE
{
	class Entity;
	
	class Box2DPhysics : public PhysicsAPI 
	{
	public:
		Box2DPhysics();
		~Box2DPhysics();
		
		virtual void CreateWorld(const WorldProperties& props) override;
		virtual void DestroyWorld(int worldID) override;
		
		virtual void RegisterEntity(Entity* A) override; // make the entity seen by the physics processor
		virtual void ChangeType(Entity* A) override;
		virtual void UnregisterEntity(Entity* A) override; // make it invisible again
		
		// collisions
		virtual void CollisionSelection(Entity* A, Entity* B) override;
		virtual void CollisionDetection(Entity* A, Entity* B) override;
		virtual void CollisionReaction(Entity* A, Entity* B) override;
		
		// reactions
		
		// processors
		virtual void GeneralRoutine(Scene* scene, TimeType sceneTime) override;
		
		// general
		virtual void Shutdown() override;
		
	private:
		b2World* m_Box2DWorld = nullptr; // according to the tutorial, Box2D world must be in scope as it is defined in the stack, we just follow the example here until we feel strong enough to create our own magic
		float m_TimeStep = CSE_FPS60;
		int32 m_VelocityIterations = 6;
		int32 m_PositionIterations = 2;
		// std::unordered_map<b2Body*, uint32_t> m_Bodies = {}; // all the bodies to be registered and unregistered
		std::unordered_map<b2Body*, uint32_t> m_Bodies = {}; // all the bodies to be registered and unregistered
		std::unordered_map<uint32_t, b2Body*> m_Entities = {}; // all the bodies to be registered and unregistered
	};
}

#endif
