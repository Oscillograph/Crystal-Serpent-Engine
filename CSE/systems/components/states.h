#ifndef CSE_COMPONENTS_STATES_H
#define CSE_COMPONENTS_STATES_H

#include <CSE/core.h>

namespace CSE
{
	struct StateStruct // this is the data that defines a state
	{
		bool isStanding 	= false;
		bool isMoving 		= false;
		bool isWalking 		= false;
		bool isJumping 		= false;
		bool isFlying 		= false;
		bool isFalling 		= false;
		bool isBumping 		= false;
		bool isFighting 	= false;
		bool isHitting 		= false;
		bool isShooting 	= false;
		bool isDying 		= false;
	};
	
	class State
	{
	public:
		State();
		State(const State&) = default;
		
		~State();
		
		// check whether we can enter the state
		void OnEnter();
		// set up the state
		void Set();
		// check whether we can exit the state
		void OnExit();
		
		// concept: block if not in the list
		void AllowEntry(State* state);
		void BlockEntry(State* state); // check in the list, remove if found
		void AllowExit(State* state);
		void BlockExit(State* state); // check in the list, remove if found
		
	private:
		std::vector<State*> m_AllowEntry;
		std::vector<State*> m_AllowExit;
		StateStruct m_StateData;
	};
}

#endif
