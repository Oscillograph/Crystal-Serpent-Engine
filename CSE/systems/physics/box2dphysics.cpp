#include <CSE/systems/components.h>

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
		PhysicsComponent& physics = A->GetComponent<PhysicsComponent>();
		PositionComponent& position = A->GetComponent<PositionComponent>();
		TransformComponent& transform = A->GetComponent<TransformComponent>();
		
		// create a body
		b2BodyDef bodyDefinition;
		bodyDefinition.type = (physics.bodyType == PhysicsDefines::BodyType::Static) ? b2_staticBody : b2_dynamicBody;
		
		b2Body* body = m_Box2DWorld->CreateBody(&bodyDefinition);
		
		// TODO: implement hitbox children processing
		for (int i = 0; i < physics.hitBoxes.size(); i++)
		{
			switch (physics.hitBoxes[i].hitBoxType)
			{
				case PhysicsDefines::HitBoxType::Circle:
					{
						// create a fixture shape
						b2CircleShape shape;
						shape.m_p = { physics.hitBoxes[i].points[0].x, physics.hitBoxes[i].points[0].y };
						shape.m_radius = physics.hitBoxes[i].radius;
						
						// add the fixture to the body
						b2FixtureDef fixtureDefinition;
						fixtureDefinition.shape = &shape;
						fixtureDefinition.density = 1.0f;
						fixtureDefinition.friction = 0.3f;
						
						body->CreateFixture(&fixtureDefinition);
					}
					break;
				
			case PhysicsDefines::HitBoxType::Rectangle:
				{
					// create a fixture shape
					// add the fixture to the body
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
		Entity temp = { A->GetID(), A->GetScene() };
		m_Bodies.emplace(body, temp);
	}
	
	void Box2DPhysics::UnregisterEntity(Entity* A) // make it invisible again
	{
		for (auto it = m_Bodies.begin(); it != m_Bodies.end(); it++)
		{
			if ((*it).second.GetID() == A->GetID())
			{
				m_Bodies.erase(it);
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
	void Box2DPhysics::GeneralRoutine(Scene* scene) 
	{
		// update world
		m_Box2DWorld->Step(m_TimeStep, m_VelocityIterations, m_PositionIterations);
		
		// update components
		int32 bodyCount = m_Box2DWorld->GetBodyCount();
		b2Body* bodies = m_Box2DWorld->GetBodyList();
		b2Body* currentBody = bodies->GetNext();
		Entity e;
		while(currentBody != nullptr)
		{
			// fetch the corresponding entity - we gonna store user data somehow
			e = m_Bodies[currentBody];
			
			PhysicsComponent &physics = e.GetComponent<PhysicsComponent>();
			PositionComponent& position = e.GetComponent<PositionComponent>();
			TransformComponent& transform = e.GetComponent<TransformComponent>();
			
			b2Vec2 bodyPosition = currentBody->GetPosition();
			position.x = bodyPosition.x;
			position.y = bodyPosition.y;
			
			b2Vec2 bodyVelocity = currentBody->GetLinearVelocity();
			physics.velocity = { bodyVelocity.x, bodyVelocity.y };
			
		}
	}
	
	// general
	void Box2DPhysics::Shutdown()
	{
	}
}
