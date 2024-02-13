#include <CSE/systems/canban.h>

namespace CSE
{
	std::unordered_multimap<CanbanEvents, Entity*> Canban::board;
	std::vector<CanbanEvent> Canban::advancedBoard;
	
	bool Canban::GetTask(CanbanEvents event, Entity* entity)
	{
		auto it = board.find(event);
		if (it != board.end())
		{
			entity = it->second;
			board.erase(it);
			return true;
		}
		return false;
	}
	
	void Canban::PutTask(CanbanEvents event, Entity* entity)
	{
		board.insert({event, entity});
	}
	
	void Canban::Clear()
	{
		auto it = board.begin();
		
		// first, make sure that we won't accidentally call entities destructors
		for (; it != board.end(); it++)
		{
			it->second = nullptr;
		}
		
		board.clear();
	}
}
