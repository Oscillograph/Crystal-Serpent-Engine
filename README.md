# ! WORK IN PROGRESS ! #
# CRYSTAL SERPENT ENGINE #
Crystal Serpent Engine v0.2.1 is aimed to be a game engine built on SDL.

The engine is designed to be used a header file. The common way to use it looks like this:
```
#include "./cse.h"

int main (int argv, char** argc)
{
	CSE_CORE_LOG("This is an engine logger.");
	CSE_LOG("And this is an app logger.");
	
	// create a singleton app
	auto* app = new CSE::Application({"My First App", 100, 100, 400, 400, 1});
	app->Run(); // run its game cycle
	
	delete app;
	return 0;
}
```

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
+ **Dear ImGui** (OpenGL GUI library)
+ **YaML** (objects serialization and deserialization)
