///////////////////////////////////////////////
/// Original Author: Bib    - designated as A.B
/// Edits made by: Henrique - designated as H.T
///////////////////////////////////////////////

#ifndef _CPLAYER_H_
#define _CPLAYER_H_

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"
#include "ManicMiner/Enums/EPlayerMovement.h"


// ----------------------------------------- Fwd declares ------------------------------------------------------------- //
class CManicLayer;																										//
																														//
template< typename TActionType > class TGCActionToKeyMap;																//
																														//
// enum of user defined input actions the class																			//
enum  EPlayerActions																									//
{																														//
	EPA_AxisMove_X,																										//
	EPA_AxisMove_Y,																										//
	EPA_Jump																											//
};																														//
// -------------------------------------------------------------------------------------------------------------------- //


class CPlayer
	: public CGCObjSpritePhysics
{
private:


// -------------- Movement Properties --------------------------------------------------------------------------------- //
																														//
// Keeps track of the direction player is heading towards [ horizontal only ]											//
	EPlayerDirection m_ePlayerDirection;																				//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Pending direction, assigned by the last contacted CMovingPlatform													//
	EPlayerDirection m_ePendingDirection;																				//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Enables / Disables ability to Jump																					//
// Toggled when Player lands on / leaves the ground surface																//
	bool		m_bCanJump;																								//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Enables / Disables ability to change horizontal direction															//
// Only set to false when Player is on top of a CMovingPlatform, and his walking direction is locked.					//
	bool		m_bCanBeControlled;																						//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Tells us if the Player is pending a direction change																	//
// We use this to allow the Player to keep walking against the CMovingPlatform											//
// Until he tries to change direction. This will force the Player to go in the direction								//
// Of the conveyor belt, and will set m_bCanBeControlled to true, effectively locking horizontal movement				//
	bool		m_bIsPendingDirection;																					//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Tells us if the Player is standing on a surface																		//
// This implies that m_iHardContactCount > 0																			//
	bool		m_bIsGrounded;																							//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Tells us if the player is alive this frame																			//
// We use this to avoid multiple deaths during the same frame															//
// ( might happen because of multiple collisions )																		//
	bool		m_bIsAlive;																								//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Horizontal velocity for when player is moving sideways																//
	float		m_fWalkSpeed;																							//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Instant vertical velocity for when player jumps																		//
	float		m_fJumpSpeed;																							//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Specific gravitational acceleration for the player																	//
	const float	m_kfGravitionalPull;																					//
// -------------------------------------------------------------------------------------------------------------------- //
																														
																														
// -------------- Collision Properties -------------------------------------------------------------------------------- //
																														//
// Expresses the current number of "Soft Contacts"																		//
// How many unique sensors is our "Feet" sensor overlapping with during this frame										//
	int		m_iSensorContactCount;																						//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Expresses the current number of "Hard Contacts"																		//
// How many fixtures is our player touching during this frame															//
// Does not include any sensor fixtures																					//
	int		m_iHardContactCount;																						//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Stores the Y coordinate of when Willy left the surface																//
// Functionality not yet implemented, but this will be needed for fall damage / death									//
	float	m_fLastYPosition;																							//
// -------------------------------------------------------------------------------------------------------------------- //



// ----- Life logic --------------------------------------------------------------------------------------------------- //
																														//
// Number of times player can die before losing																			//
	int	m_iMaxLives;																									//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Number of lives left before losing																					//
	int	m_iLives;																										//
// -------------------------------------------------------------------------------------------------------------------- //
	
// -
	TGCActionToKeyMap< EPlayerActions >*	m_pcControllerActionToKeyMap;												
																														
public:																													
// Constructor -------------------------------------------------------------------------------------------------------- // 
	CPlayer( const cocos2d::Vec2& startingPos);

// Destructor --------------------------------------------------------------------------------------------------------- //
	virtual ~CPlayer();

// Getters ------------------------------------------------------------------------------------------------------------ //
																														//
	EPlayerDirection GetCurrentDirection	() const;																	//
	bool			 GetCanJump				() const;																	//
	bool			 GetCanBeControlled		() const;																	//
	bool			 GetIsGrounded			() const;																	//
	bool			 GetIsOnConveyorBelt	() const;																	//
																														//
	int				 GetMaxLives			() const;																	//
	int				 GetLives				() const;																	//
	bool			 GetIsAlive				() const;																	//
																														//
	int				 GetHardContactCount	() const;																	//
	int				 GetSensorContactCount	() const;																	//
																														//
// Setters ------------------------------------------------------------------------------------------------------------ //
																														//
	void SetCanJump			( const bool	bCanJump	);																//
	void SetCanBeControlled	( const bool	bCanControl	);																//
	void SetLives			( const int		iLives		);																//
	void SetLastYPos		( const float	fYPos		);																//
																														//
// -------------------------------------------------------------------------------------------------------------------- //



// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// ---------- CGCObjSprite Interface ---------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
																														//
	virtual void VOnResourceAcquire	() override;																		//
	virtual void VOnResourceRelease	() override;																		//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	VOnResurrected																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Resets sprite orientation																			//
//					Resets movement related member variables															//
//					Resets physics body related components																//
//																														//
// Parameters	:	f32 fTimeStep																						//
//					Time elapsed this frame																				//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	virtual void VOnResurrected() override;																				//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	VOnUpdate																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Checks Player Input																					//
//					Prints out debug info if said instructions are left uncommented										//
//																														//
// Parameters	:	f32 fTimeStep																						//
//					Time elapsed this frame																				//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	virtual void VOnUpdate( f32 fTimeStep ) override;																	//
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //








// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// ----- Update and movement functions -------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	CheckKeyboardInput																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Checks Player Input, updates movement with ApplyDirectionChange() if needed							//
//																														//
//					Called from the VOnUpdate loop																		//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void CheckKeyboardInput();																							//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ApplyDirectionChange																				//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Changes the horizontal velocity of player, according to the new direction we want to force			//
//					Also resets the vertical velocity if 2nd parameter is set to true									//
//					Changes the m_ePlayerDirection to eNewDirection														//
//																														//
// Parameters	:	const EPlayerDirection eNewDirection																//
//					Direction we want to move towards																	//
//																														//
//					const bool bResetVerticalVelocity		->		defaults to false									//
//					Should we set the player's velocity.y to be 0?														//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void ApplyDirectionChange ( const EPlayerDirection eNewDirection, const bool bResetVerticalVelocity = false );		//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	JumpEvent																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Instantly changes the vertical velocity of the player to be m_fJumpSpeed							//
//																														//
//					Called when jump input is detected, if player is grounded											//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void JumpEvent();																									//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //








// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// ----- Player Collision Handling ------------------------------------------------------------------------------------ //
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	HardContactEvent																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Increments or decrements m_iHardContactCount														//
//					Used when player lands on platforms or bumps into walls												//
//																														//
//					Called from current CManicLayer when player & platform hard contact began or ended					//
//																														//
// Parameters	:	const bool bBeganContact																			//
//					Whether the contact began ( true ) or ended ( false )												//
//																														//
// Returns		:	void																								//
//																														//
// See Also		:	SensorContactEvent & CManicLayer's b2ContactListener Interface functions							//
// -------------------------------------------------------------------------------------------------------------------- //
	void HardContactEvent( const bool bBeganContact );																	//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	SensorContactEvent																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Increments or decrements m_iSensorContactCount														//
//					Serves to enable / disable the collision for the platform in question								//
//																														//
//					Called from current CManicLayer when player's feet sensor starts or stops overlapping				//
//					with a platform's top surface sensor ( always below player feet )									//
//																														//
// Parameters	:	const bool bBeganContact																			//
//					Whether the overlap began ( true ) or ended ( false )												//
//																														//
// Returns		:	void																								//
//																														//
// See Also		:	HardContactEvent & CManicLayer's b2ContactListener Interface functions								//
// -------------------------------------------------------------------------------------------------------------------- //
	void SensorContactEvent( const bool bBeganContact );																//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	LandedOnWalkablePlatform																			//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Adjusts some movement properties [ m_bCanJump, m_bIsGrounded, etc ]									//
//					Debugs that player has landed																		//
//					Re-enables player control and movement																//
//																														//
//					Called from current CManicLayer when landing on top of a platform surface							//
//					if previously considered as not grounded															//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
//																														//
// See Also		:	CPlatform & CManicLayer's b2ContactListener Interface functions										//
// -------------------------------------------------------------------------------------------------------------------- //
	void LandedOnWalkablePlatform();																					//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	LeftGround																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Sets player as not grounded, might force player to drop straight down if this was not a jump		//
//																														//
//					Called when m_iHardContactCount turns to 0 ( player is no longer touching any ground surface )		//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
//																														//
// See Also		:	CPlatform & CManicLayer's b2ContactListener Interface functions										//
// -------------------------------------------------------------------------------------------------------------------- //
	void LeftGround();																									//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	BumpedWithBricks																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Sets player's velocity's horizontal component to 0													//
//																														//
//					Called when player is in mid air and bumps his head or bumps sideways with a brick platform			//
//					A brick platform is consistently solid, player can never pass through one							//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
//																														//
// See Also		:	CBrickPlatform & CManicLayer's b2ContactListener Interface functions								//
// -------------------------------------------------------------------------------------------------------------------- //
	void BumpedWithBricks();																							//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	LandedOnConveyorBelt																				//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Sets direction lock to pending																		//
//																														//
//					Called from current CManicLayer when landing on top of a conveyor belt surface						//
//																														//
// Parameters	:	const EPlayerDirection eDirectionLock																//
//					Direction the conveyor belt wants to enforce														//
//																														//
// Returns		:	void																								//
//																														//
// See Also		:	CMovingPlatform & CManicLayer's b2ContactListener Interface functions								//
// -------------------------------------------------------------------------------------------------------------------- //
	void LandedOnConveyorBelt( const EPlayerDirection eDirectionLock );													//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ForceConveyorBeltMovement																			//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Sets player as grounded, and forces player to take the pending direction lock						//
//					Sets player as no longer controllable																//
//																														//
//					Called when player lands on a conveyor belt and is not trying to move								//
//					Also called when player was previously walking against the direction lock							//
//					and now decided to change direction																	//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
//																														//
// See Also		:	CMovingPlatform & CManicLayer's b2ContactListener Interface functions								//
// -------------------------------------------------------------------------------------------------------------------- //
	void ForceConveyorBeltMovement();																					//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	Die																									//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Decrements m_iLives by 1																			//
//																														//
//					Called from the current CManicLayer when player hits an enemy or hazard or runs out of air			//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void Die();																											//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
};
#endif // #ifndef _CPLAYER_H_