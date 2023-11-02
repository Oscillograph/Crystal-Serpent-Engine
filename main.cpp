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
			CSE::TransformComponent& transform = logo->AddComponent<CSE::TransformComponent>();
			transform.size = {1.0f, 1.0f};
			sprite = new CSE::Texture("./CSE/assets/CSE_logo.png", GetLayer()->GetWindow()->GetRenderer());
			CSE::SpriteComponent& spriteComponent = logo->AddComponent<CSE::SpriteComponent>(sprite);
			// spriteComponent.tilingFactor = {0.0f, 0.0f};
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
		if (player1 != nullptr)
			delete player1;
		player1 = nullptr;
		if (player2 != nullptr)
			delete player2;
		player2 = nullptr;
		if (floor != nullptr)
			delete floor;
		floor = nullptr;
	}
	
	void Init()
	{
		if (player1 == nullptr)
		{
			// a player1 entity on the screen
			player1 = CreateEntity("player1");
			CSE::PositionComponent& position = player1->AddComponent<CSE::PositionComponent>(0.75f, 0.9f);
			position.direction = 1; // 1 means right, -1 means left
			
			CSE::TransformComponent& transform = player1->AddComponent<CSE::TransformComponent>();
			transform.size = {0.1f, 0.15f};
			
			CSE::StateMachineComponent& stateMachine = player1->AddComponent<CSE::StateMachineComponent>();
			CSE::State* player1WalkState = stateMachine.AddState(CSE::EntityStates::WALK);
			CSE::State* player1StandState = stateMachine.AddState(CSE::EntityStates::STAND);

			player1WalkState->AllowEntryFrom(player1StandState->data); 
			player1WalkState->AllowExitTo(player1StandState->data);
			
			player1StandState->AllowEntryFrom(player1WalkState->data);
			player1StandState->AllowExitTo(player1WalkState->data);
			stateMachine.SetState(CSE::EntityStates::STAND);
			
			std::unordered_map<int, SDL_Keycode> player1KBControls = {
				{CSE::Commands::KBCommand_Left, CSE::ScanCode::Left}, 
				{CSE::Commands::KBCommand_Right, CSE::ScanCode::Right},
				{CSE::Commands::KBCommand_Up, CSE::ScanCode::Up},
				{CSE::Commands::KBCommand_Down, CSE::ScanCode::Down}
			};
			
			CSE::KeyBoardComponent& keyboard = player1->AddComponent<CSE::KeyBoardComponent>(player1KBControls);
			
			sprite = new CSE::Texture("./App/Sprites.png", GetLayer()->GetWindow()->GetRenderer(), {0, 0, 0});
			CSE::SpriteComponent& spriteComponent = player1->AddComponent<CSE::SpriteComponent>(sprite);
			CSE::AnimationComponent& animationComponent = player1->AddComponent<CSE::AnimationComponent>();
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
		
		if (player2 == nullptr)
		{
			// a player1 entity on the screen
			player2 = CreateEntity("player1");
			CSE::PositionComponent& position = player2->AddComponent<CSE::PositionComponent>(0.25f, 0.9f);
			position.direction = 1; // 1 means right, -1 means left
			
			CSE::TransformComponent& transform = player2->AddComponent<CSE::TransformComponent>();
			transform.size = {0.1f, 0.15f};
			
			CSE::StateMachineComponent& stateMachine = player2->AddComponent<CSE::StateMachineComponent>();
			CSE::State* player2WalkState = stateMachine.AddState(CSE::EntityStates::WALK);
			CSE::State* player2StandState = stateMachine.AddState(CSE::EntityStates::STAND);
			
			player2WalkState->AllowEntryFrom(player2StandState->data); 
			player2WalkState->AllowExitTo(player2StandState->data);
			
			player2StandState->AllowEntryFrom(player2WalkState->data);
			player2StandState->AllowExitTo(player2WalkState->data);
			stateMachine.SetState(CSE::EntityStates::STAND);
			
			std::unordered_map<int, SDL_Keycode> player2KBControls = {
				{CSE::Commands::KBCommand_Left, CSE::ScanCode::A}, 
				{CSE::Commands::KBCommand_Right, CSE::ScanCode::D},
				{CSE::Commands::KBCommand_Up, CSE::ScanCode::W},
				{CSE::Commands::KBCommand_Down, CSE::ScanCode::S}
			};
			
			CSE::KeyBoardComponent& keyboard = player2->AddComponent<CSE::KeyBoardComponent>(player2KBControls);
			
			sprite = new CSE::Texture("./App/Sprites.png", GetLayer()->GetWindow()->GetRenderer(), {0, 0, 0});
			CSE::SpriteComponent& spriteComponent = player2->AddComponent<CSE::SpriteComponent>(sprite);
			CSE::AnimationComponent& animationComponent = player2->AddComponent<CSE::AnimationComponent>();
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
		
		if (floor == nullptr)
		{
			// a player1 entity on the screen
			floor = CreateEntity("Floor");
			CSE::PositionComponent& position = floor->AddComponent<CSE::PositionComponent>(0.5f, 0.9f);
			CSE::TransformComponent& transform = floor->AddComponent<CSE::TransformComponent>();
			transform.size = {0.05f, 0.5f};
			
			CSE::StateMachineComponent& stateMachine = floor->AddComponent<CSE::StateMachineComponent>();
			stateMachine.AddState(CSE::EntityStates::STAND);
			stateMachine.SetState(CSE::EntityStates::STAND);
			
			CSE::SpriteComponent& spriteComponent = floor->AddComponent<CSE::SpriteComponent>(sprite);
			spriteComponent.tilingFactor = {0.0f, 2.0f};
			
			CSE::AnimationComponent& animationComponent = floor->AddComponent<CSE::AnimationComponent>();
			animationComponent.Add(
				CSE::EntityStates::STAND1, 
				new CSE::AnimationFrames( {72, 26}, {76, 29}, 5, 4, 4.0f, true)
				);
			animationComponent.Set(CSE::EntityStates::STAND1);
			animationComponent.Start();
			
			CSE::PhysicsComponent& physicsComponent = floor->AddComponent<CSE::PhysicsComponent>();
			physicsComponent.bodyType = CSE::PhysicsDefines::BodyType::Static;
		}
	}
	
	void OnUpdate(CSE::TimeType timeFrame)
	{
		// THE player1 SECTION
		CSE::KeyBoardComponent& player1KeyBoard = player1->GetComponent<CSE::KeyBoardComponent>();
		CSE::AnimationComponent& player1Animation = player1->GetComponent<CSE::AnimationComponent>();
		CSE::PositionComponent& player1Position = player1->GetComponent<CSE::PositionComponent>();
		CSE::StateMachineComponent& player1StateMachine = player1->GetComponent<CSE::StateMachineComponent>();
		
		if ((CSE::Input::IsButtonPressed(player1KeyBoard.controls[CSE::Commands::KBCommand_Left]))
			|| (CSE::Input::IsButtonPressed(player1KeyBoard.controls[CSE::Commands::KBCommand_Right]))
			|| (CSE::Input::IsButtonPressed(player1KeyBoard.controls[CSE::Commands::KBCommand_Up]))
			|| (CSE::Input::IsButtonPressed(player1KeyBoard.controls[CSE::Commands::KBCommand_Down])))
		{
			// change state
			if (player1StateMachine.GetState()->data == CSE::EntityStates::STAND)
			{
				player1StateMachine.SetState(CSE::EntityStates::WALK);
				
				// recover previous animation if a substate did not change
				player1Animation.Set((player1Position.direction == 1) ? CSE::EntityStates::WALK1 : CSE::EntityStates::WALK2);
				player1Animation.Start();
			}
			
			// change substate
			if (CSE::Input::IsButtonPressed(player1KeyBoard.controls[CSE::Commands::KBCommand_Left]))
			{
				// change substate
				if (player1Position.direction != -1)
				{
					player1Position.direction = -1;
				}
				
				// set a proper animation
				if (CSE::EntityStates::WALK2 != player1Animation.Get())
				{
					player1Animation.Set(CSE::EntityStates::WALK2);
					player1Animation.Start();
				}
			}
			
			// change substate
			if (CSE::Input::IsButtonPressed(player1KeyBoard.controls[CSE::Commands::KBCommand_Right]))
			{
				if (player1Position.direction != 1)
				{
					player1Position.direction = 1;
				}
				
				// set a proper animation
				if (CSE::EntityStates::WALK1 != player1Animation.Get())
				{
					player1Animation.Set(CSE::EntityStates::WALK1);
					player1Animation.Start();
				}
			}
			
			// process states
			// TODO: Implement physics: an update should take into consideration the app inner time
			if (player1StateMachine.GetState()->data == CSE::EntityStates::WALK)
			{
				if (CSE::Input::IsButtonPressed(player1KeyBoard.controls[CSE::Commands::KBCommand_Up]))
					player1Position.y -= 0.002f;
				if (CSE::Input::IsButtonPressed(player1KeyBoard.controls[CSE::Commands::KBCommand_Down]))
					player1Position.y += 0.002f;
				if (CSE::Input::IsButtonPressed(player1KeyBoard.controls[CSE::Commands::KBCommand_Left]))
					player1Position.x -= 0.002f;
				if (CSE::Input::IsButtonPressed(player1KeyBoard.controls[CSE::Commands::KBCommand_Right]))
					player1Position.x += 0.002f;
				CSE_LOG("player1 coordinates: (", player1Position.x, "; ", player1Position.y, ")");
			}
		} else {
			if (player1StateMachine.GetState()->data == CSE::EntityStates::WALK)
			{
				player1StateMachine.SetState(CSE::EntityStates::STAND);
				
				player1Animation.Set((player1Position.direction == 1) ? CSE::EntityStates::STAND1 : CSE::EntityStates::STAND2);
				player1Animation.Start();
			}
		}
		// \ THE player1 SECTION
		
		// THE player2 SECTION
		CSE::KeyBoardComponent& player2KeyBoard = player2->GetComponent<CSE::KeyBoardComponent>();
		CSE::AnimationComponent& player2Animation = player2->GetComponent<CSE::AnimationComponent>();
		CSE::PositionComponent& player2Position = player2->GetComponent<CSE::PositionComponent>();
		CSE::StateMachineComponent& player2StateMachine = player2->GetComponent<CSE::StateMachineComponent>();
		
		if ((CSE::Input::IsButtonPressed(player2KeyBoard.controls[CSE::Commands::KBCommand_Left]))
			|| (CSE::Input::IsButtonPressed(player2KeyBoard.controls[CSE::Commands::KBCommand_Right]))
			|| (CSE::Input::IsButtonPressed(player2KeyBoard.controls[CSE::Commands::KBCommand_Up]))
			|| (CSE::Input::IsButtonPressed(player2KeyBoard.controls[CSE::Commands::KBCommand_Down])))
		{
			// change state
			if (player2StateMachine.GetState()->data == CSE::EntityStates::STAND)
			{
				player2StateMachine.SetState(CSE::EntityStates::WALK);
				
				// recover previous animation if a substate did not change
				player2Animation.Set((player1Position.direction == 1) ? CSE::EntityStates::WALK1 : CSE::EntityStates::WALK2);
				player2Animation.Start();
			}
			
			// change substate
			if (CSE::Input::IsButtonPressed(player2KeyBoard.controls[CSE::Commands::KBCommand_Left]))
			{
				// change substate
				if (player2Position.direction != -1)
				{
					player2Position.direction = -1;
				}
				
				// set a proper animation
				if (CSE::EntityStates::WALK2 != player2Animation.Get())
				{
					player2Animation.Set(CSE::EntityStates::WALK2);
					player2Animation.Start();
				}
			}
			
			// change substate
			if (CSE::Input::IsButtonPressed(player2KeyBoard.controls[CSE::Commands::KBCommand_Right]))
			{
				if (player2Position.direction != 1)
				{
					player2Position.direction = 1;
				}
				
				// set a proper animation
				if (CSE::EntityStates::WALK1 != player2Animation.Get())
				{
					player2Animation.Set(CSE::EntityStates::WALK1);
					player2Animation.Start();
				}
			}
			
			// process states
			// TODO: Implement physics: an update should take into consideration the app inner time
			if (player2StateMachine.GetState()->data == CSE::EntityStates::WALK)
			{
				if (CSE::Input::IsButtonPressed(player2KeyBoard.controls[CSE::Commands::KBCommand_Up]))
					player2Position.y -= 0.002f;
				if (CSE::Input::IsButtonPressed(player2KeyBoard.controls[CSE::Commands::KBCommand_Down]))
					player2Position.y += 0.002f;
				if (CSE::Input::IsButtonPressed(player2KeyBoard.controls[CSE::Commands::KBCommand_Left]))
					player2Position.x -= 0.002f;
				if (CSE::Input::IsButtonPressed(player2KeyBoard.controls[CSE::Commands::KBCommand_Right]))
					player2Position.x += 0.002f;
				CSE_LOG("player2 coordinates: (", player2Position.x, "; ", player2Position.y, ")");
			}
		} else {
			if (player2StateMachine.GetState()->data == CSE::EntityStates::WALK)
			{
				player2StateMachine.SetState(CSE::EntityStates::STAND);
				
				player2Animation.Set((player2Position.direction == 1) ? CSE::EntityStates::STAND1 : CSE::EntityStates::STAND2);
				player2Animation.Start();
			}
		}
		// \ THE player2 SECTION
	}
	
private:
	CSE::Texture* sprite = nullptr;
	CSE::Entity* player1 = nullptr;
	CSE::Entity* player2 = nullptr;
	CSE::Entity* floor = nullptr;
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
