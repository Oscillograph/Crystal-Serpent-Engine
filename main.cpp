#include "./cse.h"

// Scenes
class SceneLogo : public CSE::Scene
{
public:
	SceneLogo()
		: CSE::Scene()
	{
	};
	
	~SceneLogo()
	{
		if (sprite != nullptr)
			delete sprite;
		sprite = nullptr;
		if (logo != nullptr)
			delete logo;
		logo = nullptr;
	}
	
	void Init()
	{
		// TODO: figure out how to de-initialize a scene for a proper scene unload mechanism
		if (logo == nullptr)
		{
			logo = CreateEntity("CSE Logotype");
			logo->AddComponent<CSE::PositionComponent>(0.5f, 0.5f);
			sprite = new CSE::Texture("./CSE/assets/CSE_logo.png", GetLayer()->GetWindow()->GetRenderer());
			logo->AddComponent<CSE::SpriteComponent>(sprite);
		}
	}
	
private:
	CSE::Texture* sprite = nullptr;
	CSE::Entity* logo = nullptr;
};

class SceneGame : public CSE::Scene
{
public:
	SceneGame()
		: CSE::Scene()
	{
	};
	
	~SceneGame()
	{
		if (sprite != nullptr)
			delete sprite;
		sprite = nullptr;
		if (ball != nullptr)
			delete ball;
		ball = nullptr;
	}
	
	void Init()
	{
		if (ball == nullptr)
		{
			// a ball entity on the screen
			ball = CreateEntity("Ball");
			CSE::PositionComponent& position = ball->AddComponent<CSE::PositionComponent>(0.5f, 0.5f);
			position.direction = 1; // 1 means right, -1 means left
			CSE::StateMachineComponent& stateMachine = ball->AddComponent<CSE::StateMachineComponent>();
			
			std::unordered_map<int, SDL_Keycode> ballKBControls = {
				{CSE::Commands::KBCommand_Left, CSE::ScanCode::Left}, 
				{CSE::Commands::KBCommand_Right, CSE::ScanCode::Right}
			};
			
			CSE::KeyBoardComponent& keyboard = ball->AddComponent<CSE::KeyBoardComponent>(ballKBControls);
			
			sprite = new CSE::Texture("./App/Sprites.png", GetLayer()->GetWindow()->GetRenderer(), {0, 0, 0});
			CSE::SpriteComponent& spriteComponent = ball->AddComponent<CSE::SpriteComponent>(sprite);
			CSE::AnimationComponent& animationComponent = ball->AddComponent<CSE::AnimationComponent>();
			animationComponent.Add(
				CSE::EntityStates::STAND1, 
				new CSE::AnimationFrames( {80, 0}, {99, 29}, 20, 29, 4.0f, true)
				);
			animationComponent.Add(
				CSE::EntityStates::STAND2, 
				new CSE::AnimationFrames( {260, 0}, {279, 29}, 20, 29, 4.0f, true)
				);
			animationComponent.Add(
				CSE::EntityStates::WALK1, 
				new CSE::AnimationFrames( {80, 0}, {159, 29}, 20, 29, 20.0f, true)
				);
			animationComponent.Add(
				CSE::EntityStates::WALK2, 
				new CSE::AnimationFrames( {260, 0}, {200, 29}, -20, 29, 20.0f, true)
				);
			animationComponent.Add(
				CSE::EntityStates::JUMP1, 
				new CSE::AnimationFrames( {160, 0}, {179, 29}, 20, 29, 20.0f, true)
				);
			animationComponent.Add(
				CSE::EntityStates::JUMP2, 
				new CSE::AnimationFrames( {180, 0}, {199, 29}, 20, 29, 20.0f, true)
				);
			animationComponent.Set(CSE::EntityStates::STAND1);
			animationComponent.Start();
		}
	}
	
