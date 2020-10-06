//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (C) Gamer Camp / Alex Darby 2018
//// Distributed under the MIT license - see readme.md
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#ifndef _GCOBJPLAYER_H_
//#define _GCOBJPLAYER_H_
//
//#ifndef _GCOBJSPRITEPHYSICS_H_
//	#include "../../GCCocosInterface/GCObjSpritePhysics.h"
//#endif
//
//
////////////////////////////////////////////////////////////////////////////
//// forward declare
//class CGCObjGroupProjectilePlayer;
//template< typename TActionType > class TGCActionToKeyMap;
//
//// enum of user defined input actions the class
//enum EPlayerActions
//{
//	EPA_AxisMove_X,
//	EPA_AxisMove_Y,
//	EPA_Jump
//};
//
//enum class EPlayerDirection
//{
//	EPD_Static,
//	EPD_Right,
//	EPD_Left,
//	EPD_Jumping,
//	EPD_Falling
//};
//
//
//class CGCObjPlayer 
//: public CGCObjSpritePhysics
//{
//private:
//
//	// Constant values
//
//	const int	     m_kiStartingLives = 3;          // The starting life of the player, changing this will apply it to m_iLives on Construction
//	const float	     m_kfGravitionalPull = 30.0f;    // The gravitional force that affects the player for jumping purpouses
//
//	// Our Movement Related variables
//
//	EPlayerDirection m_ePlayerDirection;			 // This stores the current direction the player is at
//	EPlayerDirection m_eLastPlayerDirection;		 // This stores the last direction of the player, used for the jump lock
//
//	bool m_bCanJump;								 // This regulates the player's ability to jump again
//	bool m_bCanBeControlled;						 // This disables input of the X-axis directional movement of the player while jumping (or on a conveyor belt)
//
//	// TODO: Turn these to const Vectors as they are not modified during runtime
//
//	cocos2d::Vec2 m_v2MovementLeft;					 // Left Movement Speed
//	cocos2d::Vec2 m_v2MovementRight;				 // Right Movement Speed
//	cocos2d::Vec2 m_v2MovementStatic;				 // Static Movement Speed (none)
//	cocos2d::Vec2 m_v2JumpStatic;					 // Static Jump Height (none)
//	cocos2d::Vec2 m_v2JumpRight;				     // Right Jump Height
//	cocos2d::Vec2 m_v2JumpLeft;					     // Left Jump Height
//
//	// Other values
//
//	int m_iLives;									 // The current life of the player
//
//	// The controller
//
//	TGCActionToKeyMap< EPlayerActions >* m_pcControllerActionToKeyMap;
//
//public:
//	CGCObjPlayer();
//
//	//////////////////////////////////////////////////////////////////////////
//	// we need a virtual destructor since delete will be called on pointers of 
//	// this class to delete derived types.
//	virtual ~CGCObjPlayer()
//	{}
//
//
//	//////////////////////////////////////////////////////////////////////////
//	// overridden virtuals from the game object interface
//
//		// This will be called exactly once for each CGCObject derived class instance 
//		// registered with CGCObjectManager as soon as the TGB level file has stopped 
//		// loading- it is an "explicit constructor".
//		virtual void VOnResourceAcquire( void );
//
//		// OnReset is called for each CGCObject derived class when CGCObjectManager is 
//		// reset
//		virtual void VOnReset( void ); 
//
//		// OnUpdate is called for each CGCObject derived class when CGCObjectManager is 
//		// updated in t2dSCeneGraph onProcessTick()
//		virtual void VOnUpdate( f32 fTimeStep ); 
//
//        // called immediately before the managing object group releases its own assets
//        virtual void VOnResourceRelease( void );
//
//
//	// updates the movement of the CCSprite
//	void UpdateMovement( f32 fTimeStep );
//
//	void KeyboardInput();
//
//
//
//
//	//Getters and setters
//	//For platform collisions:: CanJump bool
//	bool GetCanJump() const		    {	  return m_bCanJump;	};
//	void SetCanJump( bool canJump ) {	  m_bCanJump = canJump; };
//
//	// For GameState related things:: Player lives
//	// Additionally included pre-made increment/decrement functions
//	int GetLives() const			{     return m_iLives;      };
//	void SetLives( int lives )		{     m_iLives = lives;     };
//
//	void IncrementLives()			{	  m_iLives++;		    };
//	void DecrementLives()			{	  m_iLives--;			};
//
//
//	// Returns the current movement direction of the player
//	EPlayerDirection GetDirection()					            { return m_eLastPlayerDirection; };
//
//	// Avoid calling SetDirection unless you absolutely must do something to the player.
//	// If you need this for conveyor belts, please use ConveyorBeltMovement.
//	void SetDirection(EPlayerDirection lastDirection)           { m_ePlayerDirection = lastDirection; };
//
//
//	void ConveyorBeltMovement(EPlayerDirection directionLock)   { m_ePlayerDirection = directionLock; m_bCanBeControlled = false; };
//	void EndConveyorBeltMovement()								{ m_bCanBeControlled = true; };
//
//
//};
//#endif // #ifndef _GCOBJPLAYER_H_