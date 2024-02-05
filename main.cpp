#include "./cse.h"

// Scenes
class SceneLogo : public CSE::Scene
{
public:
	SceneLogo()
		: CSE::Scene(CSE::PhysicsSystem::None)
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
			logo = new CSE::Arcade::Decoration(
				this, 
				"CSE Logotype",
				"./CSE/assets/CSE_logo.png",
				{160, 120},
				{320, 240}
				);
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
		if (grid != nullptr)
			delete grid;
		grid = nullptr;
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
			background = new CSE::Arcade::Decoration(
				this, 
				"Background",
				"./CSE/assets/CSE_logo.png",
				{80, 60},
				{160, 120},
				{0.6f, 0.6f},
				{0, 0, 0}
				);
		}
		
		CSE_LOG("- background SDL");
		if (backgroundSDL == nullptr)
		{
			backgroundSDL = new CSE::Arcade::Decoration(
				this, 
				"Background SDL",
				"./CSE/assets/SDL_logo.png",
				{240, 60},
				{160, 120},
				{0.4f, 0.4f},
				{0, 0, 0}
				);
		}
		
		CSE_LOG("- player 1");
		if (player1 == nullptr)
		{
			// a player1 entity on the screen
			CSE::KBControls player1KBControls = {
				{CSE::Commands::KBCommand_Left, CSE::ScanCode::A}, 
				{CSE::Commands::KBCommand_Right, CSE::ScanCode::D},
				{CSE::Commands::KBCommand_Jump, CSE::ScanCode::W},
				{CSE::Commands::KBCommand_Down, CSE::ScanCode::S}
			};
			
			player1 = new CSE::Arcade::Unit(
				this,
				"Player 1",
				"./App/Volleyballist.png",
				player1KBControls,
				{80, 216},
				{32, 36},
				{0.0f, 0.0f},
				{0, 0, 0},
				{70, 170}
				);
			((CSE::Arcade::Unit*)(player1))->ChangeDirection(1);
			
			((CSE::Arcade::Unit*)(player1))->SetAnimations(
				{
					CSE::EntityStates::STAND1,
					CSE::EntityStates::STAND2,
					CSE::EntityStates::WALK1,
					CSE::EntityStates::WALK2,
					CSE::EntityStates::JUMP1,
					CSE::EntityStates::JUMP2
				},
				{20, 30},
				15.0f,
				true
				);
		}
		
		CSE_LOG("- player 2");
		if (player2 == nullptr)
		{
			// a player1 entity on the screen
			CSE::KBControls player2KBControls = {
				{CSE::Commands::KBCommand_Left, CSE::ScanCode::Left}, 
				{CSE::Commands::KBCommand_Right, CSE::ScanCode::Right},
				{CSE::Commands::KBCommand_Jump, CSE::ScanCode::Up},
				{CSE::Commands::KBCommand_Down, CSE::ScanCode::Down}
			};
			
			player2 = new CSE::Arcade::Unit(
				this,
				"Player 2",
				"./App/Volleyballist.png",
				player2KBControls,
				{240, 216},
				{32, 36},
				{0.0f, 0.0f},
				{0, 0, 0},
				{130, 170}
				);
			((CSE::Arcade::Unit*)(player2))->ChangeDirection(-1);
			
			((CSE::Arcade::Unit*)(player2))->SetAnimations(
				{
					CSE::EntityStates::STAND1,
					CSE::EntityStates::STAND2,
					CSE::EntityStates::WALK1,
					CSE::EntityStates::WALK2,
					CSE::EntityStates::JUMP1,
					CSE::EntityStates::JUMP2
				},
				{20, 30},
				15.0f,
				true
				);
		}
		
		CSE_LOG("- grid");
		if (grid == nullptr)
		{
			// a player1 entity on the screen
			// grid = CreateEntity("Grid");
			
			grid = new CSE::Arcade::Doodad(
				this,
				"Grid",
				"./App/Grid.png",
				{},
				{160, 186},
				{10, 60},
				{0.0f, 1.0f},
				{0, 0, 0},
				{100, 100},
				{
					{-0.01, 4}, 
					{0.01, 4}, 
					{0.01, 0}, 
					{-0.01, 0}
				}
				);
			
			((CSE::Arcade::Doodad*)(grid))->SetAnimations(
				{
					CSE::EntityStates::STAND1,
				},
				{5, 4},
				1.0f,
				true
				);
		}
		
		CSE_LOG("- floor");
		if (floor == nullptr)
		{
			// a player1 entity on the screen
			floor = new CSE::Arcade::Doodad(
				this,
				"Floor",
				"",
				{},
				{160, 186},
				{10, 60},
				{0.0f, 0.0f},
				{0, 0, 0},
				{100, 178},
				{
					{-100,  0}, 
					{ 100,  0}, 
					{ 100, -2}, 
					{-100, -2}
				}
				);
			
			((CSE::Arcade::Doodad*)(floor))->SetAnimations(
				{
					CSE::EntityStates::STAND1,
				},
				{5, 4},
				1.0f,
				true
				);
		}
		
		// turn physics off temporarily
		// PhysicsOff();
	}
	
	void OnUpdate(CSE::TimeType timeFrame)
	{
		float maxSpeed = 20.0f;
		
		auto players = GetRegistry().view<CSE::KeyBoardComponent>();
		for (auto& p : players)
		{
			CSE::Entity player(p, this);
			
			// THE player SECTION
			CSE::KeyBoardComponent& keyBoard = player.GetComponent<CSE::KeyBoardComponent>();
			CSE::AnimationComponent& animation = player.GetComponent<CSE::AnimationComponent>();
			CSE::PositionComponent& position = player.GetComponent<CSE::PositionComponent>();
			CSE::PhysicsComponent& physics = player.GetComponent<CSE::PhysicsComponent>();
			CSE::TransformComponent& transform = player.GetComponent<CSE::TransformComponent>();
			CSE::StateMachineComponent& stateMachine = player.GetComponent<CSE::StateMachineComponent>();
			
			// process states
			// TODO: Implement physics: an update should take into consideration the app inner time
			if (stateMachine.GetState()->data == CSE::EntityStates::STAND)
			{
				// change state
				// physics.velocity.x = 0.0f;
				// physics.velocity.y = 0.0f;
				
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
						// physics.position.y += 0.2f;
						//GetActiveCamera()->MoveBy({0.0f, 0.2f});
					}
					
					if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Left]))
					{
						// physics.position.x -= 0.2f;
						// physics.velocity.x = -maxSpeed;
						GetPhysicsProcessor()->ApplyForce(player, {-1.0f, 0.f}, maxSpeed);
						//GetActiveCamera()->MoveBy({-0.2f, 0.0f});
					}
					
					if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Right]))
					{
						// physics.position.x += 0.2f;
						// physics.velocity.x = maxSpeed;
						GetPhysicsProcessor()->ApplyForce(player, {1.0f, 0.f}, maxSpeed);
						//GetActiveCamera()->MoveBy({0.2f, 0.0f});
					}
					
					CSE_LOG(player.GetComponent<CSE::NameComponent>().value, " coordinates: (", physics.position.x, "; ", physics.position.y, ")");
					CSE_LOG(player.GetComponent<CSE::NameComponent>().value, " size: (", transform.size.x, "; ", transform.size.y, ")");
				} else {
					stateMachine.SetState(CSE::EntityStates::STAND);
				}
			}
			
			if (stateMachine.GetState()->data == CSE::EntityStates::JUMP)
			{
				if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Jump]))
				{
					if (physics.position.y > 140.0f)
					{
						// physics.position.y -= 0.2f;
						GetPhysicsProcessor()->ApplyForce(player, {0.0f, -1.0f}, maxSpeed);
						//GetActiveCamera()->MoveBy({0.0f, -0.2f});
					} else {
						stateMachine.SetState(CSE::EntityStates::FLY);
					}
				} else {
					stateMachine.SetState(CSE::EntityStates::FLY);
				}
				
				if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Left]))
				{
					// physics.position.x -= 0.2f;
					// physics.velocity.x = -maxSpeed;
					GetPhysicsProcessor()->ApplyForce(player, {-1.0f, 0.f}, maxSpeed);
					//GetActiveCamera()->MoveBy({-0.2f, 0.0f});
					
					// change animation if needed
					if (CSE::EntityStates::JUMP2 != animation.Get())
					{
						animation.Set(CSE::EntityStates::JUMP2);
						animation.Start();
					}
				}
				
				if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Right]))
				{
					// physics.position.x += 0.2f;
					// physics.velocity.x = maxSpeed;
					GetPhysicsProcessor()->ApplyForce(player, {1.0f, 0.f}, maxSpeed);
					//GetActiveCamera()->MoveBy({0.2f, 0.0f});
					
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
				physics.velocity.y = maxSpeed;
				
				if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Left]))
				{
					// physics.position.x -= 0.2f;
					// physics.velocity.x = -maxSpeed;
					GetPhysicsProcessor()->ApplyForce(player, {-1.0f, 0.f}, maxSpeed);
					//GetActiveCamera()->MoveBy({-0.2f, 0.0f});
					
					// change animation if needed
					if (CSE::EntityStates::JUMP2 != animation.Get())
					{
						animation.Set(CSE::EntityStates::JUMP2);
						animation.Start();
					}
				}
				
				if (CSE::Input::IsButtonPressed(keyBoard.controls[CSE::Commands::KBCommand_Right]))
				{
					// physics.position.x += 0.2f;
					// physics.velocity.x = maxSpeed;
					GetPhysicsProcessor()->ApplyForce(player, {1.0f, 0.f}, maxSpeed);
					//GetActiveCamera()->MoveBy({0.2f, 0.0f});
					
					// change animation if needed
					if (CSE::EntityStates::JUMP1 != animation.Get())
					{
						animation.Set(CSE::EntityStates::JUMP1);
						animation.Start();
					}
				}
				
				if (physics.position.y < 100.0f)
				{
					// physics.velocity.y = maxSpeed;
					// physics.position.y += 0.2f;
					//GetActiveCamera()->MoveBy({0.0f, 0.2f});
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
	CSE::Entity* grid = nullptr;
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
		
		// viewport = new CSE::Viewport(nullptr, {80, 60, 160, 120});
		// SetViewport(viewport);
		
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
