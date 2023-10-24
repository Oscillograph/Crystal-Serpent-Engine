#include "./cse.h"

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
		if (spriteLogo != nullptr)
			delete spriteLogo;
		spriteLogo = nullptr;
		if (logo != nullptr)
			delete logo;
		logo = nullptr;
		
		if (scene != nullptr)
			delete scene;
		scene = nullptr;
		if (sprite != nullptr)
			delete sprite;
		sprite = nullptr;
		if (ball != nullptr)
			delete ball;
		ball = nullptr;
	};
	
	bool OnAttach()
	{
		sceneLogo = new CSE::Scene();
		logo = sceneLogo->CreateEntity("CSE Logotype");
		logo->AddComponent<CSE::PositionComponent>(0.5f, 0.5f);
		spriteLogo = new CSE::Texture("./CSE/assets/CSE_logo.png", GetWindow()->GetRenderer());
		logo->AddComponent<CSE::SpriteComponent>(spriteLogo);
		LoadScene(sceneLogo);
		CSE_LOG("SceneLogo has been created and loaded.");

		scene = new CSE::Scene();
		// a ball entity on the screen
		ball = scene->CreateEntity("Ball");
		CSE::PositionComponent& position = ball->AddComponent<CSE::PositionComponent>(0.5f, 0.5f);
		
		std::unordered_map<int, SDL_Keycode> ballKBControls = {
			{CSE::Commands::KBCommand_Left, SDLK_LEFT}, 
			{CSE::Commands::KBCommand_Right, SDLK_RIGHT}
		};
		
		CSE::KeyBoardComponent& keyboard = ball->AddComponent<CSE::KeyBoardComponent>(ballKBControls);
		
		sprite = new CSE::Texture("./App/Sprites.png", GetWindow()->GetRenderer(), {0, 0, 0});
		CSE::SpriteComponent& spriteComponent = ball->AddComponent<CSE::SpriteComponent>(sprite);
		CSE::AnimationComponent& animationComponent = ball->AddComponent<CSE::AnimationComponent>();
		animationComponent.Add(
			CSE::EntityStates::IDLE, 
			new CSE::AnimationFrames( {80, 0}, {159, 29}, 20, 29, 4.0f, true)
			// new CSE::AnimationFrames( {0, 0}, {79, 19}, 20, 20, 4.0f, true)
			);
		animationComponent.Set(CSE::EntityStates::IDLE);
		animationComponent.Start();
		// LoadScene(scene);

		return true;
	}
	
	bool OnDisplay()
	{
		m_Scene->Update(CSE::Platform::GetTimeMs());
		
		return true;
	}
	
	bool OnDetach()
	{
		UnloadScene(m_Scene);
		
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
					LoadScene(scene);
				} else {
					if ((m_Scene == scene) && (scene != nullptr))
					{
						UnloadScene(scene);
						LoadScene(sceneLogo);
					}
				}
			}
			
			if (ball->GetComponent<CSE::KeyBoardComponent>().controls[CSE::Commands::KBCommand_Left] == event->key.keysym.sym)
			{
				ball->GetComponent<CSE::PositionComponent>().x -= 0.05f;
			}
			if (ball->GetComponent<CSE::KeyBoardComponent>().controls[CSE::Commands::KBCommand_Right] == event->key.keysym.sym)
			{
				ball->GetComponent<CSE::PositionComponent>().x += 0.05f;
			}
		}
		
		return true; // "true" means that event doesn't move further 
	}
	
	CSE::Scene* sceneLogo = nullptr;
	CSE::Texture* spriteLogo = nullptr;
	CSE::Entity* logo = nullptr;
	
	CSE::Scene* scene = nullptr;
	CSE::Texture* sprite = nullptr;
	CSE::Entity* ball = nullptr;
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
