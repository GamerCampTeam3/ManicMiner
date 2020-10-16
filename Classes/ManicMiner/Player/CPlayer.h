#ifndef _CPLAYER_H_
#define _CPLAYER_H_

#ifndef _GCOBJSPRITEPHYSICS_H_
#include "../../GCCocosInterface/GCObjSpritePhysics.h"
#endif

#include "ManicMiner/Enums/EPlayerMovement.h"

//////////////////////////////////////////////////////////////////////////
// forward declare

class CManicLayer;

template< typename TActionType > class TGCActionToKeyMap;

// enum of user defined input actions the class
enum EPlayerActions
{
	EPA_AxisMove_X,
	EPA_AxisMove_Y,
	EPA_Jump
};


class CPlayer
	: public CGCObjSpritePhysics
{
private:

	// Constant values
	const float								m_kfGravitionalPull = 20.0f;   // The gravitational force that affects the player for jumping purposes

	// Our Movement Related variables
	EPlayerDirection						m_ePlayerDirection;				// This stores the current direction the player is at
	EPlayerDirection						m_eLastPlayerDirection;			// This stores the last direction of the player, used for the jump lock

	bool									m_bCanJump;						// This regulates the player's ability to jump again
	bool									m_bCanBeControlled;				// This disables input of the X-axis directional movement of the player while jumping (or on a conveyor belt)
	bool									m_bIsOnLadder;
	bool									m_bIsAlive;

	cocos2d::Vec2							m_v2Movement;					// Used to move the player
	cocos2d::Vec2							m_v2Jump;						// Used for the jump
	float									m_fMovementSpeed;
	float									m_fJumpHeight;

	// Life logic
	int										m_iMaxLives;					// The maximum life of the player
    int										m_iLives;						// The current life of the player

	// Pointers
	CManicLayer*							m_pcManicLayer;
	TGCActionToKeyMap< EPlayerActions >*	m_pcControllerActionToKeyMap;

public:
	CPlayer();
	CPlayer( CManicLayer &cLayer, const cocos2d::Vec2& startingPos);
	CPlayer( CManicLayer &cLayer, const cocos2d::Vec2& startingPos,const int startingLives);

	virtual ~CPlayer()	{}


	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	//////////////////////////////////////////////////////////////////////////
	// This will be called exactly once for each CGCObject derived class instance 
	// registered with CGCObjectManager as soon as the TGB level file has stopped 
	// loading- it is an "explicit constructor".
	virtual void VOnResourceAcquire(void);
	virtual void VOnReset(void);
	virtual void VOnUpdate(f32 fTimeStep);
	virtual void VOnResourceRelease(void);
	//////////////////////////////////////////////////////////////////////////


	// Update and movement functions
	void UpdateMovement			(f32 fTimeStep);
	void KeyboardInput			();
	void ApplyDirectionChange	( const EPlayerDirection newDirection, const float xOffSet, const float yOffSet);
	void JumpEvent				( const float x, const float y );

	// Damage / Death logic
	void TakeDamage();

	//Getters and setters
	bool GetCanJump			() const			{ return m_bCanJump; }
	void SetCanJump			(bool canJump)		{ m_bCanJump = canJump; }
	bool GetCanBeControlled	() const			{ return m_bCanBeControlled; }
	void SetCanBeControlled	(bool canControl)	{ m_bCanBeControlled = canControl; }

	

	// For GameState related things:: Player lives
	// Additionally included pre-made increment/decrement functions
	int	 GetLives		() const	{ return m_iLives; }
	void SetLives		(int lives) { m_iLives = lives; }
	int  GetMaxLives	() const	{ return m_iMaxLives; }

	void IncrementLives	()			{ m_iLives++; }
	void DecrementLives	()			{ m_iLives--; }




	// Avoid calling SetDirection unless you absolutely must do something to the player.
	// If you need this for conveyor belts, please use ConveyorBeltMovement.
	void SetDirection			(EPlayerDirection lastDirection){ m_ePlayerDirection = lastDirection; }

	void ConveyorBeltMovement	(EPlayerDirection xAxisLock)	{ m_ePlayerDirection = xAxisLock; m_bCanBeControlled = false; }
	void EndConveyorBeltMovement()								{ m_bCanBeControlled = true; }

	void MountedLadder			(EPlayerDirection yAxisLock)	{ m_ePlayerDirection = yAxisLock; m_bIsOnLadder = true; }
	void UnMountedLadder		()								{ m_bIsOnLadder = false; }

	// Returns the current movement direction of the player
	EPlayerDirection GetDirection() { return m_eLastPlayerDirection; };
};
#endif // #ifndef _CPLAYER_H_