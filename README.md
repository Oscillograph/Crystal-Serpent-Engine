# ! WORK IN PROGRESS ! #
# CRYSTAL SERPENT ENGINE #
Crystal Serpent Engine is aimed to be a game engine based on SDL.

## Features ##
+ Multiple windows support
+ 2D graphics SDL-based renderer
+ Architecture (Engine core and utils -> Application -> Window -> Layer -> Scene -> ECS)
+ Command line logger with colors
+ Support for animations in spritesheet (animation can be played backwards through a hack)
+ Support for 2D cameras and viewports to look into scenes
+ Presets for units, doodads and decorations to populate scenes
+ Simple resource manager with a garbage collector

## Project files organization ##
+ /bin - for binaries meant to be dynamically linked
+ /build - for binaries during the build process
+ /data - various assets required by the engine systems
+ /doc - documentation (TODO: use doxygen)
+ /examples - example apps using CSE
+ /include/CSE - header files
+ /release - output .exe here
+ /src/CSE - source files
+ /tests - tests (TODO: implement them)
+ /tools - various CSE-specific tools, including scene editor, asset editor, etc. (TODO: develop them)
+ /vendor - external libraries developed by someone else
+ LICENSE.md - current license is creative commons
+ NEWS.md - recent updates
+ README.md - this document

Repository development strategy is yet to be devised.

## Usage ##
The engine is designed to be used through a header file. The common way to use it looks like this:
1. Include <CSE/cse.h>
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
		// Create a window
		m_WindowStack.Push(new CSE::Window({"CSE: Тест", 100, 100, 320, 240}));
		
		// Attach a layer to the window
		AttachLayer(m_WindowStack.GetContents()[0], new Layer1());
		
		LimitFPS(CSE_FPS60);
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

A precompiled header can dramatically decrease compilation time:
Crystal Serpent Engine/CSE/common.h

## Tech Stack ##
+ **C++17** (almost everything)
+ **[SDL2](https://github.com/libsdl-org/SDL)** (windowing, events listening, rendering, sounds)
+ **[GLM](https://github.com/g-truc/glm)** (math library)
+ **[EnTT](https://github.com/skypjack/entt)** (entities and components management in scenes)
+ **[Color Console](https://github.com/aafulei/color-console)** (Windows-only)
+ **[Box2D](https://github.com/erincatto/box2d)** (basic 2D physics)
