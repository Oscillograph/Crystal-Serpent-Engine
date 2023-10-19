#ifndef CSE_COMMON_H
#define CSE_COMMON_H

// STL IO
#include <stdio.h>
#include <iostream>
#include <fstream>

// STL containers and streams
#include <string>
#include <sstream>
#include <vector> // to allocate dynamically
#include <array> // to allocate statically
#include <unordered_map>
#include <unordered_set>

// STL utilities
#include <cstdlib> // exit() and other functions
#include <memory> // smart pointers
#include <utility> // pairs
#include <initializer_list> // to convert initializer lists to vectors to avoid problems with pointers to vectors
#include <chrono> // time management
#include <time.h> // also time management
#include <random> // randomizers
#include <math.h> // simple math for graphics, audio and other things

#include <functional> // for function callbacks
#include <algorithm> // for things like find()
#include <filesystem> // for easier access to file system

// vendor libs
#include <CSE/vendor/glm/glm.hpp> // complex math for graphics
#include <CSE/vendor/glm/gtc/matrix_transform.hpp> // matrix transform and translate libraries
#include <CSE/vendor/glm/gtc/type_ptr.hpp> // for value_ptr function
#include <CSE/vendor/glm/gtx/quaternion.hpp> // for quaternions
#include <CSE/vendor/glm/gtx/matrix_decompose.hpp> // for glm::decompose
#include <CSE/vendor/glm/gtx/rotate_vector.hpp> // for rotating vectors

// defines before anything other Vendor-ish is included
// #define GLM_FORCE_CTOR_INIT // to force GLM initialize vectors and matrices

#include <CSE/vendor/utf8.h> // to make it easier to work with utf8, utf32
// #include <CSE/vendor/yaml_cpp/include/yaml-cpp/yaml.h> // serialization/deserialization library

namespace CSE
{
	namespace EngineInfo 
	{
		const std::string Name = "Crystal Serpent Engine";
		const std::string Version = "0.2.3";
		const std::string Author = "ТехнОкраТ";
	};
	
	namespace Constants
	{
		const float PI = 3.14159265;
		const float PI_Half = PI/2;
		const float SQRT2 = 35/49; // rough approximation of a square root of 2
	}
	
	// smart pointers aliases
	template <typename T>
	using Scope = std::unique_ptr<T>;
	
	template <typename T>
	using Ref = std::shared_ptr<T>;
	// Thing is, shared pointers make use of a reference counter which is incremented through
	// operator "=" and std::make_shared<T>(value) function.
	// Unique pointers are created with constructor std::unique_ptr<T> m(new T(value)).
	// They have no reference count, and they get destroyed when they are out of scope.
	// So, usefulness of these aliases is doubtful at best.
}

// useful macros
#define BIT(x) (1 << x)

// FPS macros				ms between frames 
#define CSE_FPSNOLIMIT		0.0001f
#define CSE_FPS120			0.0083f
#define CSE_FPS90			0.0111f
#define CSE_FPS60			0.0167f
#define CSE_FPS30			0.0333f
#define CSE_FPS15			0.0667f
#define CSE_FPS10			0.1f
#define CSE_FPS5			0.2f
#define CSE_FPS4			0.25f
#define CSE_FPS3			0.3333f
#define CSE_FPS2			0.5f
#define CSE_FPS1			1.0f

#endif
