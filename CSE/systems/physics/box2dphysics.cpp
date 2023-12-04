#include <CSE/systems/components.h>

#include <CSE/systems/physics/box2dphysics.h>

#include <CSE/vendor/box2d/b2_world.h>
#include <CSE/vendor/entt/entt.hpp>

namespace CSE
{
	// b2World Box2DPhysics::m_Box2DWorld;
	
	Box2DPhysics::Box2DPhysics()
	{
		CSE_CORE_LOG("Box2DPhysics Engine initialized");
	}
	
	Box2DPhysics::~Box2DPhysics()
	{
	}
	
	void Box2DPhysics::CreateWorld(const WorldProperties& props)
	{
		b2Vec2 box2DGravity = { props.gravity.x, props.gravity.y };
		m_Box2DWorld = new b2World(box2DGravity);
		// CSE_CORE_LOG("Is the world even created?");
	}
	
	void Box2DPhysics::DestroyWorld(int worldID) 
	{
		if (m_Box2DWorld != nullptr)
			delete m_Box2DWorld;
		m_Box2DWorld = nullptr;
	}
	
	void Box2DPhysics::RegisterEntity(Entity* A) // make the entity seen by the physics processor
	{
		PhysicsComponent& physics = A->GetComponent<PhysicsComponent>();
		PositionComponent& position = A->GetComponent<PositionComponent>();
		TransformComponent& transform = A->GetComponent<TransformComponent>();
		
		// create a body
		b2BodyDef bodyDefinition;
		bodyDefinition.type = (physics.bodyType == PhysicsDefines::BodyType::Static) ? b2_staticBody : b2_dynamicBody;
		
		b2Body* body = m_Box2DWorld->CreateBody(&bodyDefinition);
		CSE_CORE_LOG("Do we even register anyfin?");
		// TODO: implement hitbox children processing
		for (int i = 0; i < physics.hitBoxes.size(); i++)
		{
			CSE_CORE_LOG("Registering hitbox #", i);
			switch (physics.hitBoxes[i].hitBoxType)
			{
				case PhysicsDefines::HitBoxType::Circle:
					{
						// create a fixture shape
						b2CircleShape shape;
						shape.m_p = { position.x + physics.hitBoxes[i].points[0].x, position.y + physics.hitBoxes[i].points[0].y };
						shape.m_radius = physics.hitBoxes[i].radius;
						
						// add the fixture to the body
						b2FixtureDef fixtureDefinition;
						fixtureDefinition.shape = &shape;
						fixtureDefinition.density = 1.0f;
						fixtureDefinition.friction = 0.3f;
						
						body->CreateFixture(&fixtureDefinition);
					}
					break;
				
				case PhysicsDefines::HitBoxType::Rectangle: // TODO: Rectangular shapes
					{
						// create a fixture shape
						b2PolygonShape shape;
						shape.m_count = 4;
						shape.m_vertices[0].Set(position.x + physics.hitBoxes[i].points[0].x, position.y + physics.hitBoxes[i].points[0].y);
						shape.m_vertices[1].Set(position.x + physics.hitBoxes[i].points[1].x, position.y + physics.hitBoxes[i].points[1].y);
						shape.m_vertices[2].Set(position.x + physics.hitBoxes[i].points[2].x, position.y + physics.hitBoxes[i].points[2].y);
						shape.m_vertices[3].Set(position.x + physics.hitBoxes[i].points[3].x, position.y + physics.hitBoxes[i].points[3].y);
						
						// add the fixture to the body
						b2FixtureDef fixtureDefinition;
						fixtureDefinition.shape = &shape;
						fixtureDefinition.density = 1.0f;
						fixtureDefinition.friction = 0.3f;
						
						body->CreateFixture(&fixtureDefinition);
					}
					break;
			}
		}
		
		// b2World world;
		// world.Step();

		/*
		  // basically, this is a hitbox
		  b2PolygonShape polygonShape; // polygonShape.SetAsBox(width, height)
		  // body->CreateFixture(&polygonShape, 0.0f) // static bodies have 0 mass
		*/
		// Entity temp = { A->GetID(), A->GetScene() };
		m_Bodies.emplace(body, (uint32_t)(A->GetID()));
		m_Entities.emplace((uint32_t)(A->GetID()), body);
	}
	
	void Box2DPhysics::ChangeType(Entity* A)
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
					m_Entities[(uint32_t)(A->GetID())]->SetEnabled(false);
				}
				break;
			case PhysicsDefines::BodyType::Static:
				{
					m_Entities[(uint32_t)(A->GetID())]->SetEnabled(true);
					m_Entities[(uint32_t)(A->GetID())]->SetType(b2BodyType::b2_staticBody);
				}
				break;
			case PhysicsDefines::BodyType::Astral: // Box2D doesn't support this, so it is an alias for kinematic here
				{
					m_Entities[(uint32_t)(A->GetID())]->SetEnabled(true);
					m_Entities[(uint32_t)(A->GetID())]->SetType(b2BodyType::b2_kinematicBody);
				}
				break;
			case PhysicsDefines::BodyType::Dynamic:
				{
					m_Entities[(uint32_t)(A->GetID())]->SetEnabled(true);
					m_Entities[(uint32_t)(A->GetID())]->SetType(b2BodyType::b2_dynamicBody);
				}
				break;
		}
	}
	
	void Box2DPhysics::UnregisterEntity(Entity* A) // make it invisible again
	{
		for (auto bodyIterator = m_Bodies.begin(); bodyIterator != m_Bodies.end(); bodyIterator++)
		{
			if ((*bodyIterator).second == (uint32_t)(A->GetID()))
			{
				auto entityIterator = m_Entities.find((*bodyIterator).second);
				m_Entities.erase(entityIterator);
				
				m_Box2DWorld->DestroyBody((*bodyIterator).first);
				m_Bodies.erase(bodyIterator);
				break;
			}
		}
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
	void Box2DPhysics::GeneralRoutine(Scene* scene, TimeType sceneTime) 
	{
		// update world
		m_Box2DWorld->Step(m_TimeStep, m_VelocityIterations, m_PositionIterations);
		// CSE_CORE_LOG("Box2D: Step");
		// update components
		int32 bodyCount = m_Box2DWorld->GetBodyCount();
		b2Body* bodies = m_Box2DWorld->GetBodyList();
		b2Body* currentBody = bodies;
		// CSE_CORE_LOG("Box2D: First body selected");
		Entity e;
		while(currentBody != nullptr)
		{
			// fetch the corresponding entity - we gonna store user data somehow
			e = {(entt::entity)(m_Bodies[currentBody]), scene};
			CSE_CORE_LOG("Box2D: Entity #", (uint32_t)(e.GetID()), " constructed from initializers");
			
			PhysicsComponent &physics = e.GetComponent<PhysicsComponent>();
			PositionComponent& position = e.GetComponent<PositionComponent>();
			TransformComponent& transform = e.GetComponent<TransformComponent>();
			
			b2Vec2 bodyPosition = currentBody->GetPosition();
			position.x = bodyPosition.x;
			position.y = bodyPosition.y;
			
			b2Vec2 bodyVelocity = currentBody->GetLinearVelocity();
			physics.velocity = { bodyVelocity.x, bodyVelocity.y };
			currentBody->GetNext();
		}
		// CSE_CORE_LOG("Box2D: All bodies processed");
	}
	
	// general
	void Box2DPhysics::Shutdown()
	{
	}
}
