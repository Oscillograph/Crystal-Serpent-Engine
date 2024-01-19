#ifndef CSE_COMPONENTS_H
#define CSE_COMPONENTS_H

#include <CSE/core.h>
#include <CSE/systems/platform.h>
#include <CSE/systems/renderer/texture.h>

#include <CSE/systems/components/states.h>
#include <CSE/systems/entity.h>

#include <initializer_list>

// special systems
// #include <CSE/vendor/box2d/box2d.h>

namespace CSE
{
	class Window;
	
	struct NameComponent 
	{
		std::string value = "";
		
		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& name);
	};
	
	struct PositionComponent // THIS CORRESPONDS TO HOW IT WORKS IN THE SCENE
	{
		// coordinates corresponding to the scene world
		float x = 0.0f;
		float y = 0.0f;
		int direction = 0; // user defined meaning
		
		PositionComponent() = default;
		PositionComponent(const PositionComponent&) = default;
		PositionComponent(float _x, float _y);
	};
	
	struct TransformComponent // THIS CORRESPONDS TO HOW WE SEE IT
	{
		// coordinates and tranformations corresponding to the viewport
		glm::vec2 position = {0.0f, 0.0f}; // in pixels
		glm::vec2 size = {1.0f, 1.0f}; // in pixels
		
		glm::vec2 positionNormalized = {0.0f, 0.0f};
		glm::vec2 sizeNormalized = {1.0f, 1.0f};
		
		float rotation = 0.0f; // counter-clockwise, radians
		
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		
		void NormalizeToWindow(Window* window);
		glm::vec2 GetPositionNormalizedToWindow(Window* window);
		glm::vec2 GetSizeNormalizedToWindow(Window* window);
	};
	
	struct UUIDComponent 
	{
		uint64_t value = 0;
		
		UUIDComponent();
		UUIDComponent(const UUIDComponent&) = default;
		UUIDComponent(const uint64_t& uuid);
	};
	
	struct SpriteComponent
	{
		Texture* texture = nullptr;
		glm::vec2 clip = {1, 1}; // in pixels
		glm::vec2 tilingFactor = {0.0f, 0.0f};
		// glm::vec2 size = {1.0f, 1.0f}; // meters
		// float rotation = 0.0f; // counter-clockwise, radians
		
		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(Texture* value);
		SpriteComponent(Texture* value, glm::vec2 _clip);
	};
	
	struct KeyBoardComponent {
		// int stands for command from enum
		// using funcVoid = std::function<void()>;
		// using ControlsMap = std::unordered_map<int, SDL_Keycode, funcVoid>;
		using ControlsMap = std::unordered_map<int, SDL_Keycode>;
		ControlsMap controls;
		
		KeyBoardComponent();
		KeyBoardComponent(const KeyBoardComponent&) = default;
		KeyBoardComponent(const ControlsMap& kbControls);
		
		
		void Set(int state, SDL_Keycode code);
		SDL_Keycode Get(int state);
	};
	
	struct MouseComponent 
	{
		// int stands for command from enum
		// using funcVoid = std::function<void()>;
		// using ControlsMap = std::unordered_map<int, SDL_Keycode, funcVoid>;
		using ControlsMap = std::unordered_map<int, SDL_Keycode>;
		ControlsMap controls;
		
		MouseComponent();
		MouseComponent(const MouseComponent&) = default;
		MouseComponent(const ControlsMap& mControls);
		
		void Set(int state, SDL_Keycode code);
		SDL_Keycode Get(int state);
	};
	
	struct StateMachineComponent
	{
		// every group of entities might share the same set of states  
		std::vector<State*> States;
		
		StateMachineComponent();
		StateMachineComponent(const StateMachineComponent&) = default;
		~StateMachineComponent();
		
		State* AddState(int state);
		bool HasState(int state);
		bool SetState(int state); // true if success, false otherwise
		State* GetState();
		
	private:
		State* m_CurrentState = nullptr;
	};
	
	// ================== Animation ===================
	// This struct is taken from SDLCSE v0.1.1
	// Its point is to control what are the frames on a spritesheet and how to play them. 
	struct AnimationFrames
	{
		SDL_Point begin; // upper left corner of the first frame
		SDL_Point end; // upper left corner of the last frame
		int width; // width of a frame
		int height; // height of a frame
		int framesTotal; // equals (end.x - begin.x)/width;
		float framesPerSecond; // basically, equals 1/secondsPerFrame
		float secondsPerFrame; // basically, equals 1/framesPerSecond
		int timeBetweenFrames;
		bool loop;
		
		AnimationFrames();
		AnimationFrames(SDL_Point beginPoint, SDL_Point endPoint, int frameWidth, int frameHeight, float fps, bool loopIt);
	};
	
	// Basically, it's a container for AnimationFrames associated with corresponding states.
	struct AnimationComponent
	{
		std::unordered_map<int, AnimationFrames*> frames; // [ANIM_CODE] = {};
		int currentAnimation;
		int currentFrame;
		uint64_t timeBefore; // last time this component was checked
		int framesTotal;
		bool paused;
		
		AnimationComponent();
		AnimationComponent(const AnimationComponent&) = default;
		AnimationComponent(std::unordered_map<int, AnimationFrames*>& f);
		~AnimationComponent();
		
		void Add(int state, AnimationFrames* animFrames);
		void Set(int state);
		inline int Get() { return currentAnimation; } 
		void Stop();
		void Start();
		void Pause();
		void Unpause();
		void Reset();
		void Change(int state, AnimationFrames* animFrames);
	};
	// ================================================
	
	// ==================  Physics  ===================
	// TODO: tie out from box2d
	namespace PhysicsDefines
	{
		typedef enum
		{
			Dynamic, // full interaction with bodies
			Static, // doesn't react to other bodies, but other bodies react on this one
			Astral, // reacts to other bodies, but other bodies don't react to this one
			Banned // no interaction possible
		} BodyType;
		
		typedef enum
		{
			Rectangle,
			Circle
		} HitBoxType;
	}
	
	struct PhysicsHitBox
	{
		// it's a tree
		std::vector<PhysicsHitBox> children;
		PhysicsHitBox* parent = nullptr;
		
		// characteristics
		int hitBoxType = PhysicsDefines::HitBoxType::Rectangle;
		std::vector<SDL_FPoint> points; // meters
		float radius = 0.0f; // meters, only used in Circle HitBoxes
		
		PhysicsHitBox();
		PhysicsHitBox(const PhysicsHitBox&) = default;
		~PhysicsHitBox();
	};
	
	struct PhysicsComponent
	{
		std::vector<PhysicsHitBox> hitBoxes;
		
		int bodyType = PhysicsDefines::BodyType::Dynamic;
		
		// time management
		int timeInitial = 0; // in milliseconds - should be updated with SDL_GetTicks64()
		float time; // in seconds - should be initiated with timeInitial / 1000.0f and updated with (SDL_GetTicks64() - timeInitial) / 1000.0f
		
		// translational movement
		glm::vec2 velocity = glm::vec2(0.0f); // m/s
		glm::vec2 acceleration = glm::vec2(0.0f); // m/s^2
		
		// characteristics
		float mass = 0.0f; // kg
		
		int world = 0;
		
		// automation stuff
		Entity entity;
		
		/*
		// box2D stuff
		b2BodyDef bodyDefinition; // bodyDefinition.position.Set(x, y)
		// bodyDefinition.type = b2_staticBody || b2_dynamicBody || b2_kinematicBody;
		b2Body* body = nullptr; // created by wolrd.CreateBody(bodyDefinition);
		
		// basically, this is a hitbox
		b2PolygonShape polygonShape; // polygonShape.SetAsBox(width, height)
		// body->CreateFixture(&polygonShape, 0.0f) // static bodies have 0 mass
		*/
		
		PhysicsComponent();
		PhysicsComponent(const Entity& A); // to automatically register
		PhysicsComponent(const PhysicsComponent&) = default;
		void ChangeType(PhysicsDefines::BodyType newType);
		~PhysicsComponent();
	};
	// ================================================
}

#endif
