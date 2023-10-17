#include "./cse.h"


class Layer1 : public CSE::Layer
{
public:
	Layer1()
	: CSE::Layer("#1")
	{};
	
	~Layer1() {};
};

class Layer2 : public CSE::Layer
{
public:
	Layer2()
	: CSE::Layer("#2")
	{};
	
	~Layer2() {};
};

class App : public CSE::Application
{
public:
	App(const CSE::WindowPrefs& prefs)
		: CSE::Application(prefs) 
	{
		AttachLayer(new Layer1());
		AttachLayer(new Layer2());
	};
	~App() {};
private:
	
};

CSE::Application* CSE::CreateApplication()
{
	return new App({"My First App", 100, 100, 400, 400, 1});
}
