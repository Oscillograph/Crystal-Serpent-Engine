#ifndef CSE_COMPONENTS_STATES_H
#define CSE_COMPONENTS_STATES_H

#include <CSE/core.h>

namespace CSE
{
	// TODO: Redesign it to be a bitmask defined by CSE/constants.h
	// But remember: 64 bit (8 bytes per state) is the max, so 
	// 				 we can afford only 64 different states to set per entity.
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
		State(int value);
		State(const State&) = default;
		
		~State();
		
		// check whether we can enter the state
		void OnEnter();
		// set up the state
		void Set();
		// check whether we can exit the state
		void OnExit();
		
		// concept: block if not in the list
		void AllowEntryFrom(int state);
		bool IsAllowedEntryFrom(int state);
		void BlockEntryFrom(int state); // check in the list, remove if found
		
		void AllowExitTo(int state);
		bool IsAllowedExitTo(int state);
		void BlockExitTo(int state); // check in the list, remove if found
		
		/*
		// comparing states
		bool operator==(State* other) 
		{
			return (this->data == other->data);
		};
		bool operator!=(State* other) 
		{
			return !(this == other);
		};
		bool operator==(int other) 
		{
			return (this->data == other);
		};
		bool operator!=(int other) 
		{
			return (this->data != other);
		};
		bool operator==(EntityStates::EntityState other) 
		{
			return (this->data == other);
		};
		bool operator!=(EntityStates::EntityState other) 
		{
			return (this->data != other);
		};
		*/
		
		// data
		int data; // see EntityState
		
	private:
		std::vector<int> m_AllowEntry;
		std::vector<int> m_AllowExit;
	};
}

#endif
