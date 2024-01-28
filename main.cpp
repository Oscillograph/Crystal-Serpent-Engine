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
	
	void OnInit()
	{
		// TODO: figure out how to de-initialize a scene for a proper scene unload mechanism
		if (logo == nullptr)
		{
			logo = CreateEntity("CSE Logotype");
			
			// screen position
			CSE::TransformComponent& transform = logo->AddComponent<CSE::TransformComponent>();
			transform.position = {0, 0};
			transform.size = {320, 240};
			transform.NormalizeToWindow(GetLayer()->GetWindow());
			
			// sprite control
			sprite = new CSE::Texture("./CSE/assets/CSE_logo.png", GetLayer()->GetWindow()->GetRenderer());
			CSE::SpriteComponent& spriteComponent = logo->AddComponent<CSE::SpriteComponent>(sprite);
			// spriteComponent.clip = {9000, 9000};
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
		: CSE::Scene(CSE::PhysicsSystem::Box2D)
	{
	};
	
	~SceneGame()
	{
		if (sprite != nullptr)
			delete sprite;
		sprite = nullptr;
		if (spriteBG != nullptr)
			delete spriteBG;
		spriteBG = nullptr;
		if (spriteSDL != nullptr)
			delete spriteSDL;
		spriteSDL = nullptr;
		if (player1 != nullptr)
			delete player1;
		player1 = nullptr;
		if (player2 != nullptr)
			delete player2;
		player2 = nullptr;
		if (floor != nullptr)
			delete floor;
		floor = nullptr;
		if (background != nullptr)
			delete background;
		background = nullptr;
		if (backgroundSDL != nullptr)
			delete backgroundSDL;
		backgroundSDL = nullptr;
	}
	
	void OnInit()
	{
		CSE_LOG("- background logo");
		if (background == nullptr)
		{
			background = CreateEntity("Background");
			
			// screen representation
			CSE::TransformComponent& transform = background->AddComponent<CSE::TransformComponent>();
			transform.position = {80, 60};
			transform.size = {160, 120};
			transform.NormalizeToWindow(GetLayer()->GetWindow());
			
			// sprite control
			spriteBG = new CSE::Texture("./CSE/assets/CSE_logo.png", GetLayer()->GetWindow()->GetRenderer(), {0, 0, 0});
			CSE::SpriteComponent& spriteComponent = background->AddComponent<CSE::SpriteComponent>(spriteBG);
			spriteComponent.tilingFactor = {0.6f, 0.6f};
		}
		
		CSE_LOG("- background SDL");
		if (backgroundSDL == nullptr)
		{
			backgroundSDL = CreateEntity("Background");
			
			// screen representation
			CSE::TransformComponent& transform = backgroundSDL->AddComponent<CSE::TransformComponent>();
			transform.position = {240, 60};
			transform.size = {160, 120};
			transform.NormalizeToWindow(GetLayer()->GetWindow());
			
			// sprite control
			spriteSDL = new CSE::Texture("./CSE/assets/SDL_logo.png", GetLayer()->GetWindow()->GetRenderer(), {0, 0, 0});
			CSE::SpriteComponent& spriteComponent = backgroundSDL->AddComponent<CSE::SpriteComponent>(spriteSDL);
			spriteComponent.tilingFactor = {0.4f, 0.4f};
		}
		
		CSE_LOG("- player 1");
		if (player1 == nullptr)
		{
			// a player1 entity on the screen
			player1 = CreateEntity("Player 1");
			
			// physical position
			CSE::PositionComponent& position = player1->AddComponent<CSE::PositionComponent>(10, 0);
			position.direction = 1; // 1 means right, -1 means left
			
			// screen representation
			CSE::TransformComponent& transform = player1->AddComponent<CSE::TransformComponent>();
			transform.position = {240, 216};
			transform.size = {32, 36};
			transform.NormalizeToWindow(GetLayer()->GetWindow());
			
			// state machine setup
			CSE::StateMachineComponent& stateMachine = player1->AddComponent<CSE::StateMachineComponent>();
			CSE::State* walkState = stateMachine.AddState(CSE::EntityStates::WALK);
			CSE::State* standState = stateMachine.AddState(CSE::EntityStates::STAND);
			CSE::State* jumpState = stateMachine.AddState(CSE::EntityStates::JUMP);
			CSE::State* flyState = stateMachine.AddState(CSE::EntityStates::FLY);
			CSE::State* fallState = stateMachine.AddState(CSE::EntityStates::FALL);

			walkState->AllowEntryFrom(standState->data);
			walkState->AllowEntryFrom(fallState->data);
			walkState->AllowExitTo(standState->data);
			walkState->AllowExitTo(jumpState->data);
			
			standState->AllowEntryFrom(walkState->data);
			standState->AllowEntryFrom(fallState->data);
			standState->AllowExitTo(walkState->data);
			standState->AllowExitTo(jumpState->data);
			
			jumpState->AllowEntryFrom(standState->data);
			jumpState->AllowEntryFrom(walkState->data);
			jumpState->AllowExitTo(flyState->data);
			
			flyState->AllowEntryFrom(jumpState->data);
			flyState->AllowExitTo(fallState->data);
			
			fallState->AllowEntryFrom(flyState->data);
			fallState->AllowExitTo(standState->data);
			fallState->AllowExitTo(walkState->data);
			
			stateMachine.SetState(CSE::EntityStates::STAND);
			
			std::unordered_map<int, SDL_Keycode> player1KBControls = {
				{CSE::Commands::KBCommand_Left, CSE::ScanCode::Left}, 
				{CSE::Commands::KBCommand_Right, CSE::ScanCode::Right},
				{CSE::Commands::KBCommand_Jump, CSE::ScanCode::Up},
				{CSE::Commands::KBCommand_Down, CSE::ScanCode::Down}
			};
			
			// controls setup
			CSE::KeyBoardComponent& keyboard = player1->AddComponent<CSE::KeyBoardComponent>(player1KBControls);
			
			// sprite control
			sprite = new CSE::Texture("./App/Sprites.png", GetLayer()->GetWindow()->GetRenderer(), {0, 0, 0});
			CSE::SpriteComponent& spriteComponent = player1->AddComponent<CSE::SpriteComponent>(sprite);
			
			// animation addon to sprite control
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
			
			// physics setup
			CSE::PhysicsComponent& physicsComponent = player1->AddComponent<CSE::PhysicsComponent>();
			physicsComponent.position = {10, 0, 0};
			
			CSE::PhysicsHitBox hitbox;
			hitbox.hitBoxType = CSE::PhysicsDefines::HitBoxType::Circle;
			hitbox.points = {{0.0f, 0.0f}};
			hitbox.radius = 1.0f;
			
			physicsComponent.entity = *player1;
			physicsComponent.hitBoxes.push_back(hitbox);
			physicsComponent.velocity = {0.0f, 0.0f};
			physicsComponent.acceleration = {0.0f, 0.0f};
			physicsComponent.mass = 1.0f;
			physicsComponent.bodyType = CSE::PhysicsDefines::BodyType::Static;
			GetPhysicsProcessor()->RegisterEntity(player1);
		}
		
		CSE_LOG("- player 2");
		if (player2 == nullptr)
		{
			// a player1 entity on the screen
			player2 = CreateEntity("Player 2");
			
			// physical position
			CSE::PositionComponent& position = player2->AddComponent<CSE::PositionComponent>(-10, 0);
			position.direction = 1; // 1 means right, -1 means left
			
			// screen representation
			CSE::TransformComponent& transform = player2->AddComponent<CSE::TransformComponent>();
			transform.position = {80, 216};
			transform.size = {32, 36};
			transform.NormalizeToWindow(GetLayer()->GetWindow());
			
			// state machine setup
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
				{CSE::Commands::KBCommand_Jump, CSE::ScanCode::W},
				{CSE::Commands::KBCommand_Down, CSE::ScanCode::S}
			};
			
			// controls setup
			CSE::KeyBoardComponent& keyboard = player2->AddComponent<CSE::KeyBoardComponent>(player2KBControls);
			
			// sprite control
			sprite = new CSE::Texture("./App/Sprites.png", GetLayer()->GetWindow()->GetRenderer(), {0, 0, 0});
			CSE::SpriteComponent& spriteComponent = player2->AddComponent<CSE::SpriteComponent>(sprite);
			
			// animation addon to sprite control
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
		
		CSE_LOG("- floor");
		if (floor == nullptr)
		{
			// a player1 entity on the screen
			floor = CreateEntity("Floor");
			
			// physical position
			CSE::PositionComponent& position = floor->AddComponent<CSE::PositionComponent>(0, 0);
			
			// screen representation
			CSE::TransformComponent& transform = floor->AddComponent<CSE::TransformComponent>();
			transform.position = {160, 216};
			transform.size = {3.2, 120};
			transform.NormalizeToWindow(GetLayer()->GetWindow());
			
			// state machine setup
			CSE::StateMachineComponent& stateMachine = floor->AddComponent<CSE::StateMachineComponent>();
			stateMachine.AddState(CSE::EntityStates::STAND);
			stateMachine.SetState(CSE::EntityStates::STAND);
			
			// sprite control
			CSE::SpriteComponent& spriteComponent = floor->AddComponent<CSE::SpriteComponent>(sprite);
			spriteComponent.tilingFactor = {0.0f, 2.0f};
			
			// animation addon to sprite control
			CSE::AnimationComponent& animationComponent = floor->AddComponent<CSE::AnimationComponent>();
			animationComponent.Add(
				CSE::EntityStates::STAND1, 
				new CSE::AnimationFrames( {72, 26}, {76, 29}, 5, 4, 4.0f, true)
				);
			animationComponent.Set(CSE::EntityStates::STAND1);
			animationComponent.Start();
			
			// physics setup
			
			CSE::PhysicsComponent& physicsComponent = floor->AddComponent<CSE::PhysicsComponent>();
			physicsComponent.position = {0, 0, 0};
			
			CSE::PhysicsHitBox hitbox; // match rectangle points coordinates to transform size
			hitbox.hitBoxType = CSE::PhysicsDefines::HitBoxType::Rectangle;
			hitbox.points = {{-0.1, 10}, {0.1, 10}, {0.1, 0}, {-0.1, 0}};
			// hitbox.radius = 1.0f;
			
			physicsComponent.entity = *floor;
			physicsComponent.hitBoxes.push_back(hitbox);
			physicsComponent.velocity = {0.0f, 0.0f};
			physicsComponent.acceleration = {0.0f, 0.0f};
			physicsComponent.mass = 1.0f;
			physicsComponent.bodyType = CSE::PhysicsDefines::BodyType::Dynamic;
			GetPhysicsProcessor()->RegisterEntity(floor);
			
		}
	}
	
	void OnUpdate(CSE::TimeType timeFrame)
	{
		auto players = GetRegistry().view<CSE::KeyBoardComponent>();
		for (auto& p : players)
		{
			CSE::Entity player(p, this);
			
			// THE player SECTION
			CSE::KeyBoardComponent& keyBoard = player.GetComponent<CSE::KeyBoardComponent>();
			CSE::AnimationComponent& animation = player.GetComponent<CSE::AnimationComponent>();
			CSE::PositionComponent& position = player.GetComponent<CSE::PositionComponent>();
			CSE::TransformComponent& transform = player.GetComponent<CSE::TransformComponent>();
			CSE::StateMachineComponent& stateMachine = player.GetComponent<CSE::StateMachineComponent>();
			
			// process states
			// TODO: Implement physics: an update should take into consideration the app inner time
			if (stateMachine.GetState()->data == CSE::EntityStates::STAND)
			{
				// change state
				if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Left]))
					stateMachine.SetState(CSE::EntityStates::WALK);
				if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Right]))
					stateMachine.SetState(CSE::EntityStates::WALK);
				if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Jump]))
					stateMachine.SetState(CSE::EntityStates::JUMP);
				
				// recover previous animation if a substate did not change
				if ((CSE::EntityStates::STAND1 != animation.Get()) && (CSE::EntityStates::STAND2 != animation.Get()))
				{
					animation.Set((position.direction == 1) ? CSE::EntityStates::STAND1 : CSE::EntityStates::STAND2);
					animation.Start();
				}
			}
			
			if (stateMachine.GetState()->data == CSE::EntityStates::WALK)
			{
				if ((CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Left]))
					|| (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Right]))
					|| (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Jump])))
				{
					// change substate
					if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Left]))
					{
						// change substate
						if (position.direction != -1)
						{
							position.direction = -1;
						}
						
						// set a proper animation
						if (CSE::EntityStates::WALK2 != animation.Get())
						{
							animation.Set(CSE::EntityStates::WALK2);
							animation.Start();
						}
					}
					
					// change substate
					if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Right]))
					{
						if (position.direction != 1)
						{
							position.direction = 1;
						}
						
						// set a proper animation
						if (CSE::EntityStates::WALK1 != animation.Get())
						{
							animation.Set(CSE::EntityStates::WALK1);
							animation.Start();
						}
					}
					
					if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Jump]))
					{
						stateMachine.SetState(CSE::EntityStates::JUMP);
					}
					
					if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Down]))
					{
						position.y += 0.2f;
						GetActiveCamera()->MoveBy({0.0f, 0.2f});
					}
					
					if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Left]))
					{
						position.x -= 0.2f;
						GetActiveCamera()->MoveBy({-0.2f, 0.0f});
					}
					
					if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Right]))
					{
						position.x += 0.2f;
						GetActiveCamera()->MoveBy({0.2f, 0.0f});
					}
					
					CSE_LOG(player.GetComponent<CSE::NameComponent>().value, " coordinates: (", position.x, "; ", position.y, ")");
					CSE_LOG(player.GetComponent<CSE::NameComponent>().value, " size: (", transform.size.x, "; ", transform.size.y, ")");
				} else {
					stateMachine.SetState(CSE::EntityStates::STAND);
				}
			}
			
			if (stateMachine.GetState()->data == CSE::EntityStates::JUMP)
			{
				if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Jump]))
				{
					if (position.y > 0.75f)
					{
						position.y -= 0.2f;
						GetActiveCamera()->MoveBy({0.0f, -0.2f});
					} else {
						stateMachine.SetState(CSE::EntityStates::FLY);
					}
				} else {
					stateMachine.SetState(CSE::EntityStates::FLY);
				}
				
				if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Left]))
				{
					position.x -= 0.2f;
					GetActiveCamera()->MoveBy({-0.2f, 0.0f});
					
					// change animation if needed
					if (CSE::EntityStates::JUMP2 != animation.Get())
					{
						animation.Set(CSE::EntityStates::JUMP2);
						animation.Start();
					}
				}
				
				if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Right]))
				{
					position.x += 0.2f;
					GetActiveCamera()->MoveBy({0.2f, 0.0f});
					
					// change animation if needed 
					if (CSE::EntityStates::JUMP1 != animation.Get())
					{
						animation.Set(CSE::EntityStates::JUMP1);
						animation.Start();
					}
				}
				
				// if the animation is set the first time
				if ((CSE::EntityStates::JUMP1 != animation.Get()) && (CSE::EntityStates::JUMP2 != animation.Get()))
				{
					animation.Set((position.direction == 1) ? CSE::EntityStates::JUMP1 : CSE::EntityStates::JUMP2);
					animation.Start();
				}
			}
			
			if (stateMachine.GetState()->data == CSE::EntityStates::FLY)
			{
				stateMachine.SetState(CSE::EntityStates::FALL);
			}
			
			if (stateMachine.GetState()->data == CSE::EntityStates::FALL)
			{
				if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Left]))
				{
					position.x -= 0.2f;
					GetActiveCamera()->MoveBy({-0.2f, 0.0f});
					
					// change animation if needed
					if (CSE::EntityStates::JUMP2 != animation.Get())
					{
						animation.Set(CSE::EntityStates::JUMP2);
						animation.Start();
					}
				}
				
				if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Right]))
				{
					position.x += 0.2f;
					GetActiveCamera()->MoveBy({0.2f, 0.0f});
					
					// change animation if needed
					if (CSE::EntityStates::JUMP1 != animation.Get())
					{
						animation.Set(CSE::EntityStates::JUMP1);
						animation.Start();
					}
				}
				
				if (position.y < 0.9f)
				{
					position.y += 0.2f;
					GetActiveCamera()->MoveBy({0.0f, 0.2f});
				} else {
					stateMachine.SetState(CSE::EntityStates::STAND);
				}
			}
			// \ THE player SECTION
		}
	}
	
private:
	CSE::Texture* sprite = nullptr;
	CSE::Texture* spriteBG = nullptr;
	CSE::Texture* spriteSDL = nullptr;
	CSE::Entity* player1 = nullptr;
	CSE::Entity* player2 = nullptr;
	CSE::Entity* floor = nullptr;
	CSE::Entity* background = nullptr;
	CSE::Entity* backgroundSDL = nullptr;
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
		if (viewport != nullptr)
			delete viewport;
		viewport = nullptr;
		if (camera != nullptr)
			delete camera;
		camera = nullptr;
	};
	
	bool OnAttach()
	{
		// TODO: Automated Scene collection to make loading/unloading routine easier 
		sceneLogo = new SceneLogo();
		LoadScene(sceneLogo);
		CSE_LOG("Scene \"sceneLogo\" has been created and loaded.");

		scene = new SceneGame();
		CSE_LOG("Scene \"scene\" has been created.");

		// viewport = new CSE::Viewport();
		// camera = new CSE::Camera2D();
		// viewport->SetCamera(camera);
		// viewport->SetScene(scene);
		
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
	CSE::Viewport* viewport = nullptr;
	CSE::Camera2D* camera = nullptr;
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
