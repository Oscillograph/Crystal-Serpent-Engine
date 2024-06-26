## 0.4.0 ##
Changelog:
+ Changed the architecture vision: Scenes should be as independent objects as windows;

## 0.3.0 ##
Changelog:
+ Changed the project files organization structure to match "common" style;

## 0.2.4 ##
Changelog:
+ Started development of a custom math library from a simple vec3 struct;

## 0.2.3 ##
Changelog:
+ Moved LayerStack to the Window class so that each window can have its own pack of layers;
+ Changed Layer pointers to shared pointers across application, window and layer files;
+ Support for multiple windows;
+ Added Scenes abstraction which works with Entities, Components and entities registry by EnTT library;
+ Added header constants.h to CSE folder to hoard all enums and predefined variables;
+ Expanded Components with code from SDLCSE v0.1.1-2 to support sprites, animations and controls;
+ Added Scene load/unload mechanic;
+ Added layers display mechanism (and tested with AnimationComponent);
+ Scenes Update routine automates work with animations and draw calls;
+ Custom Input class and keycode/scancode predefines support;
+ Made sure that inverted animations supported;
+ Renderer supports tiling;
+ Engine draws Wireframes if necessary to debug rendering;
+ Each layer supports up to one viewport to look at a scene contents;
+ Added Box2D as one of physics processor implementations;
+ Scenes support cameras;
+ Added presets library to set up scenes via units, doodads and decorations one-line constructors;
+ Added a simple resource manager static class with a garbage collector;

## 0.2.2 ##
Changelog:
+ Separated SDL platform from Application and Window classes;
+ Added a Renderer abstraction;
+ Added layers and LayerStack abstractions to organize window content;
+ Changed the engine entry point so that a custom app class can be set up;
+ Due to the previous change, the function int main() moved into cse/application.cpp;
+ Reimplemented Texture class from CSE v0.1.1;
+ Renderer can display textures now;
+ FPS limit;
+ Window title can be changed;
+ Added to assets folder: CSE logo, SDL logo;
+ Reorganized files in CSE folder.

## 0.2.1 ##
Made a decision to rework the engine completely, using my new bag of experience gained while redeveloping The Cherno's Hazel2D.

Currently there is:
+ Initial application class;
+ Initial window class (which initializes SDL);
+ SDL initialization (core system, window, renderer, IMG, Mixer, events listener);
+ Windows-compatible command line Logger (uses color.hpp library from https://github.com/aafulei/color-console);

Potential:
+ multiple windows support by design


