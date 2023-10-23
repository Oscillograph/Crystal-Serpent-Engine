#ifndef CSE_COMPONENTS_H
#define CSE_COMPONENTS_H

#include <CSE/core.h>
#include <CSE/systems/platform.h>
#include <CSE/systems/renderer/texture.h>

namespace CSE
{	
	struct NameComponent 
	{
		std::string value = "";
		
		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& name)
		: value(name) {};
	};
	
	struct UUIDComponent 
	{
		uint64_t value = 0;
		
		UUIDComponent() 
		: value(Utils::UUID::Generate()) 
		{};
		UUIDComponent(const UUIDComponent&) = default;
		UUIDComponent(const uint64_t& uuid)
		: value(uuid) 
		{};
	};
	
	struct SpriteComponent
	{
		Texture* texture;
		
		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(Texture* value)
		: texture(value) {};
	};
	
	struct KeyBoardComponent {
		// int stands for command from enum
		std::unordered_map<int, SDL_Keycode> controls;
		
		KeyBoardComponent()
		{
			for (int i = Commands::KBCommand_BEGIN+1; i < Commands::KBCommand_TOTAL; i++)
			{
				controls[i] = SDLK_UNKNOWN;
			}
		}
		KeyBoardComponent(const KeyBoardComponent&) = default;
		KeyBoardComponent(const std::unordered_map<int, SDL_Keycode>& kbControls)
		: controls(kbControls) {};
		
		
		void Set(int state, SDL_Keycode code)
		{
			controls[state] = code;
		}
		
		SDL_Keycode Get(int state)
		{
			return controls[state];
		}
	};
	
	struct MouseComponent {
		// int stands for command from enum
		std::map<int, SDL_Keycode> controls;
		
		MouseComponent()
		{
			for (int i = Commands::KBCommand_BEGIN+1; i < Commands::KBCommand_TOTAL; i++)
			{
				controls[i] = SDLK_UNKNOWN;
			}
		}
		MouseComponent(const MouseComponent&) = default;
		MouseComponent(const std::map<int, SDL_Keycode>& mControls)
		: controls(mControls) {};
		
		void Set(int state, SDL_Keycode code)
		{
			controls[state] = code;
		}
		
		SDL_Keycode Get(int state)
		{
			return controls[state];
		}
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
		
		AnimationFrames()
		{
			begin = {0, 0};
			end = {0, 0};
			width = 0;
			height = 0;
			framesTotal = 0;
			framesPerSecond = 1.0f;
			secondsPerFrame = 1.0f;
			timeBetweenFrames = 1000;
			loop = false;
		}
		
		AnimationFrames(SDL_Point beginPoint, SDL_Point endPoint, int frameWidth, int frameHeight, float fps, bool loopIt)
		{
			begin = beginPoint;
			end = endPoint;
			width = frameWidth;
			height = frameHeight;
			framesTotal = (int)round(std::abs((endPoint.x - beginPoint.x) / frameWidth));
			
			if (framesTotal == 0)
			{
				framesTotal = 1;
			}
			framesPerSecond = fps;
			secondsPerFrame = 1/fps;
			timeBetweenFrames = (int)round(1000/fps);
			loop = loopIt;
		}
	};
	
	// Basically, it's a container for AnimationFrames associated with corresponding states.
	struct AnimationComponent
	{
		std::map<int, AnimationFrames*> frames; // [ANIM_CODE] = {};
		int currentAnimation;
		int currentFrame;
		uint64_t timeBefore; // last time this component was checked
		int framesTotal;
		bool paused;
		
		AnimationComponent()
		{
			paused = true;
			currentAnimation = EntityStates::IDLE;
			currentFrame = 0;
			timeBefore = Platform::GetTimeMs();
			framesTotal = 0;
			
			for (int i = EntityStates::BEGIN+1; i < EntityStates::TOTAL; i++)
			{
				frames[i] = nullptr;
			}
		}
		
		AnimationComponent(const AnimationComponent&) = default;
		AnimationComponent(std::map<int, AnimationFrames*>& f)
		: frames(f) 
		{
			paused = true;
			currentAnimation = EntityStates::IDLE;
			currentFrame = 0;
			timeBefore = SDL_GetTicks64();
			framesTotal = 0;
		};
		~AnimationComponent()
		{
			for (int i = EntityStates::BEGIN+1; i < EntityStates::TOTAL; i++)
			{
				if (frames[i] != nullptr)
				{
					delete frames[i];
					frames[i] = nullptr;
				}
			}
		}
		
		void Add(int state, AnimationFrames* animFrames)
		{
			frames[state] = animFrames;
		}
		
		void Set(int state)
		{
			currentAnimation = state;
			framesTotal = frames[currentAnimation]->framesTotal;
			currentFrame = 0;
		}
		
		void Stop()
		{
			paused = true;
			currentFrame = 0;
		}
		
		void Start()
		{
			paused = false;
			currentFrame = 0;
		}
		
		void Pause()
		{
			paused = true;
		}
		
		void Unpause()
		{
			paused = false;
		}
		
		void Reset()
		{
			currentFrame = 0;
		}
		
		void Change(int state, AnimationFrames* animFrames)
		{
			if (frames[state] != NULL)
			{
				delete frames[state];
			}
			Add(state, animFrames);
		}
	};
	// ================================================
}

#endif
