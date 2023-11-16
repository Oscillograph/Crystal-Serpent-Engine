#include <CSE/systems/physics.h>
#include <CSE/systems/scene.h>
#include <CSE/systems/entity.h>
#include <CSE/systems/components.h>

namespace CSE
{
	// Physics Processor
	std::vector<World*> PhysicsProcessor::m_Worlds;
	PhysicsSystem PhysicsProcessor::m_PhysicsSystemSelected = PhysicsSystem::None;
	PhysicsAPI* PhysicsProcessor::m_API = nullptr;
	
	World* PhysicsProcessor::CreateWorld()
	{
		// default world properties
		WorldProperties props = WorldProperties();
		
		// create a new world
		World* world = new World(props);
		m_Worlds.push_back(world);
		
		m_API->CreateWorld(props);
		
		return world;
	}
	
	World* PhysicsProcessor::CreateWorld(const WorldProperties& props)
	{
		// create a new world
		World* world = new World(props);
		m_Worlds.push_back(world);
		
		// b2Vec2 box2DGravity = {props.gravity.x, props.gravity.y};
		// m_Box2DWorld(box2DGravity);
		
		m_API->CreateWorld(props);
		
		return world;
	}
	
	void PhysicsProcessor::DestroyWorld(World* world)
	{
		if (world != nullptr)
		{
			for (auto it = m_Worlds.begin(); it != m_Worlds.end(); it++)
			{
				if (*it == world)
				{
					delete *it;
					m_Worlds.erase(it);
					world = nullptr;
				}
			}
		}
	}
	
	World* PhysicsProcessor::AccessWorld(int id)
	{
		if (id < m_Worlds.size())
			return m_Worlds[id];
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
	
	void PhysicsProcessor::Init()
	{
		m_API = CreatePhysicsProcessor(PhysicsSystem::CSE);
	}
	
	void PhysicsProcessor::Shutdown()
	{
		for (uint32_t i = 0; i < m_Worlds.size(); i++)
		{
			if (m_Worlds[i] != nullptr)
			{
				// delete box2DWorld
				
				// delete CSE physics world
				delete m_Worlds[i];
			}
			m_Worlds[i] = nullptr;
		}
	}
}
