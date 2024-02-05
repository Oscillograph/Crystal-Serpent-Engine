#ifndef CSE_PRESETS_UNITS_H
#define CSE_PRESETS_UNITS_H

#include <CSE/core.h>
#include <CSE/systems/platform.h>
#include <CSE/systems/entity.h>

namespace CSE
{
	class Scene;
	class Texture;
	
	typedef std::unordered_map<int, SDL_Keycode> 	KBControls;
	
	namespace Arcade
	{
		// ================== Unit - can be controlled and interacted with
		
		class Unit : public Entity
		{
		public:
			Unit() = default;
			Unit(
				Scene* scene,
				const std::string& name,
				const std::string& spritePath,
				const KBControls& keyboardControls,
				const glm::vec2& transformPosition,
				const glm::vec2& transformSize,
				const glm::vec2& spriteTilingFactor = {0.0f, 0.0f},
				const glm::uvec3& spriteColorKey = {0, 0, 0},
				const glm::vec2& physicalPosition = {100, 100}
				);
			~Unit();
			
			void SetAnimations(
				std::vector<int> animationStates, 
				glm::uvec2 frameSize, 
				float framesPerSecond = 20.0f,
				bool loopAnimations = true
				);
			void ChangeDirection(int direction);
			void ChangePhysics(const glm::vec2& speedMax);
			
		protected:
			Entity* m_Entity = nullptr;
			Texture* m_SpriteTexture = nullptr;
			glm::vec2 m_SpeedMax = {25.0f, 25.0f}; // meters per second
		};
		
		// ================== Doodad - can't be controlled, but can be interacted with
		
		class Doodad : public Entity
		{
		public:
			Doodad() = default;
			Doodad(
				Scene* scene,
				const std::string& name,
				const std::string& spritePath,
				const KBControls& keyboardControls,
				const glm::vec2& transformPosition,
				const glm::vec2& transformSize,
				const glm::vec2& spriteTilingFactor = {0.0f, 0.0f},
				const glm::uvec3& spriteColorKey = {0, 0, 0},
				const glm::vec2& physicalPosition = {100, 100},
				const std::vector<glm::vec2>& hitboxPoints = {{-0.01f, 2.0f}, {0.01f, 2.0f}, {0.01f, 0.0f}, {-0.01f, 0.0f}}
				);
			~Doodad();
			
			void SetAnimations(
				std::vector<int> animationStates, 
				glm::uvec2 frameSize, 
				float framesPerSecond = 20.0f,
				bool loopAnimations = true
				);
			void ChangeDirection(int direction);
			void ChangePhysics(const glm::vec2& speedMax);
			
		protected:
			Entity* m_Entity = nullptr;
			Texture* m_SpriteTexture = nullptr;
			glm::vec2 m_SpeedMax = {25.0f, 25.0f}; // meters per second
		};
		
		// ================== Decoration - can't be interacted with, can't be controlled
		
		class Decoration : public Entity
		{
		public:
			Decoration() = default;
			Decoration(
				Scene* scene,
				const std::string& name,
				const std::string& spritePath, 
				const glm::vec2& place,
				const glm::vec2& size,
				const glm::vec2& tilingFactor = {0.0f, 0.0f},
				const glm::uvec3& colorKey = {0, 0, 0}
				);
			~Decoration();
			
			void ChangeAppearance(
				const std::string& spritePath, 
				const glm::vec2& place,
				const glm::vec2& size,
				const glm::vec2& tilingFactor
				);
			
		protected:
			Entity* m_Entity = nullptr;
			Texture* m_SpriteTexture = nullptr;
		};
	}
}

#endif
