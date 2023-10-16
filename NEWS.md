## 0.2.2 ##
Changelog:
+ Separated SDL platform from Application and Window classes;
+ Added a Renderer abstraction;

## 0.2.1 ##
Made a decision to rework the engine completely, using my new bag of experience gained while redeveloping The Cherno's Hazel2D.

Currently there is:
+ Initial application class;
+ Initial window class (which initializes SDL);
+ SDL initialization (core system, window, renderer, IMG, Mixer, events listener);
+ Windows-compatible command line Logger (uses color.hpp library from https://github.com/aafulei/color-console);

Potential:
+ multiple windows support by design


