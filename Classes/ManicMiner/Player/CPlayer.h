//////////////////////////////////////////////////////////////////////////////////////
// Gamer Camp 2020 / Henrique & Bib													//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _CPLAYER_H_
#define _CPLAYER_H_


#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"
#include "ManicMiner/Enums/EPlayerMovement.h"


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


	// MOVEMENT PROPERTIES	////////////////////////////////////////////////////////////////////////////////////////
	
	// Keeps track of the direction player is heading towards [ horizontal only ]
	EPlayerDirection m_ePlayerDirection;				
	
	// Pending direction, assigned by the last contacted CMovingPlatform
	EPlayerDirection m_ePendingDirection;

	// Enables / Disables ability to Jump
	// Toggled when Player lands on / leaves the ground surface
	bool m_bCanJump;						
	
	// Enables / Disables ability to change horizontal direction
	// Only set to false when Player is on top of a CMovingPlatform, and his walking direction is locked.
	bool m_bCanBeControlled;
	
	// Tells us if the Player is pending a direction change
	// We use this to allow the Player to keep walking against the CMovingPlatform
	// Until he tries to change direction. This will force the Player to go in the direction
	// Of the conveyor belt, and will set m_bCanBeControlled to true, effectively locking horizontal movement
	bool									m_bIsPendingDirection;

	// Tells us if the Player is standing on a surface
	// This implies that m_iHardContactCount > 0
	bool									m_bIsGrounded;

	// Tells us if the player is alive this frame
	// We use this to avoid multiple deaths during the same frame
	// ( might happen because of multiple collisions )
	bool									m_bIsAlive;

	// Horizontal velocity for when player is moving sideways
	float									m_fWalkSpeed;

	// Instant vertical velocity for when player jumps
	float									m_fJumpSpeed;

	// Specific gravitational acceleration for the player
	const float								m_kfGravitionalPull;
	

	// COLLISION PROPERTIES	////////////////////////////////////////////////////////////////////////////////////////

	// Expresses the current number of "Soft Contacts"
	// How many unique sensors is our "Feet" sensor overlapping with during this frame
	int										m_iSensorContactCount;
	
	// Expresses the current number of "Hard Contacts"
	// How many fixtures is our player touching during this frame
	// Does not include any sensor fixtures
	int										m_iHardContactCount;			

	// Stores the Y coordinate of when Willy left the surface
	// Functionality not yet implemented, but this will be needed for fall damage / death
	float									m_fLastYPosition;

	// Life logic
	int										m_iMaxLives;					// The maximum life of the player
    int										m_iLives;						// The current life of the player

	TGCActionToKeyMap< EPlayerActions >*	m_pcControllerActionToKeyMap;

public:
	CPlayer( const cocos2d::Vec2& startingPos);

	virtual ~CPlayer()	{}


	//////////////////////////////////////////////////////////////////////////
	// Overridden virtuals from the game object interface
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: VOnResourceAcquire																	//
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// No params																					//
	////////////////////////////////////////////////////////////////////////////////////////////////// 
	// +Info:																						//
	//																								//
	// Virtual override of CGCObjSpritePhysics::VOnResourceAcquire									//
	//																								//
	// Calls CGCObjSpritePhysics::VOnResourceAcquire and sets m_pcControllerActionToKeyMap			//
	//////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void VOnResourceAcquire(void);



	virtual void VOnResurrected(void) override;
	virtual void VOnUpdate(f32 fTimeStep);
	virtual void VOnResourceRelease(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// End of game object interface overrides														//
	//////////////////////////////////////////////////////////////////////////////////////////////////


	// Update and movement functions
	void KeyboardInput			();
	void ApplyDirectionChange	( const EPlayerDirection eNewDirection, const bool bResetVerticalVelocity = false );
	void JumpEvent				();

	// Damage / Death logic
	void Die();

	// Getters and setters
	bool GetCanJump			() const			{ return m_bCanJump; }
	void SetCanJump			(bool canJump)		{ m_bCanJump = canJump; }
	bool GetCanBeControlled	() const			{ return m_bCanBeControlled; }
	void SetCanBeControlled	(bool canControl)	{ m_bCanBeControlled = canControl; }
	bool GetIsGrounded();

	bool GetIsOnConveyorBelt();
	//void SetIsGrounded( bool bIsGrounded ) { m_bIsGrounded = bIsGrounded; }

	int GetHardContactCount();
	int GetSensorContactCount();

	
	// Called both on BeginContact() and EndContact() from collision engine
	// Will either increment or decrement the m_iHardContactCount 
	void HardContactEvent( bool bBeganContact );
	
	// For platform jump-through
	// Called both on BeginContact() and EndContact() from collision engine
	// Will either increment or decrement the m_iSensorContactCount
	void SensorContactEvent( bool bBeganContact );


	// For GameState related things:: Player lives
	// Additionally included pre-made increment/decrement functions
	int	 GetLives		() const	{ return m_iLives; }
	void SetLives		(int lives) { m_iLives = lives; }
	int  GetMaxLives	() const	{ return m_iMaxLives; }
	bool GetIsAlive		()			{ return m_bIsAlive; }

	void IncrementLives	()			{ m_iLives++; }
	void DecrementLives	()			{ m_iLives--; }
	float GetMovementSpeed()		{ return m_fWalkSpeed; }


	// Avoid calling SetDirection unless you absolutely must do something to the player.
	// If you need this for conveyor belts, please use ConveyorBeltMovement.
	void SetDirection			(EPlayerDirection lastDirection){ m_ePlayerDirection = lastDirection; }
	void SetLastYPos( float yPos ) { m_fLastYPosition = yPos; }

	// Called when landing on top of a platform surface, enables player control and movement
	void LandedOnWalkablePlatform();

	// Called when landing on top of a conveyor belt surface, will set direction lock to pending
	void LandedOnConveyorBelt( EPlayerDirection eDirectionLock );

	// Called when player is no longer touching any ground surface
	void LeftGround();

	// Called when player bumps head or sideways midjump with brick platform (solid, not pass-through)
	void BumpedWithBricks();

	void ForceConveyorBeltMovement();

	// Returns the current movement direction of the player
	EPlayerDirection GetCurrentDirection() const;

};
#endif // #ifndef _CPLAYER_H_