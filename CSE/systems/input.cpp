#include <CSE/systems/input.h>

namespace CSE
{
	std::vector<InputKeyEvent> Input::m_EventsFIFO = {};
	int Input::m_CurrentEvent = 0; // unused
	
	bool Input::IsButtonPressed(int scancode)
	{
		const uint8_t* keyBoardState =  SDL_GetKeyboardState(nullptr);
		if (keyBoardState[scancode])
			return true;
		return false;
	}
	
	bool Input::HasEvents()
	{
		return (m_EventsFIFO.size() > 0);
	}
	
	InputKeyEvent Input::PeekEvent()
	{
		InputKeyEvent event;
		if (m_EventsFIFO.size() > 0)
		{
			event = *(m_EventsFIFO.begin());
		} else {
			event = InputKeyEvent();
		}
		return event;
	}
	
	InputKeyEvent Input::PopEvent()
	{
		InputKeyEvent event;
		if (m_EventsFIFO.size() > 0)
		{
			event = *(m_EventsFIFO.begin());
			m_EventsFIFO.erase(m_EventsFIFO.begin());
		} else {
			event = InputKeyEvent();
		}
		return event;
	}
	
	void Input::PutEvent(const InputKeyEvent& event)
	{
		m_EventsFIFO.push_back(event);
	}
}
