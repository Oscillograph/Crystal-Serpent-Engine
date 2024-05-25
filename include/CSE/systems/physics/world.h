#ifndef CSE_PHYSICS_WORLD_H
#define CSE_PHYSICS_WORLD_H

#include <CSE/core.h>

namespace CSE
{
	// forward declarations
	class Entity;
	// --------------------
	
	struct WorldProperties
	{
		glm::vec2 gravity = {0.0f, 9.8f}; // up or down directions
		glm::vec2 size = {1000.0f, 1000.0f}; // in meters
		
		WorldProperties();
		WorldProperties(const vec2& worldSize);
	};
	
	struct World
	{
		int id;
		WorldProperties properties;
		std::vector<Entity*> entities;
		
		World();
		World(const WorldProperties& props);
		~World();
		
		void AddEntity(Entity& e);
		void RemoveEntity(Entity* e);
	};
}

#endif
