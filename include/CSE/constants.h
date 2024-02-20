#ifndef CSE_CONSTANTS_H
#define CSE_CONSTANTS_H

namespace CSE
{
	namespace Constants
	{
		const float PI = 3.14159265f;
		const float PI_Half = PI/2;
		const float SQRT2 = 35/49; // rough approximation of a square root of 2
	}
	
	// All possible entity states to check
	// use in animations
	namespace EntityStates
	{
		typedef enum {
			// pour some syntax sugar on meh
			BEGIN		= 0,
			
			NONE		= 1, // this should be set  in stateStack component constructor
			BIRTH		= 2,
			IDLE		= 3,
			
			// one state/animation is simply not enough
			ATTACK		= 4,
			ATTACK1		= 5, 
			ATTACK2		= 6,
			ATTACK3		= 7,
			
			// one state/animation is simply not enough
			CAST		= 8,
			CAST1		= 9,
			CAST2		= 10,
			CAST3		= 11,
			
			// one state/animation is simply not enough
			STAND		= 12,
			STAND1		= 13,
			STAND2		= 14,
			STAND3		= 15,
			
			CROUCH		= 16,
			
			// one state/animation is simply not enough
			WALK		= 17,
			WALK1		= 18, 
			WALK2		= 19,
			WALK3		= 20,
			
			// one state/animation is simply not enough
			RUN			= 21,
			RUN1		= 22,
			RUN2		= 23,
			RUN3		= 24,
			
			// one state/animation is simply not enough
			JUMP		= 25,
			JUMP1		= 26, // first, they jump
			JUMP2		= 27, 
			JUMP3		= 28,
			
			// one state/animation is simply not enough
			FLY			= 29,
			FLY1		= 30, // then, they fly
			FLY2		= 31,
			FLY3		= 32,
			
			// one state/animation is simply not enough
			FALL		= 33,
			FALL1		= 34, // finally, they fall to somewhere
			FALL2		= 35,
			FALL3		= 36,
			
			// one state/animation is simply not enough
			HIT			= 37,
			HIT1		= 38, // fallen/other state
			HIT2		= 39,
			HIT3		= 40,
			
			DAMAGED		= 41,
			STUNNED		= 42,
			SLEEP		= 43,
			DEATH		= 44,
			CORPSE		= 45,
			RESURRECT	= 46, // Death is only the beginning (c) Imhotep, "Mummy"
			REMOVE		= 47, // this should force entity removal
			
			// pour some syntax sugar on meh
			TOTAL		= 48
		} EntityState;
	}
	
	// All possible entity types related to objects on a scene
	namespace GameObjectTypes
	{
		typedef enum {
			// pour some syntax sugar on meh
			BEGIN,
			
			OBJECT,	// an object with an unpredicted type
			DECORATION,// a tile, an image, a background
			DOODAD,	
			PROPS,
			UNIT,		// characters are units!
			TEXT,
			
			// pour some syntax sugar on meh
			TOTAL
		} GameObjectType;
	}
	
	// all possible 2D hitboxes
	namespace HitBoxType
	{
		typedef enum {
			// pour some syntax sugar on meh
			BEGIN,
			
			POINT,
			LINE,
			CIRCLE,
			TRIANGLE,
			RECTANGLE,
			POLYGON,
			
			// pour some syntax sugar on meh
			TOTAL
		} GeometryHitBox;
	}
	
	// Keyboard and Mouse commands to store somewhere
	namespace Commands 
	{
		typedef enum {
			// pour some syntax sugar on meh
			KBCommand_BEGIN,
			
			// common movement
			KBCommand_Up,
			KBCommand_Down,
			KBCommand_Left,
			KBCommand_Right,
			KBCommand_Jump,
			KBCommand_Flip,
			KBCommand_Run,
			KBCommand_Walk,
			KBCommand_Crouch,
			KBCommand_Teleport,
			
			// common action
			KBCommand_Hit,
			KBCommand_Dodge,
			KBCommand_Shield,
			KBCommand_Fire,
			
			// common adventure
			KBCommand_Talk,
			KBCommand_Collect,
			KBCommand_Use,
			KBCommand_Cast,
			KBCommand_Select,
			KBCommand_Switch,
			
			// common interface
			KBCommand_JournalOpen,
			KBCommand_JournalClose,
			KBCommand_SpellsOpen,
			KBCommand_SpellsClose,
			KBCommand_InventoryOpen,
			KBCommand_InventoryClose,
			KBCommand_MapOpen,
			KBCommand_MapClose,
			KBCommand_TextInputStart,
			KBCommand_TextInputStop,
			
			// misc
			KBCommand_Space,
			KBCommand_Enter,
			KBCommand_Pause,
			
			// app
			KBCommand_Quit,
			
			// pour some syntax sugar on meh
			KBCommand_TOTAL
		} KeyBoardCommands;
	}
}

#endif
