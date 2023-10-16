#include "./cse.h"

int main (int argv, char** argc)
{
	std::cout << "Hello, world!\n";
	
	CSE_CORE_LOG("This is an engine logger.");
	CSE_LOG("And this is an app logger.");
	
	auto* app = new CSE::Application({"My First App", 100, 100, 400, 400, 1});
	CSE_LOG("App set up.");
	app->Run();
	CSE_LOG("Exit from App main loop.");
	delete app;
	CSE_LOG("App logger out.");
	
	return 0;
}
