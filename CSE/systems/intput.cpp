#include <CSE/systems/input.h>

namespace CSE
{
	bool IsButtonPressed(int scancode)
	{
		const uint8_t* keyBoardState =  SDL_GetKeyboardState(nullptr);
		if (keyBoardState[scancode])
			return true;
		return false;
	}
}