	void OnUpdate(CSE::TimeType timeFrame)
	{
		auto& ballKeyBoard = ball->GetComponent<CSE::KeyBoardComponent>();
		auto& ballAnimation = ball->GetComponent<CSE::AnimationComponent>();
		auto& ballPosition = ball->GetComponent<CSE::PositionComponent>();
		auto& ballState = ball->GetComponent<CSE::StateMachineComponent>();
		
		if ((CSE::Input::IsButtonPressed(ballKeyBoard.controls[CSE::Commands::KBCommand_Left])) ||
			(CSE::Input::IsButtonPressed(ballKeyBoard.controls[CSE::Commands::KBCommand_Right])))
		{
			if (CSE::Input::IsButtonPressed(ballKeyBoard.controls[CSE::Commands::KBCommand_Left]))
			{
				// change substate
				if (ballPosition.direction != -1)
				{
					ballPosition.direction = -1;
					ballAnimation.Set(CSE::EntityStates::WALK2);
				}
			}
			
			if (CSE::Input::IsButtonPressed(ballKeyBoard.controls[CSE::Commands::KBCommand_Right]))
			{
				// change substate
				if (ballPosition.direction != 1)
				{
					ballPosition.direction = 1;
					ballAnimation.Set(CSE::EntityStates::WALK1);
				}
			}

			// change state
			if (!ballState.isWalking)
			{
				// exit previous state
				ballAnimation.Stop();
				// initiate new state
				ballState.isWalking = true;
				ballAnimation.Set((ballPosition.direction == 1) ? CSE::EntityStates::WALK1 : CSE::EntityStates::WALK2);
				// start new state
				ballAnimation.Start();
			}
			
			ballPosition.x += ballPosition.direction * 0.002f;
			
		} else {
			// change state
			if (ballState.isWalking)
			{
				// exit previous state
				ballAnimation.Stop();
				// initiate new state
				ballState.isWalking = false;
				// start new state
				ballAnimation.Set((ballPosition.direction == 1) ? CSE::EntityStates::STAND1 : CSE::EntityStates::STAND2);
				ballAnimation.Start();
			}
		}
	}
	
private:
	CSE::Texture* sprite = nullptr;
	CSE::Entity* ball = nullptr;
};

// Layers of the main (default) window, which is created with the application start.
class Layer1 : public CSE::Layer
{
public:
	Layer1()
	: CSE::Layer("Game")
	{};
	
	~Layer1() 
	{
		if (sceneLogo != nullptr)
			delete sceneLogo;
		sceneLogo = nullptr;
		if (scene != nullptr)
			delete scene;
		scene = nullptr;
	};
	
	bool OnAttach()
	{
		// TODO: Automated Scene collection to make loading/unloading routine easier 
		sceneLogo = new SceneLogo();
		LoadScene(sceneLogo);
		CSE_LOG("Scene \"sceneLogo\" has been created and loaded.");

		scene = new SceneGame();
		CSE_LOG("Scene \"scene\" has been created.");

		return true;
	}
	
	bool OnEvent(SDL_Event* event)
	{
		// CSE_LOG("Layer ", GetName(), " stopped event: ", event->type);
		if (event->type == SDL_KEYDOWN)
		{
			if (event->key.keysym.sym == SDLK_SPACE)
			{
				if ((m_Scene == sceneLogo) && (sceneLogo != nullptr))
				{
					UnloadScene(sceneLogo);
					CSE_LOG("Scene \"sceneLogo\" has been unloaded");
					LoadScene(scene);
					CSE_LOG("Scene \"scene\" has been loaded");
				} else {
					if ((m_Scene == scene) && (scene != nullptr))
					{
						UnloadScene(scene);
						CSE_LOG("Scene \"scene\" has been unloaded");
						LoadScene(sceneLogo);
						CSE_LOG("Scene \"sceneLogo\" has been loaded");
					}
				}
			}
		}
		
		return true; // "true" means that event doesn't move further 
	}
	
	CSE::Scene* sceneLogo = nullptr;
	CSE::Scene* scene = nullptr;
};

class Layer2 : public CSE::Layer
{
public:
	Layer2()
	: CSE::Layer("GUI")
	{};
	
	~Layer2() {};
	
	bool OnEvent(SDL_Event* event)
	{
		// CSE_LOG("Layer ", GetName(), " stopped event: ", event->type);
		return true; // "true" means that event doesn't move further
	}
};

class App : public CSE::Application
{
public:
	App(const CSE::WindowPrefs& prefs)
	: CSE::Application(prefs) 
	{
		CSE_LOG("App constructor");
		CSE::Ref<CSE::Layer> layer1(new Layer1());
		CSE::Ref<CSE::Layer> layer2(new Layer2());
		
		GetWindows().Push(new CSE::Window(prefs));
		// m_WindowStack.Push(new CSE::Window({"CSE: Второе окно", 400, 100, 320, 240}));
		CSE_LOG("Total windows in App's WindowStack: ", GetWindows().Size());
		
		for (CSE::Window* window : GetWindows())
		{
			AttachLayer(window, layer1);
			CSE_LOG("Layer ", layer1.get()->GetName(), " attached to window #", window->GetNativeWindowID());
			AttachLayer(window, layer2);
			CSE_LOG("Layer ", layer2.get()->GetName(), " attached to window #", window->GetNativeWindowID());
		}
		
		LimitFPS(CSE_FPS60);
	};
	~App() 
	{
	};
};

CSE::Application* CSE::CreateApplication()
{
	return new App({"CSE: Тест", 100, 100, 320, 240});
	// return new App();
}
