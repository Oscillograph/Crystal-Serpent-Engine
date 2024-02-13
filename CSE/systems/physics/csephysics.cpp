#include <CSE/systems/physics/csephysics.h>

#include <CSE/systems/entity.h>
#include <CSE/systems/components.h>
#include <CSE/systems/scene.h>
#include <CSE/systems/renderer/camera2d.h>
#include <CSE/systems/layer.h>
#include <CSE/systems/window.h>
#include <CSE/utils/math.h>

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
		m_World = props;
	}
	
	void CSEPhysics::DestroyWorld(int worldID) 
	{
	}
	
	void CSEPhysics::RegisterEntity(Entity* A) // make the entity seen by the physics processor
	{
		NormalizePhysics(A);
	}
	
	void CSEPhysics::ChangeType(Entity* A)
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
			}
			break;
		case PhysicsDefines::BodyType::Static:
			{
			}
			break;
		case PhysicsDefines::BodyType::Astral: // Box2D doesn't support this, so it is an alias for kinematic here
			{
			}
			break;
		case PhysicsDefines::BodyType::Dynamic:
			{
			}
			break;
		}
	}
	
	void CSEPhysics::UnregisterEntity(Entity* A) // make it invisible again
	{
	}
	
	// collisions
	void CSEPhysics::CollisionSelection(Entity* A, Entity* B) 
	{
		PhysicsComponent& physicsA = A->GetComponent<PhysicsComponent>();
		PhysicsComponent& physicsB = B->GetComponent<PhysicsComponent>();
		
		int i, j, k, l;
		for (i = 0; i < physicsA.hitBoxes.size(); ++i)
		{
			for (j = i; j < physicsB.hitBoxes.size(); ++j)
			{
				/*
				if ((physicsA.hitBoxes[i].hitBoxType == PhysicsDefines::HitBoxType::Rectangle) &&
					(physicsB.hitBoxes[j].hitBoxType == PhysicsDefines::HitBoxType::Rectangle))
				{
					// Check every point method
					auto& pointsA =  physicsA.hitBoxes[i].points;
					auto& pointsB =  physicsB.hitBoxes[j].points;
					for (k = 0; k < 4; k++)
					{
						// rectangles don't intersect when a point coordinate of one of them 
						// either is less than coordinates of all points of the other one,
						// either is bigger than them.
						if (!(
							((pointsA[k].x < pointsB[0].x) && (pointsA[k].x < pointsB[1].x) &&
							 (pointsA[k].x < pointsB[2].x) && (pointsA[k].x < pointsB[3].x)) 
														   ||
							((pointsA[k].x > pointsB[0].x) && (pointsA[k].x > pointsB[1].x) &&
							 (pointsA[k].x > pointsB[2].x) && (pointsA[k].x > pointsB[3].x))
														   ||
							((pointsA[k].y < pointsB[0].y) && (pointsA[k].y < pointsB[1].y) &&
							 (pointsA[k].y < pointsB[2].y) && (pointsA[k].y < pointsB[3].y)) 
														   ||
							((pointsA[k].y > pointsB[0].y) && (pointsA[k].y > pointsB[1].y) &&
							 (pointsA[k].y > pointsB[2].y) && (pointsA[k].y > pointsB[3].y))
							))
						{
							CSE_CORE_LOG(A->GetComponent<NameComponent>().value, " collided with ", B->GetComponent<NameComponent>().value);
							// find out where the collision happened exactly
							CollisionDetection(A, B);
							// react to collision
							CollisionReaction(A, B);
						}
					}
				}
				*/
				
				/*
				if ((physicsA.hitBoxes[i].hitBoxType == PhysicsDefines::HitBoxType::Rectangle) &&
					(physicsB.hitBoxes[j].hitBoxType == PhysicsDefines::HitBoxType::Circle))
				{
					// Check every point method
					auto& pointsA =  physicsA.hitBoxes[i].points; // rectangle
					auto& pointsB =  physicsB.hitBoxes[j].points; // circle
					for (k = 0; k < 4; k++)
					{
						if (!(
							((pointsA[k].x < (pointsB[0].x - physicsB.hitBoxes[j].radius)) || 
							 (pointsA[k].x > (pointsB[0].x + physicsB.hitBoxes[j].radius))) 
														   &&
							((pointsA[k].y < (pointsB[0].y - physicsB.hitBoxes[j].radius)) || 
							 (pointsA[k].y > (pointsB[0].y + physicsB.hitBoxes[j].radius)))
							))
						{
							CSE_CORE_LOG(A->GetComponent<NameComponent>().value, " collided with ", B->GetComponent<NameComponent>().value);
							// find out where the collision happened exactly
							CollisionDetection(A, B);
							// react to collision
							CollisionReaction(A, B);
						}
					}
				}
				*/
				
				if ((physicsA.hitBoxes[i].hitBoxType == PhysicsDefines::HitBoxType::Circle) &&
					(physicsB.hitBoxes[j].hitBoxType == PhysicsDefines::HitBoxType::Circle))
				{
					// Check every point method
					auto& pointsA =  physicsA.hitBoxes[i].points; // circle
					auto& pointsB =  physicsB.hitBoxes[j].points; // circle
					
					math::vec3 distance(
						(physicsB.position.x + pointsB[0].x) - (physicsA.position.x + pointsA[0].x), 
						(physicsB.position.y + pointsB[0].y) - (physicsA.position.y + pointsA[0].y)
						);
					if (distance.SquareMagnitude() < math::Square(physicsA.hitBoxes[i].radius + physicsB.hitBoxes[j].radius))
					{
						CSE_CORE_LOG("Collision selection:\n",
							A->GetComponent<NameComponent>().value, " collided with ", B->GetComponent<NameComponent>().value,
							"\nA xy: ", physicsA.position.x + pointsA[0].x, ", ", physicsA.position.y + pointsA[0].y,
							"\nB xy: ", physicsB.position.x + pointsB[0].x, ", ", physicsB.position.y + pointsB[0].y
							);
						// find out where the collision happened exactly
						CollisionDetection(A, B);
						// react to collision
						CollisionReaction(A, B);
					}
				}
			}
		}
	}
	
	void CSEPhysics::CollisionDetection(Entity* A, Entity* B) 
	{
		PhysicsComponent& physicsA = A->GetComponent<PhysicsComponent>();
		PhysicsComponent& physicsB = B->GetComponent<PhysicsComponent>();
	}
	
	void CSEPhysics::CollisionReaction(Entity* A, Entity* B) 
	{
		PhysicsComponent& physicsA = A->GetComponent<PhysicsComponent>();
		PhysicsComponent& physicsB = B->GetComponent<PhysicsComponent>();
	}
	
	// reactions
	
	// processors
	void CSEPhysics::GeneralRoutine(Scene* scene, TimeType sceneTime) 
	{
		m_LastScene = scene;
		// m_TimeThisFrame = sceneTime;
		// m_TimeBuffer += TimeTypeToSeconds(m_TimeThisFrame - m_TimeLastFrame);
		// m_TimeLastFrame = m_TimeThisFrame;
		m_TimeBuffer += TimeTypeToSeconds(sceneTime);
		m_TimePassedTotal += m_TimeBuffer;
		
		while (m_TimeBuffer >= m_TimeStep)
		{
			// Calculate objects' physical parameters
			scene->GetRegistry().view<PhysicsComponent>().each([=](auto entity, PhysicsComponent& physics){
				physics.positionBackup = physics.position;
				physics.velocityBackup = physics.velocity;
				physics.accelerationBackup = physics.acceleration;
				
				// simple Newton movement mechanic
				physics.position = {
					physics.position.x + physics.velocity.x * m_TimeStep + physics.acceleration.x * m_TimeStep * m_TimeStep * 0.5f,
					physics.position.y + physics.velocity.y * m_TimeStep + physics.acceleration.y * m_TimeStep * m_TimeStep * 0.5f,
					0
				};
				Entity e(entity, scene);
				NormalizePhysics(&e);
			});
			
			// Calculate collisions
			auto physicalEntities = scene->GetRegistry().view<PhysicsComponent>();
			for (auto entityIterator = physicalEntities.begin(); entityIterator != physicalEntities.end(); ++entityIterator)
			{
				if ((entityIterator + 1) != physicalEntities.end())
				{
					Entity A = {*entityIterator, scene};
					Entity B = {*(entityIterator + 1), scene};
					
					CollisionSelection(&A, &B); // rough check if a collision is possible
				}
			}
			
			// Update transforms
			glm::vec4 cameraArea = scene->GetActiveCamera()->GetTargetArea();
			for (auto entityID : scene->GetRegistry().view<PhysicsComponent>())
			{
				Entity entity(entityID, scene);
				PhysicsComponent& physics = entity.GetComponent<PhysicsComponent>();
				TransformComponent& transform = entity.GetComponent<TransformComponent>();
				
				// Set back to backups if necessary
				if (physics.restoreBackup)
				{
					physics.position = physics.positionBackup;
					physics.velocity = physics.velocityBackup;
					physics.acceleration = physics.accelerationBackup;
					physics.restoreBackup = false;
					
					NormalizePhysics(&entity);
				}
				
				/*
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
				*/
			}
			
			m_TimeBuffer -= m_TimeStep * m_TimeAcceleration;
		}
	}
	
	void CSEPhysics::ApplyForce(Entity e, glm::vec2 vector, float amplitude)
	{
	}
	
	void CSEPhysics::ApplyImpulse(Entity e, glm::vec2 vector, float amplitude)
	{
	}
	
	// general
	void CSEPhysics::Shutdown()
	{
		// m_TimeLastFrame = 0;
		// m_TimeThisFrame = 0;
		m_TimeBuffer = 0.0f;
		m_TimePassedTotal = 0.0f;
		
		// Backup physics
		m_LastScene->GetRegistry().view<PhysicsComponent>().each([=](auto entity, PhysicsComponent& physics){
			physics.positionBackup = physics.position;
			physics.velocityBackup = physics.velocity;
			physics.accelerationBackup = physics.acceleration;
			physics.restoreBackup = true;
		});
	}
	
	void CSEPhysics::NormalizePhysics(Entity* A)
	{
		PhysicsComponent& physics = A->GetComponent<PhysicsComponent>();
		math::vec3 normSize = {
			A->GetScene()->GetActiveCamera()->GetTargetArea().z - A->GetScene()->GetActiveCamera()->GetTargetArea().x,
			A->GetScene()->GetActiveCamera()->GetTargetArea().w - A->GetScene()->GetActiveCamera()->GetTargetArea().y,
			0
		};
		// CSE_CORE_LOG("World size: ", m_World.size.x, "; ", m_World.size.y);
		physics.positionNormalized.x = (math::real)physics.position.x / normSize.x;
		physics.positionNormalized.y = (math::real)physics.position.y / normSize.y;
		// physics.positionNormalized.z = physics.position.z / m_World.size.z;
		
		physics.velocityNormalized.x = (math::real)physics.velocity.x / normSize.x;
		physics.velocityNormalized.y = (math::real)physics.velocity.y / normSize.y;
		
		physics.accelerationNormalized.x = (math::real)physics.acceleration.x / normSize.x;
		physics.accelerationNormalized.y = (math::real)physics.acceleration.y / normSize.y;
		
		for (int i = 0; i < physics.hitBoxes.size(); i++)
		{
			// CSE_CORE_LOG("Entity ", A->GetComponent<NameComponent>().value, " - Hitbox #", i);
			for (int j = 0; j < physics.hitBoxes[i].points.size(); j++)
			{
				// CSE_CORE_LOG("Point #", j);
				physics.hitBoxes[i].pointsNormalized[j].x = (math::real)physics.hitBoxes[i].points[j].x / normSize.x;
				physics.hitBoxes[i].pointsNormalized[j].y = (math::real)physics.hitBoxes[i].points[j].y / normSize.y;
			}
			// CSE_CORE_LOG("Points normalized.");
			
			if (physics.hitBoxes[i].hitBoxType == PhysicsDefines::HitBoxType::Circle)
			{
				physics.hitBoxes[i].radiusNormalized = (math::real)physics.hitBoxes[i].radius / normSize.x;
				// CSE_CORE_LOG("Radius normalized.");
			}
		}
	}
}
