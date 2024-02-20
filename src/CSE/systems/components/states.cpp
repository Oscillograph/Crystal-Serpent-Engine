#include <CSE/systems/components/states.h>

namespace CSE
{
	State::State()
	{
	}
	
	State::State(int value)
	{
		data = value;
	}
	
	State::~State()
	{
		/*
		for (int i = 0; i < m_AllowEntry.size(); i++)
		{
			m_AllowEntry[i] = nullptr;
		}
		
		for (int i = 0; i < m_AllowExit.size(); i++)
		{
			m_AllowExit[i] = nullptr;
		}
		*/
	}
	
	// check whether we can enter the state
	void State::OnEnter()
	{
	}
	// set up the state
	void State::Set()
	{
	}
	
	// check whether we can exit the state
	void State::OnExit()
	{
	}
	
	// concept: block if not in the list
	void State::AllowEntryFrom(int state)
	{
		CSE_CORE_ASSERT(!IsAllowedEntryFrom(state), "That state is already allowed to enter this one");
		m_AllowEntry.push_back(state);
	}
	
	bool State::IsAllowedEntryFrom(int state)
	{
		auto it = std::find(m_AllowEntry.begin(), m_AllowEntry.end(), state);
		if (it != m_AllowEntry.end())
			return true;
		return false;
	}
	
	void State::BlockEntryFrom(int state) // check in the list, remove if found
	{
		auto it = std::find(m_AllowEntry.begin(), m_AllowEntry.end(), state);
		if (it != m_AllowEntry.end())
		{
			m_AllowEntry.erase(it);
		} else {
			CSE_CORE_LOG("The state has not been found in Allow Entry list");
		}
	}
	
	void State::AllowExitTo(int state)
	{
		CSE_CORE_ASSERT(!IsAllowedExitTo(state), "This state is already allowed to exit to that one");
		m_AllowExit.push_back(state);
	}
	
	bool State::IsAllowedExitTo(int state)
	{
		for (auto i : m_AllowExit)
		{
			if (i == state)
				return true;
		}
		return false;
	}
	
	void State::BlockExitTo(int state) // check in the list, remove if found
	{
		auto it = std::find(m_AllowExit.begin(), m_AllowExit.end(), state);
		if (it != m_AllowExit.end())
		{
			m_AllowExit.erase(it);
		} else {
			CSE_CORE_LOG("The state has not been found in Allow Entry list");
		}
	}
}
