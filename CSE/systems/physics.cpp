#include <CSE/systems/physics.h>
#include <CSE/systems/scene.h>
#include <CSE/systems/entity.h>
#include <CSE/systems/components.h>

namespace CSE
{
	// World Collection
	WorldCollection::WorldCollection()
	{
	}
	
	WorldCollection::~WorldCollection()
	{
		for (Entity* e : entities)
		{
			if (e != nullptr)
				delete e;
			e = nullptr;
		}
	}
	
	void WorldCollection::AddEntity(Entity& e)
	{
		Entity* entity = new Entity(e.GetID(), e.GetScene());
		entities.push_back(entity);
	}
	
	void WorldCollection::RemoveEntity(Entity* e)
	{
		if (e != nullptr)
		{
			for (auto it = entities.begin(); it != entities.end(); it++)
			{
				if (*it == e)
				{
					delete *it;
					*it = nullptr;
					entities.erase(it);
					break;
				}
			}
		}
	}
	
	// Physics Processor
	std::vector<WorldCollection> PhysicsProcessor::m_Worlds;
	
	int PhysicsProcessor::CreateWorld()
	{
	}
	
	void PhysicsProcessor::DestroyWorld()
	{
	}
	
	WorldCollection* PhysicsProcessor::AccessWorld(int id)
	{
		if (id < m_Worlds.size())
			return &(m_Worlds[id]);
		return nullptr;
	}
	
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
	
	void PhysicsProcessor::Move(Entity* A)
	{
		PhysicsComponent& physics = A->GetComponent<PhysicsComponent>();
		
		// translational movement
		physics.velocity.x += physics.acceleration.x * physics.time;
		physics.velocity.y += physics.acceleration.y * physics.time;
		
		float moveX = physics.velocity.x * physics.time;
		float moveY = physics.velocity.y * physics.time;
		
		// move center of mass if it's that easy
		// += moveX
		// += moveY
		
		// affect hitboxes
	}
	
	void PhysicsProcessor::UnMove(Entity* A)
	{
		PhysicsComponent& physics = A->GetComponent<PhysicsComponent>();
		
		// translational movement
		// physics.velocity.x += physics.acceleration.x * physics.time;
		// physics.velocity.y += physics.acceleration.y * physics.time;
		
		float moveX = physics.velocity.x * physics.time;
		float moveY = physics.velocity.y * physics.time;
		
		// move center of mass if it's that easy
		// -= moveX
		// -= moveY
		
		// affect hitboxes
	}
}
