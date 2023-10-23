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
		scene = new CSE::Scene();
		LoadScene(scene);
		
		// a ball entity on the screen
		ball = scene->CreateEntity("Ball");
		sprite = new CSE::Texture("./App/Sprites.png", GetWindow()->GetRenderer());
		CSE::SpriteComponent& spriteComponent = ball->AddComponent<CSE::SpriteComponent>(sprite);
		CSE::AnimationComponent& animationComponent = ball->AddComponent<CSE::AnimationComponent>();
		animationComponent.Add(
			CSE::EntityStates::IDLE, 
			// new CSE::AnimationFrames( {80, 0}, {99, 29}, 20, 20, 4.0f, true)
			new CSE::AnimationFrames( {0, 0}, {79, 19}, 20, 20, 4.0f, true)
			);
		animationComponent.Set(CSE::EntityStates::IDLE);
		animationComponent.Start();
		
		return true;
	}
	
	bool OnDisplay()
	{
		CSE::AnimationComponent& animationComponent = ball->GetComponent<CSE::AnimationComponent>();
		auto* frameset = animationComponent.frames[animationComponent.currentAnimation];
		
		if (!animationComponent.paused)
		{
			uint64_t timeTemp = CSE::Platform::GetTimeMs();
			if ((timeTemp - animationComponent.timeBefore) >= frameset->timeBetweenFrames)
			{
				animationComponent.timeBefore = timeTemp;
				animationComponent.currentFrame++;
				if (animationComponent.currentFrame == animationComponent.framesTotal){
					if (frameset->loop)
					{
						animationComponent.currentFrame = 0;
					} else {
						animationComponent.currentFrame--;
						animationComponent.paused = true;
					}
				}
			}
		}
		
		SDL_Rect frame = 
		{
			frameset->begin.x + (frameset->width * animationComponent.currentFrame),
			frameset->begin.y,
			std::abs(frameset->width),
			std::abs(frameset->height)
		};
		
		SDL_FRect place = {50, 50, frame.w, frame.h};
		
		CSE::Renderer::DrawTexture(
			sprite->GetTexture(),
			&place,
			&frame,
			m_Window->GetScale().x,
			m_Window->GetScale().y
			);
		
		return true;
	}
	
	bool OnDetach()
	{
		UnloadScene(scene);
		
		return true;
	}
	
	bool OnEvent(SDL_Event* event)
	{
		// CSE_LOG("Layer ", GetName(), " stopped event: ", event->type);
		return true; // "true" means that event doesn't move further 
	}
	
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
		
		GetWindows().Push(new CSE::Window({"CSE: Тест", 100, 100, 320, 240}));
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
}
