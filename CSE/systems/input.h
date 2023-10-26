#ifndef CSE_INPUT_H
#define CSE_INPUT_H

#include <CSE/core.h>
#include <CSE/systems/platform.h>
#include <CSE/systems/input/codes.h>

namespace CSE
{
	class Input
	{
	public:
		static bool IsButtonPressed(int scancode);
		
	private:
		// TODO: think about two arrays for keyboard key states: 
		// one array as a base to match with
		// one array to store actual key states
		// then the bitwise OR gives the program the information what keys are pressed right now.
	};
}

#endif
