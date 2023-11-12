#ifndef CSE_PHYSICS_H
#define CSE_PHYSICS_H

#include <CSE/core.h>

namespace CSE
{
	class Scene;
	class Entity;
	
	// An entity can have a component PhysicsComponent.
	// PhysicsComponent ties the entity to a specific world.
	// Every world is basically a collection with a set of rules applied to all entities in it.
	// We don't need to check scene registry for entities if we have this collection.
	struct WorldCollection
	{
		int id;
		std::vector<Entity*> entities;
		
		WorldCollection();
		~WorldCollection();
		
		void AddEntity(Entity& e);
		void RemoveEntity(Entity* e);
	};
	
	class PhysicsProcessor
	{
	public:
		// world management
		static int CreateWorld();
		static void DestroyWorld();
		static WorldCollection* AccessWorld(int id);
		
		// collisions
		static void CollisionSelection(Entity* A, Entity* B);
		static void CollisionDetection(Entity* A, Entity* B);
		static void CollisionReaction(Entity* A, Entity* B);
		
		// reactions
		
		// processors
		
		static void GeneralRoutine(Scene* scene);
		
	private:
		static std::vector<WorldCollection> m_Worlds;
		
		static void Move(Entity* A);
		static void UnMove(Entity* A);
	};
}

#endif
