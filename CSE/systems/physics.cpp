#include <CSE/systems/physics.h>
#include <CSE/systems/scene.h>
#include <CSE/systems/entity.h>
#include <CSE/systems/components.h>

namespace CSE
{
	void PhysicsProcessor::CollisionSelection(Entity* A, Entity* B)
	{
	}
	
	void PhysicsProcessor::CollisionDetection(Entity* A, Entity* B)
	{
	}
	
	void PhysicsProcessor::CollisionReaction(Entity* A, Entity* B)
	{
	}
	
	void PhysicsProcessor::GeneralRoutine(Scene* scene)
	{
		entt::registry& entityRegistry = scene->GetRegistry();
		auto physicsCollection = entityRegistry.view<PhysicsComponent>();
		
		// check for groups of objects that could possibly collide
		
		for (auto e : physicsCollection)
		{
			Entity entity(e, scene);
			PhysicsComponent& physicsComponent = entity.GetComponent<PhysicsComponent>();
			
			// TODO: develop a satisfying collision and pre-collision check algorithm
			
			// is there a possible collision?
			if (physicsComponent.bodyType != PhysicsDefines::BodyType::Banned)
			{
				// if it is possible, then make sure if it happened
			}
			
			/*
			// others react on this one
			if ((physicsComponent.bodyType == PhysicsDefines::BodyType::Static)
				|| (physicsComponent.bodyType == PhysicsDefines::BodyType::Dynamic))
			{
				
			}
			*/
			
			// this one reacts to others
			if ((physicsComponent.bodyType == PhysicsDefines::BodyType::Astral)
				|| (physicsComponent.bodyType == PhysicsDefines::BodyType::Dynamic))
			{
				
			}
		}
	}
}
