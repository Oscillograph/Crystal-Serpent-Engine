#ifndef CSE_CANBAN_H
#define CSE_CANBAN_H

#include <CSE/core.h>

#include <CSE/systems/entity.h>

namespace CSE
{
	enum class CanbanEvents
	{
		None				= 0,
		Clear				= 1,
		Physics_ChangeType	= 2,
	};
	
	struct Canban
	{
		static std::unordered_multimap<CanbanEvents, Entity*> board;
		
		Canban();
		~Canban();
		
		bool GetTask(CanbanEvents event, Entity* entity);
		void PutTask(CanbanEvents event, Entity* entity);
		void Clear();
	};
}

#endif
