#include "./cse.h"

// Layers of the main (default) window which is created with the application start
class Layer1 : public CSE::Layer
{
public:
	Layer1()
	: CSE::Layer("Game")
	{};
	
	~Layer1() {};
	
	bool OnEvent(SDL_Event* event)
	{
		CSE_LOG("Layer ", GetName(), " stopped event: ", event->type);
		return true;
	}
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
		CSE_LOG("Layer ", GetName(), " stopped event: ", event->type);
		return true;
	}
};

class App : public CSE::Application
{
public:
	App(const CSE::WindowPrefs& prefs)
	: CSE::Application(prefs) 
	{
		AttachLayer(m_Window, new Layer1());
		AttachLayer(m_Window, new Layer2());
		LimitFPS(CSE_FPS60);
	};
	~App() {};
};

CSE::Application* CSE::CreateApplication()
{
	return new App({"CSE: Тест", 100, 100, 320, 240});
}
