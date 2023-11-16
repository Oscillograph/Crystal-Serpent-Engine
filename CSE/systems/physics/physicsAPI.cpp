#include <CSE/systems/physics/physicsAPI.h>

#include <CSE/systems/physics/csephysics.h>
#include <CSE/systems/physics/box2dphysics.h>

namespace CSE
{
	PhysicsAPI* CreatePhysicsProcessor(PhysicsSystem psID)
	{	
		switch (psID)
		{
			case PhysicsSystem::CSE : 
				return new CSEPhysics(); 
				break;
			case PhysicsSystem::Box2D : 
				return new Box2DPhysics(); 
				break;
			default:
				CSE_CORE_LOG("Couldn't guess the physics system to initialize");
		}
		return nullptr;
	}
}
