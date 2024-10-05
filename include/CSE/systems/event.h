#ifndef CSE_EVENT_H
#define CSE_EVENT_H

#include <CSE/systems/platform.h>

namespace CSE
{	
	typedef SDL_Event Event;
	
	// IMPORTANT: 
	// EventManager uses SDL_WaitEvent() and SDL_PollEvent() functions which require that
	// they are to be used in a thread with an initialized video system which in its turn
	// implies that the application object must have at least one window open.
	class Application;
	
	class EventManager
	{
	public:		
//		static Event* NewEvent(); // create a custom event
//		static Event* GetEvent(); // look for an event in an events queue
		
		static void Init(Application* app = nullptr);
		static void Shutdown();
		static void SendEvents(); // send events to the application
		static void SendEvent(Event* event); // send one particular event to the application
		static void SetWaitForEvents(bool waitForEvents = true); // to lower down fps
		
	private:
		static bool m_WaitForEvents; // don't wait by default
		static Event* m_Event; // current event
		static Application* m_Application;
	};
}

#endif
