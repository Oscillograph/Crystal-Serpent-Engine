#include <CSE/systems/components.h>

#include <CSE/systems/physics/box2dphysics.h>

#include <CSE/systems/window.h>
#include <CSE/systems/layer.h>
#include <CSE/systems/scene.h>
#include <CSE/systems/viewport.h>
#include <CSE/systems/renderer/camera2d.h>
#include <CSE/systems/entity.h>

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
		m_WorldProperties = props;
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
		// PositionComponent& position = A->GetComponent<PositionComponent>();
		TransformComponent& transform = A->GetComponent<TransformComponent>();
		
		// create a body
		b2BodyDef bodyDefinition;
		bodyDefinition.type = (physics.bodyType == PhysicsDefines::BodyType::Static) ? b2_staticBody : b2_dynamicBody;
		bodyDefinition.position.Set(
			physics.position.x + m_WorldProperties.size.x/2, 
			physics.position.y + m_WorldProperties.size.y/2
			);
		bodyDefinition.linearVelocity.Set(0, 0);
		bodyDefinition.allowSleep = false;
		bodyDefinition.awake = true;
		bodyDefinition.fixedRotation = true;
		
		b2Body* body = m_Box2DWorld->CreateBody(&bodyDefinition);
		CSE_CORE_LOG("Do we even register anyfin?");
		// TODO: implement hitbox children processing
		for (int i = 0; i < physics.hitBoxes.size(); i++)
		{
			CSE_CORE_LOG("Entity \"", A->GetComponent<NameComponent>().value, "\": Registering hitbox #", i);
			switch (physics.hitBoxes[i].hitBoxType)
			{
				case PhysicsDefines::HitBoxType::Circle:
					{
						// create a fixture shape
						b2CircleShape shape;
						shape.m_p = { 
							bodyDefinition.position.x + physics.hitBoxes[i].points[0].x, 
							bodyDefinition.position.y + physics.hitBoxes[i].points[0].y 
						};
						shape.m_radius = physics.hitBoxes[i].radius;
						
						// add the fixture to the body
						b2FixtureDef fixtureDefinition;
						fixtureDefinition.shape = &shape;
						fixtureDefinition.density = 1.0f;
						fixtureDefinition.friction = physics.friction;
						
						body->CreateFixture(&fixtureDefinition);
					}
					break;
				
				case PhysicsDefines::HitBoxType::Rectangle: // TODO: Rectangular shapes
					{
						// create a fixture shape
						b2Vec2 vertices[4];
						vertices[0].Set(bodyDefinition.position.x + physics.hitBoxes[i].points[3].x, bodyDefinition.position.y + physics.hitBoxes[i].points[3].y);
						vertices[1].Set(bodyDefinition.position.x + physics.hitBoxes[i].points[2].x, bodyDefinition.position.y + physics.hitBoxes[i].points[2].y);
						vertices[2].Set(bodyDefinition.position.x + physics.hitBoxes[i].points[1].x, bodyDefinition.position.y + physics.hitBoxes[i].points[1].y);
						vertices[3].Set(bodyDefinition.position.x + physics.hitBoxes[i].points[0].x, bodyDefinition.position.y + physics.hitBoxes[i].points[0].y);
						
						b2PolygonShape shape;
						shape.Set(vertices, 4);
						
						// add the fixture to the body
						b2FixtureDef fixtureDefinition;
						fixtureDefinition.shape = &shape;
						fixtureDefinition.density = 1.0f;
						fixtureDefinition.friction = physics.friction;
						
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
		// collect data from engine
		// CSE_CORE_LOG("Box2D: General routine - collect data from components");
		Entity e;
		b2Vec2 velocityNew;
		b2Vec2 velocityOld;
		for (b2Body* currentBody = m_Box2DWorld->GetBodyList(); currentBody; currentBody = currentBody->GetNext())
		{
			e = {(entt::entity)(m_Bodies[currentBody]), scene};
			// CSE_CORE_LOG("Box2D: Entity \"", e.GetComponent<NameComponent>().value, "\"");
			
			PhysicsComponent &physics = e.GetComponent<PhysicsComponent>();
			velocityNew.x = physics.velocity.x;
			velocityNew.y = physics.velocity.y;
			
			// CSE_CORE_LOG("Box2D: collected velocity: (", velocityNew.x, "; ", velocityNew.y, ")");
			velocityOld = currentBody->GetLinearVelocity();
			if ((velocityOld.x != velocityNew.x) && (velocityOld.y != velocityNew.y))
				currentBody->SetLinearVelocity(velocityNew);
		}
		
		// update world
		m_Box2DWorld->Step(m_TimeStep, m_VelocityIterations, m_PositionIterations);
		// CSE_CORE_LOG("Box2D: General routine - Step");
		
		// update components
		// CSE_CORE_LOG("Box2D: General routine - update components data");
		glm::vec4 cameraArea = scene->GetActiveCamera()->GetTargetArea();
		// CSE_CORE_LOG("Box2D: camera at: (", cameraArea.x, "; ", cameraArea.y, "; ", cameraArea.z, "; ", cameraArea.w, ")");
		for (b2Body* currentBody = m_Box2DWorld->GetBodyList(); currentBody; currentBody = currentBody->GetNext())
		{
			// fetch the corresponding entity - we gonna store user data somehow
			e = {(entt::entity)(m_Bodies[currentBody]), scene};
			// CSE_CORE_LOG("Box2D: Entity \"", e.GetComponent<NameComponent>().value, "\"");
			
			PhysicsComponent &physics = e.GetComponent<PhysicsComponent>();
			TransformComponent& transform = e.GetComponent<TransformComponent>();
			
			b2Vec2 bodyPosition = currentBody->GetPosition();
			physics.position.x = bodyPosition.x - m_WorldProperties.size.x/2;
			physics.position.y = bodyPosition.y - m_WorldProperties.size.y/2;
			// CSE_CORE_LOG("Box2D: position: (", physics.position.x, "; ", physics.position.y, ")");
			
			b2Vec2 bodyVelocity = currentBody->GetLinearVelocity();
			physics.velocity.x = bodyVelocity.x;
			physics.velocity.y = bodyVelocity.y;
			// CSE_CORE_LOG("Box2D: velocity: (", physics.velocity.x, "; ", physics.velocity.y, ")");
			
			glm::vec2 windowSize = {
				scene->GetLayer()->GetWindow()->GetPrefs().width,
				scene->GetLayer()->GetWindow()->GetPrefs().height
			};
			transform.position = {
				windowSize.x * ((physics.position.x / (cameraArea.z - cameraArea.x))),
				windowSize.y * ((physics.position.y / (cameraArea.w - cameraArea.y))),
			};
			transform.Normalize({
				windowSize.x,
				windowSize.y
			});
			// CSE_CORE_LOG("Box2D: transform position: (", transform.position.x, "; ", transform.position.y, ")");
			// CSE_CORE_LOG("Box2D: transform position normalized: (", transform.positionNormalized.x, "; ", transform.positionNormalized.y, ")");
			
		}

		// CSE_CORE_LOG("Box2D: All bodies processed");
	}
	
	void Box2DPhysics::ApplyForce(Entity e, glm::vec2 vector, float amplitude)
	{
		b2Body* body = m_Entities[(uint32_t)(e.GetID())];
		b2Vec2 force = {
			amplitude * vector.x,
			amplitude * vector.y
		};
		body->ApplyForceToCenter(force, true);
	}
	
	void Box2DPhysics::ApplyImpulse(Entity e, glm::vec2 vector, float amplitude)
	{
		b2Body* body = m_Entities[(uint32_t)(e.GetID())];
		b2Vec2 impulse = {
			amplitude * vector.x,
			amplitude * vector.y
		};
		body->ApplyLinearImpulseToCenter(impulse, true);
	}
	
	// general
	void Box2DPhysics::Shutdown()
	{
		for (b2Body* body = m_Box2DWorld->GetBodyList(); body; body = body->GetNext())
		{
			m_Box2DWorld->DestroyBody(body);
		}
		DestroyWorld(0);
	}
}
