#include <CSE/cse.h>

// Scenes
class SceneGame : public CSE::Scene
{
public:
	SceneGame()
		: CSE::Scene(CSE::PhysicsSystem::CSE)
	{
		CSE_LOG("SceneGame init");
	};
	
	~SceneGame()
	{
		delete background;
		background = nullptr;
	}
	
	void OnInit()
	{
		if (background == nullptr)
		{
			background = new CSE::Arcade::Decoration(
				this, 
				"CSE Logotype",
				"./arcanoid/assets/background-03.jpg",
				{160, 120},
				{320, 240}
				);
		}
	}
	
	void OnUpdate(CSE::TimeType timeFrame)
	{
	}
	
private:
	CSE::Entity* background = nullptr;
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
