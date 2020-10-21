#ifndef _CPLAYER_H_
#define _CPLAYER_H_

#ifndef _GCOBJSPRITEPHYSICS_H_
#include "../../GCCocosInterface/GCObjSpritePhysics.h"
#endif

#include "ManicMiner/Enums/EPlayerMovement.h"
//#include "ManicMiner/Enums/EPlayerActions.h"


//////////////////////////////////////////////////////////////////////////
// forward declare

class CManicLayer;

template< typename TActionType > class TGCActionToKeyMap;

// enum of user defined input actions the class
enum  EPlayerActions
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
	/// Henrique note: this value isnt changing anything
	const float								m_kfGravitionalPull = 8.0f;   // The gravitational force that affects the player for jumping purposes

	// Our Movement Related variables
	EPlayerDirection						m_ePlayerDirection;				// This stores the current direction the player is at
	EPlayerDirection						m_eLastPlayerDirection;			// This stores the last direction of the player, used for the jump lock

	bool									m_bCanJump;						// This regulates the player's ability to jump again
	bool									m_bCanBeControlled;				// This disables input of the X-axis directional movement of the player while jumping (or on a conveyor belt)
	bool									m_bIsOnLadder;
	bool									m_bIsGrounded;					// Is walking on platform?
	bool									m_bIsAlive;

	float									m_fMovementSpeed;
	float									m_fJumpForce;
	cocos2d::Vec2							m_v2Movement;					// Used to move the player

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
	virtual void VOnResurrected(void) override;
	virtual void VOnUpdate(f32 fTimeStep);
	virtual void VOnResourceRelease(void);
	//////////////////////////////////////////////////////////////////////////


	// Update and movement functions
	void UpdateMovement			();
	void KeyboardInput			();
	void ApplyDirectionChange	( EPlayerDirection eNewDirection, float fHorizontalVelocity, float fVerticalVelocity );
	void JumpEvent				();

	// Damage / Death logic
	void Die();

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
	bool GetIsAlive		()			{ return m_bIsAlive; }

	void IncrementLives	()			{ m_iLives++; }
	void DecrementLives	()			{ m_iLives--; }
	bool IsInMidAir()				{ return (m_ePlayerDirection == EPlayerDirection::EPD_Jumping && !m_bIsGrounded); }
	float GetMovementSpeed() { return m_fMovementSpeed; }


	// Avoid calling SetDirection unless you absolutely must do something to the player.
	// If you need this for conveyor belts, please use ConveyorBeltMovement.
	void SetDirection			(EPlayerDirection lastDirection){ m_ePlayerDirection = lastDirection; }

	// Called when landing on top of a platform surface, enables player control and movement
	void LandedOnWalkablePlatform();



	void ConveyorBeltMovement( EPlayerDirection xAxisLock );
	void EndConveyorBeltMovement()								{ m_bCanBeControlled = true; }

	void MountedLadder			(EPlayerDirection yAxisLock)	{ m_ePlayerDirection = yAxisLock; m_bIsOnLadder = true; }
	void UnMountedLadder		()								{ m_bIsOnLadder = false; }

	// Returns the current movement direction of the player
	EPlayerDirection GetDirection() { return m_ePlayerDirection; };
};
#endif // #ifndef _CPLAYER_H_