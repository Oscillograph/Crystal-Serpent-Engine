# ! WORK IN PROGRESS ! #
# CRYSTAL SERPENT ENGINE #
Crystal Serpent Engine v0.2.1 is aimed to be a game engine built on SDL.

The engine is designed to be used through a header file. The common way to use it looks like this:
1. Include "./cse.h"
2. Set up layers:
```
class Layer1 : public CSE::Layer
{
public:
	Layer1()
	: CSE::Layer("#1")
	{};
	
	~Layer1() {};
};
```
3. Set up the app class:
```
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
};
```
4. Set up the starting window:
```
CSE::Application* CSE::CreateApplication()
{
	return new App({"My First App", 100, 100, 400, 400, 1});
}
```
5. That's it! You're ready to create.

## Build notes ##
The engine is built using TDM-GCC via Red Panda C++ IDE.

Linker options:
```
-lmingw32_@@_-lSDL2main_@@_-lSDL2_@@_-lSDL2_image_@@_-lSDL2_mixer
```
(Obviously, there should be compiled libraries for SDL2, SDL_image and SDL_mixer)

Includes directories should contain:
- SDL2
- Crystal Serpent Engine

A precompiled header can dramatically reduce compile time:
Crystal Serpent Engine/CSE/common.h

## Tech Stack ##
+ **C++17** (almost everything)
+ **SDL 2.28.4** (windowing, events listening, rendering, sounds)
+ **GLM** (math library)
