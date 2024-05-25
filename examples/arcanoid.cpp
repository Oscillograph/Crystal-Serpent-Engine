#include <CSE/cse.h>
#include <CSE/systems/components.h>

// Scenes
class SceneGame : public CSE::Scene
{
public:
	SceneGame()
		: CSE::Scene(CSE::PhysicsSystem::None)
	{
		CSE_LOG("SceneGame init");
	};
	
	~SceneGame()
	{
		delete backgroundEntity;
		backgroundEntity = nullptr;
		
		delete playerEntity;
		playerEntity = nullptr;
		
		delete ballEntity;
		ballEntity = nullptr;
		
		for (CSE::Entity* e : bricksEntities)
		{
			delete e;
			e = nullptr;
		}
		
	}
	
	void OnInit()
	{
		if (backgroundEntity == nullptr)
		{
			backgroundEntity = new CSE::Arcade::Decoration(
				this, 
				"CSE Logotype",
				"./arcanoid/assets/background-03.jpg",
				{160, 120},
				{320, 240}
				);
		}
		
		if (playerEntity == nullptr)
		{
			playerEntity = new CSE::Arcade::Unit(
				this,
				"Player",
				"./arcanoid/assets/player.png",
				{
					{ CSE::Commands::KBCommand_Left, CSE::ScanCode::Left },
					{ CSE::Commands::KBCommand_Right, CSE::ScanCode::Right },
					{ CSE::Commands::KBCommand_Space, CSE::ScanCode::Space },
				},
				{160, 120},
				{80, 20}
				);
			
			CSE::PositionComponent& position = playerEntity->GetComponent<CSE::PositionComponent>();
			position.x = 160;
			position.y = 210;
		}
		
		if (ballEntity == nullptr)
		{
			ballEntity = new CSE::Arcade::Unit(
				this,
				"Ball",
				"./arcanoid/assets/ball.png",
				{},
				{160, 120},
				{5, 5}
				);
			
			CSE::PositionComponent& position = ballEntity->GetComponent<CSE::PositionComponent>();
			position.x = 160;
			position.y = 200;
		}
		
		game.started = true;
	}
	
	void OnUpdate(CSE::TimeType timeFrame)
	{
		
		if (game.started)
		{
			if (!game.paused)
			{
				auto players = GetRegistry().view<CSE::KeyBoardComponent>();
				for (auto& p : players)
				{
					// access components of the player entity
					CSE::Entity player(p, this);
					CSE::KeyBoardComponent& keyBoard = player.GetComponent<CSE::KeyBoardComponent>();
//					CSE::AnimationComponent& animation = player.GetComponent<CSE::AnimationComponent>();
					CSE::PositionComponent& position = player.GetComponent<CSE::PositionComponent>();
					CSE::PhysicsComponent& physics = player.GetComponent<CSE::PhysicsComponent>();
					CSE::TransformComponent& transform = player.GetComponent<CSE::TransformComponent>();
					CSE::StateMachineComponent& stateMachine = player.GetComponent<CSE::StateMachineComponent>();
					
					// process states
					if (stateMachine.GetState()->data == CSE::EntityStates::STAND)
					{
						if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Left]))
							stateMachine.SetState(CSE::EntityStates::WALK);
						if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Right]))
							stateMachine.SetState(CSE::EntityStates::WALK);
						
//						if (CSE::EntityStates::STAND != animation.Get())
//						{
//							animation.Set(CSE::EntityStates::STAND);
//							animation.Start();
//						}
					}
					
					if (stateMachine.GetState()->data == CSE::EntityStates::WALK)
					{
						if ((CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Left])) ||
							(CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Right])))
						{
							if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Left]))
							{
								transform.position.x += -game.maxSpeed;
							}
							
							if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Right]))
							{
								transform.position.x += game.maxSpeed;
							}
							
							glm::vec2 windowSize = {
								GetLayer()->GetWindow()->GetPrefs().width,
								GetLayer()->GetWindow()->GetPrefs().height
							};
							
							transform.Normalize({windowSize.x, windowSize.y});
							
						} else {
							stateMachine.SetState(CSE::EntityStates::STAND);
						}
						CSE_LOG("Position is: ", transform.position.x, ", ", transform.position.y);
						
//						if (CSE::EntityStates::WALK != animation.Get())
//						{
//							animation.Set(CSE::EntityStates::WALK);
//							animation.Start();
//						}
					}
				}
			}
		}
	}
	
private:
	CSE::Entity* backgroundEntity = nullptr;
	CSE::Entity* playerEntity = nullptr;
	CSE::Entity* ballEntity = nullptr;
	std::vector<CSE::Entity*> bricksEntities = {};
	
	// game stuff
	struct GameStruct 
	{
		int balls = 0;
		int score = 0;
		bool started = false;
		bool paused = false;
		bool over = false;
		float maxSpeed = 2.0f;
		
		int level[8][10] = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
			0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};
	};
	GameStruct game;
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
		delete arcanoidScene;
		arcanoidScene = nullptr;
		
		delete viewport;
		viewport = nullptr;
		
		delete camera;
		camera = nullptr;
	};
	
	bool OnAttach()
	{
		// TODO: Automated Scene collection to make loading/unloading routine easier 
		arcanoidScene = new SceneGame();
		
		LoadScene(arcanoidScene);
		CSE_LOG("Scene \"Arcanoid\" has been created and loaded.");
		
		return true;
	}
	
	bool OnEvent(SDL_Event* event)
	{
		return true; // "true" means that event doesn't move further 
	}
	
	CSE::Scene* arcanoidScene = nullptr;
	CSE::Viewport* viewport = nullptr;
	CSE::Camera2D* camera = nullptr;
};

class App : public CSE::Application
{
public:
	App(const CSE::WindowPrefs& prefs)
		: CSE::Application(prefs) 
	{
		CSE_LOG("App constructor");
		CSE::Ref<CSE::Layer> layer1(new Layer1());
		
		GetWindows().Push(new CSE::Window(prefs));
		// m_WindowStack.Push(new CSE::Window({"CSE: Второе окно", 400, 100, 320, 240}));
		CSE_LOG("Total windows in App's WindowStack: ", GetWindows().Size());
		
		for (CSE::Window* window : GetWindows())
		{
			AttachLayer(window, layer1);
			CSE_LOG("Layer ", layer1.get()->GetName(), " attached to window #", window->GetNativeWindowID());
		}
		
		// LimitFPS(CSE_FPS60);
		// LimitFPS(CSE_FPSNOLIMIT);
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
