#include <CSE/protocore.h>
#include <CSE/systems/event.h>
#include <CSE/systems/application.h>
#include <CSE/systems/window.h>

namespace CSE
{
	bool EventManager::m_WaitForEvents = false;
	Event* EventManager::m_Event = nullptr;
	Application* EventManager::m_Application = nullptr;
	
	
	void EventManager::Init(Application* app)
	{
		m_Application = app;
	}
	
	void EventManager::Shutdown()
	{
		m_Application = nullptr;
	}
	
	void EventManager::SendEvents()
	{
		if (m_Application != nullptr)
		{
			if (m_Application->GetWindows().Size() > 0)
			{
				if (m_WaitForEvents)
				{
					if (!SDL_WaitEvent(m_Event))
					{
						CSE_LOG("Event Manager: ", SDL_GetError());
					} else {
						SendEvent(m_Event);
					}
				} else {			
					while (SDL_PollEvent(m_Event))
					{
						SendEvent(m_Event);
					}
				}
			}
		}
	}
	
	void EventManager::SendEvent(Event* event)
	{
		// we check this type only because we need to know which window produced the event
		if (event->type == 
			(SDL_KEYDOWN || SDL_KEYUP) || // SDL_KeyboardEvent
			(SDL_TEXTEDITING || SDL_TEXTEDITING_EXT) || // SDL_TextEditingEvent & SDL_TextEditingExtEvent
			(SDL_TEXTINPUT) || // SDL_TextInputEvent
			(SDL_MOUSEMOTION) || // SDL_MouseMotionEvent
			(SDL_MOUSEBUTTONDOWN || SDL_MOUSEBUTTONUP) || // SDL_MouseButtonEvent
			(SDL_MOUSEWHEEL) || // SDL_MouseWheelEvent
			(SDL_FINGERMOTION || SDL_FINGERDOWN || SDL_FINGERUP) || // SDL_TouchFingerEvent
			(SDL_DROPBEGIN || SDL_DROPFILE || SDL_DROPTEXT || SDL_DROPCOMPLETE) || // SDL_DropEvent
			(SDL_USEREVENT) // SDL_UserEvent
			)
		{
			m_Application->EventProcessor(m_Event);
		} else {
			CSE_CORE_LOG("EventManager: Undefined event \"", event->type, "\" not sent.");
		}
	}
	
	void EventManager::SetWaitForEvents(bool waitForEvents)
	{
		m_WaitForEvents = waitForEvents;
	}
}
