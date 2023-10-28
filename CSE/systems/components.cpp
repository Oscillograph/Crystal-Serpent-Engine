#include <CSE/systems/components.h>
#include <CSE/systems/window.h>

namespace CSE
{
	// Name Component
	NameComponent::NameComponent(const std::string& name)
		: value(name) 
	{
	};
	
	// UUID Component
	UUIDComponent::UUIDComponent() 
		: value(Utils::UUID::Generate()) 
	{
	};
	
	UUIDComponent::UUIDComponent(const uint64_t& uuid)
		: value(uuid) 
	{
	};
	
	// Position Component
	PositionComponent::PositionComponent(float _x, float _y)
		: x(_x), y(_y) 
	{
	}
	
	SDL_FPoint PositionComponent::NormalizedToWindow(Window* window)
	{
		SDL_FPoint newXY;
		newXY.x = window->GetPrefs().width * x;
		newXY.y = window->GetPrefs().height * y;
		
		return newXY;
	}
	
	// Sprite Component
	SpriteComponent::SpriteComponent(Texture* value)
		: texture(value) 
	{
	};
	
	// State Machine Component
	StateMachineComponent::StateMachineComponent()
	{
	}
	
	StateMachineComponent::~StateMachineComponent()
	{
		for (int i = 0; i < States.size(); i++)
		{
			if (States[i] != nullptr)
				delete States[i];
			States[i] = nullptr;
		}
		m_CurrentState = nullptr;
	}
	
	void StateMachineComponent::AddState(State* state)
	{
		auto it = std::find(States.begin(), States.end(), state);
		if (it != States.end())
		{
			States.push_back(state);
		} else {
			CSE_CORE_LOG("Can't add the state: already exists");
		}
	}
	
	void StateMachineComponent::SetState(State* state)
	{
		if (m_CurrentState->IsAllowedExitTo(state))
		{
			if (state->IsAllowedEntryFrom(m_CurrentState))
			{
				m_CurrentState->OnExit();
				m_CurrentState = state;
				m_CurrentState->OnEnter();
			} else {
				CSE_CORE_LOG("The state cannot be entered from this one.");
			}
		} else {
			CSE_CORE_LOG("Exit to the scene is not allowed from this one");
		}
	}
	
	State* StateMachineComponent::GetState()
	{
		return m_CurrentState;
	}
	
	// Animation Component - Animation Frames
	AnimationFrames::AnimationFrames()
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
	
	AnimationFrames::AnimationFrames(SDL_Point beginPoint, SDL_Point endPoint, int frameWidth, int frameHeight, float fps, bool loopIt)
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
	
	// Animation Component
	AnimationComponent::AnimationComponent()
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
	
	AnimationComponent::AnimationComponent(std::unordered_map<int, AnimationFrames*>& f)
		: frames(f) 
	{
		paused = true;
		currentAnimation = EntityStates::IDLE;
		currentFrame = 0;
		timeBefore = SDL_GetTicks64();
		framesTotal = 0;
	};
	
	AnimationComponent::~AnimationComponent()
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
	
	void AnimationComponent::Add(int state, AnimationFrames* animFrames)
	{
		frames[state] = animFrames;
	}
	
	void AnimationComponent::Set(int state)
	{
		CSE_CORE_ASSERT(frames[state] != nullptr, "Trying to set an animation that doesn't exist!");
		currentAnimation = state;
		framesTotal = frames[currentAnimation]->framesTotal;
		currentFrame = 0;
	}
	
	void AnimationComponent::Stop()
	{
		paused = true;
		currentFrame = 0;
	}
	
	void AnimationComponent::Start()
	{
		paused = false;
		currentFrame = 0;
	}
	
	void AnimationComponent::Pause()
	{
		paused = true;
	}
	
	void AnimationComponent::Unpause()
	{
		paused = false;
	}
	
	void AnimationComponent::Reset()
	{
		currentFrame = 0;
	}
	
	void AnimationComponent::Change(int state, AnimationFrames* animFrames)
	{
		if (frames[state] != NULL)
		{
			delete frames[state];
		}
		Add(state, animFrames);
	}
	
	// KeyBoard Component
	KeyBoardComponent::KeyBoardComponent()
	{
		for (int i = Commands::KBCommand_BEGIN+1; i < Commands::KBCommand_TOTAL; i++)
		{
			controls[i] = SDLK_UNKNOWN;
		}
	}
	
	KeyBoardComponent::KeyBoardComponent(const ControlsMap& kbControls)
		: controls(kbControls) 
	{
	};
	
	void KeyBoardComponent::Set(int state, SDL_Keycode code)
	{
		controls[state] = code;
	}
	
	SDL_Keycode KeyBoardComponent::Get(int state)
	{
		return controls[state];
	}
	
	// Mouse Component
	MouseComponent::MouseComponent()
	{
		for (int i = Commands::KBCommand_BEGIN+1; i < Commands::KBCommand_TOTAL; i++)
		{
			controls[i] = SDLK_UNKNOWN;
		}
	}
	
	MouseComponent::MouseComponent(const ControlsMap& mControls)
		: controls(mControls) 
	{
	};
	
	void MouseComponent::Set(int state, SDL_Keycode code)
	{
		controls[state] = code;
	}
	
	SDL_Keycode MouseComponent::Get(int state)
	{
		return controls[state];
	}
}


