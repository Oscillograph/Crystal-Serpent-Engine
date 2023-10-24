#ifndef CSE_COMPONENTS_H
#define CSE_COMPONENTS_H

#include <CSE/core.h>
#include <CSE/systems/platform.h>
#include <CSE/systems/renderer/texture.h>

#include <initializer_list>

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
	
	struct PositionComponent
	{
		// normalized coordinates corresponding to the scene world
		float x = 0.0f;
		float y = 0.0f;
		
		PositionComponent() = default;
		PositionComponent(const PositionComponent&) = default;
		PositionComponent(float _x, float _y);
		
		SDL_FPoint NormalizedToWindow(Window* window);
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
		
		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(Texture* value);
	};
	
	struct KeyBoardComponent {
		// int stands for command from enum
		using ControlsMap = std::unordered_map<int, SDL_Keycode>;
		ControlsMap controls;
		
		KeyBoardComponent();
		KeyBoardComponent(const KeyBoardComponent&) = default;
		KeyBoardComponent(const std::unordered_map<int, SDL_Keycode>& kbControls);
		
		
		void Set(int state, SDL_Keycode code);
		SDL_Keycode Get(int state);
	};
	
	struct MouseComponent {
		// int stands for command from enum
		std::unordered_map<int, SDL_Keycode> controls;
		
		MouseComponent();
		MouseComponent(const MouseComponent&) = default;
		MouseComponent(const std::unordered_map<int, SDL_Keycode>& mControls);
		
		void Set(int state, SDL_Keycode code);
		SDL_Keycode Get(int state);
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
		void Stop();
		void Start();
		void Pause();
		void Unpause();
		void Reset();
		void Change(int state, AnimationFrames* animFrames);
	};
	// ================================================
}

#endif
