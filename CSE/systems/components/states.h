#ifndef CSE_COMPONENTS_STATES_H
#define CSE_COMPONENTS_STATES_H

#include <CSE/core.h>

namespace CSE
{
	struct StateStruct // this is the data that defines a state
	{
		//	 state					 // usual transitions
		bool isStanding 	= false; // -> Walking, Jumping, Hitting
		bool isMoving 		= false; // -> ???
		bool isWalking 		= false; // -> Standing, Jumping, Hitting
		bool isJumping 		= false; // -> Hitting, Flying, Falling
		bool isFlying 		= false; // -> Hitting, Falling
		bool isFalling 		= false; // -> Standing
		bool isBumping 		= false; // -> ???
		bool isFighting 	= false; // -> ???
		bool isHitting 		= false; // -> Standing, Walking, Jumping
		bool isShooting 	= false; // -> ???
		bool isDying 		= false; // -> ???
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
