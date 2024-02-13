#ifndef CSE_CANBAN_H
#define CSE_CANBAN_H

#include <CSE/core.h>

namespace CSE
{
	class Entity;
	
	
	// the pattern is
	// XXX - for Canban-specific events
	// YYY_ZZZ - ZZZ event for a system YYY
	enum class CanbanEvents
	{
		None					= 0,
		Application_Shutdown 	= 1,
		Clear					= 2,
		Physics_ChangeType		= 3,
	};
	
	enum class CanbanReceiver
	{
		None					= 0,
		Layer					= 1,
		Scene					= 2,
		Physics					= 3,
		Renderer				= 4,
		App						= 5,
	};
	
	struct CanbanEvent
	{
		CanbanEvents type = CanbanEvents::None;
		Entity* entity = nullptr;
		int keyScanCode = 0;
		int keyStatus = 0; // 1 - Up, 2 - Down
		CanbanReceiver receiver = CanbanReceiver::None;
	};
	
	struct Canban
	{
		static std::unordered_multimap<CanbanEvents, Entity*> board;
		static std::vector<CanbanEvent> advancedBoard;
		
		static bool GetTask(CanbanEvents event, Entity* entity);
		static void PutTask(CanbanEvents event, Entity* entity);
		static void Clear();
	};
}

#endif
