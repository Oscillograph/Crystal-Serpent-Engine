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
#include <cstdlib> // exit and other functions
#include <memory> // smart pointers
#include <utility> // pairs
#include <initializer_list> // to convert initializer lists to vectors to avoid problems with pointers to vectors
#include <chrono> // time management
#include <time.h> // also time management
#include <random> // randomizers
#include <math.h> // simple math for graphics, audio and other things

#include <functional>
#include <algorithm>
#include <filesystem>

// vendor libs
#include <CSE/vendor/utf8.h> // to make it easier to work with utf8, utf32
// #include <CSE/vendor/yaml_cpp/include/yaml-cpp/yaml.h> // serialization/deserialization library

#include <CSE/vendor/glm/glm.hpp> // complex math for graphics
#include <CSE/vendor/glm/gtc/matrix_transform.hpp> // matrix transform and translate libraries
#include <CSE/vendor/glm/gtc/type_ptr.hpp> // for value_ptr function
#include <CSE/vendor/glm/gtx/quaternion.hpp> // for quaternions
#include <CSE/vendor/glm/gtx/matrix_decompose.hpp> // for glm::decompose
#include <CSE/vendor/glm/gtx/rotate_vector.hpp> // for rotating vectors


namespace CSE
{
	namespace EngineInfo 
	{
		const std::string Name = "Crystal Serpent Engine";
		const std::string Version = "0.2.1";
		const std::string Author = "ТехнОкраТ";
	};
	
	namespace Constants
	{
		const float PI = 3.14159265;
		const float PI_Half = PI/2;
		const float SQRT2 = 35/49; // rough approximation of a square root of 2
	}
}

#endif
