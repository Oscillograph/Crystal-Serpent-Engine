#ifndef CSE_INPUT_H
#define CSE_INPUT_H

#include <CSE/core.h>
#include <CSE/systems/platform.h>
#include <CSE/systems/input/codes.h>

namespace CSE
{
	enum class KeyStatus
	{
		Unknown,
		Pressed,
		Released
	};
	
	struct InputKeyEvent
	{
		InputKeyEvent() = default;
		
		int keyCode = KeyCode::Unknown;
		int scanCode = ScanCode::Unknown;
		KeyStatus status = KeyStatus::Unknown;
	};
	
	class Input
	{
	public:
		static bool IsButtonPressed(int scancode);
		
		static bool HasEvents();
		static InputKeyEvent PeekEvent(); // just look at the first unprocessed event
		static InputKeyEvent PopEvent(); // look at the first unprocessed event and remove it
		static void PutEvent(const InputKeyEvent& event); // put a new event into a stack
		
	private:
		static std::vector<InputKeyEvent> m_EventsFIFO; // the stask of events (FIFO)
		static int m_CurrentEvent; // unused
		
		// TODO: think about two arrays for keyboard key states: 
		// one array as a base to match with
		// one array to store actual key states
		// then the bitwise OR gives the program the information what keys are pressed right now.
	};
}

#endif
