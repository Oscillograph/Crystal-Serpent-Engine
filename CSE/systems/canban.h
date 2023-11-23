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
		None				= 0,
		Clear				= 1,
		Physics_ChangeType	= 2,
	};
	
	struct Canban
	{
		static std::unordered_multimap<CanbanEvents, Entity*> board;
		
		static bool GetTask(CanbanEvents event, Entity* entity);
		static void PutTask(CanbanEvents event, Entity* entity);
		static void Clear();
	};
}

#endif
