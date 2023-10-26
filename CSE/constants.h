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
	namespace EntityStates
	{
		typedef enum {
			// pour some syntax sugar on meh
			BEGIN,
			
			NONE, // this should be set  in stateStack component constructor
			BIRTH,
			IDLE,
			ATTACK1, // one state/animation is simply not enough
			ATTACK2,
			ATTACK3,
			CAST1, // one state/animation is simply not enough
			CAST2,
			CAST3,
			STAND1, // one state/animation is simply not enough
			STAND2,
			STAND3,
			CROUCH,
			WALK1, // one state/animation is simply not enough
			WALK2,
			WALK3,
			RUN1, // one state/animation is simply not enough
			RUN2,
			RUN3,
			JUMP, // first, they jump
			FLY, // then, they fly
			FALL, // finally, they fall to somewhere
			HIT, // fallen/other state
			DAMAGED,
			STUNNED,
			SLEEP,
			DEATH,
			CORPSE,
			RESURRECT, // Death is only the beginning (c) Imhotep, "Mummy"
			REMOVE, // this should force entity removal
			
			// pour some syntax sugar on meh
			TOTAL
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
