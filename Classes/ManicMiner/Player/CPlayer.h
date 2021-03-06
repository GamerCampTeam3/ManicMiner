///////////////////////////////////////////////
/// Original Author: Bib    - designated as A.B
/// Edits made by: Henrique - designated as H.T
/// Edits made by: Umeer	- designated as U.R
///////////////////////////////////////////////

#ifndef _CPLAYER_H_
#define _CPLAYER_H_

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"
#include "ManicMiner/Enums/EPlayerMovement.h"
#include "ManicMiner/Physics/PlayerB2RayCastCallBack.h"

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
	EPA_Cheat,																											//
	EPA_Jump																											//
};																														//
																														//
//	U.R																													//
enum class EAnimationState																								//
{																														//
	None,																												//
	Idle,																												//
	Run,																												//
	Jump,																												//
};																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //

class CPlayer
	: public CGCObjSpritePhysics
{
private:

// --------------- Physics -------------------------------------------------------------------------------------------- //
																														//
// Reference to the world, needed for raycasting on jump																//
	b2World& m_rcB2World;																								//
																														//
// Reference to the current CManicLayer, needed for death and escape gamestate changes									//
	CManicLayer& m_rcManicLayer;																						//
																														//
// Instance of a CPlayerB2RayCastCallBack, needed for the jump															//
	CPlayerB2RayCastCallBack m_cRayCastCallBack;																		//
																														//
// -------------------------------------------------------------------------------------------------------------------- //


// -------------- Movement Properties --------------------------------------------------------------------------------- //
																														//
// Keeps track of the direction player is heading towards [ horizontal only ]											//
	EPlayerDirection m_ePlayerDirection;																				//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Keeps track of the direction player was heading towards when jumping [ needed for conveyor belt ]					//
	EPlayerDirection m_eJumpDirection;																					//
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
// Whether the player is standing on a conveyor belt or not ( movement not necessarily locked if so )					//
	bool		m_bIsOnConveyorBelt;																					//
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
	const float	m_kfWalkSpeed;																							//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Instant vertical velocity for when player jumps																		//
	float		m_fJumpSpeed;																							//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Specific gravitational acceleration for the player																	//
	const float	m_kfGravitionalPull;																					//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Height delta that if exceeded before landing will result in player fall damage / death								//
	const float m_kfMaxFallDistance;																					//
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
																														//
// Stores the Y coordinate of when Willy left the surface																//
// Used for jumping arch-path calculation																				//
	float	m_fLastGroundedY;																							//
// -------------------------------------------------------------------------------------------------------------------- //
																														//
// Stores the highest Y coordinate since the last jump																	//
// Used for fall damage death																							//
	float	m_fLastHighestY;																							//
// -------------------------------------------------------------------------------------------------------------------- //


// ------ Cached Audio ID --------------------------------------------------------------------------------------------- //
// Running Sound ID																										//
	unsigned int m_uiRunningSoundID;																					//
																														//
// Jump Sound ID																										//
	unsigned int m_uiJumpSoundID;																						//
// Falling Sound ID																										//
	// This second one is needed because SimpleAudioEngine																//
	// does not have the functionality to start playing																	//
	// a certain audio file on X seconds, so I needed																	//
	// a whole new audio file that started a bit ahead on the track														//
	// so that it only plays the falling sequence ( pitch only goes down )												//
	unsigned int m_uiFallingSoundID;																					//
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
	
	TGCActionToKeyMap< EPlayerActions >*	m_pcControllerActionToKeyMap;
	bool m_bSpriteXFlip;
																														
public:																													
// Constructor -------------------------------------------------------------------------------------------------------- // 
	CPlayer( CManicLayer& rcManicLayer, const cocos2d::Vec2& startingPos, const bool spriteFlipStatus);

// Destructor --------------------------------------------------------------------------------------------------------- //
	virtual ~CPlayer();

// Getters ------------------------------------------------------------------------------------------------------------ //
																														//
	EPlayerDirection GetCurrentDirection	() const;																	//
	EPlayerDirection GetJumpDirection		() const;																	//
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
																														//
private:																												//
	void SetHighestMidAirY();																							//
	void SetHighestGroundedY();																							//
// -------------------------------------------------------------------------------------------------------------------- //


public:
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
// Purpose		:	Performs raycast upwards to check for a hard surface directly above player's head					//
//					If that area is clear, then it changes the vertical velocity of the player to be m_fJumpSpeed		//
//																														//
//					Called when jump input is detected, if player is grounded											//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void JumpEvent();																									//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	OnEscape																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Called when player walks through the portal into the next level										//
//					Sets the player as unkillable, uncontrollable and unmovable											//
//																														//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void OnEscape();																									//
																														//
// -------------------------------------------------------------------------------------------------------------------- //


// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// ----- Player Collision Handling ------------------------------------------------------------------------------------ //
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
																														//
private:																												//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	CreatePlayerBoxCollision																			//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Creates 4 edge fixtures and attaches to the player body, these fixtures will make					//
//					an alternative main collider for the player body, one that does not get the player stuck			//
//					when walking towards different surfaces on same height												//
//																														//
//					Called in VOnResurrected																			//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
//																														//
// See Also		:	http://www.iforce2d.net/b2dtut/ghost-vertices & CPlayer::CreateEdgeShape()							//
// -------------------------------------------------------------------------------------------------------------------- //
	void CreatePlayerBoxCollision();																					//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	CreateEdgeShape																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Creates and attaches a single edge fixture to our player body.										//
//					Called 4 times inside CPlayer::CreatePlayerBoxCollision()											//
//																														//
// Parameters	:	const cocos2d::Vec2& v2StartPoint																	//
//					Line starting point																					//
//																														//
//					const cocos2d::Vec2& v2EndPoint																		//
//					Line ending point																					//
//																														//
//					const bool kbBrickCollisionOnly																		//
//					Whether this edge fixture will only collide with "brick" Box2D physics layer						//
//					or both "brick" and "platform" Box2D physics layers													//
//																														//
//					const bool kbIsHorizontalEdge																		//
//					Whether this is a horizontal or vertical edge														//
//																														//
// Returns		:	void																								//
//																														//
// See Also		:	http://www.iforce2d.net/b2dtut/ghost-vertices & CPlayer::CreatePlayerBoxCollision					//
// -------------------------------------------------------------------------------------------------------------------- //
	void CreateEdgeShape( const cocos2d::Vec2& kV2StartPoint, const cocos2d::Vec2& kV2EndPoint, const bool kbBrickCollisionOnly, const bool kbIsHorizontalEdge );
																														//
public:																													//
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
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	OnLanded																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Runs when player comes in contact with a surface.													//
//					Sets some player properties ( for example m_bIsGrounded = true )									//
//					Checks if this is the first contact after jump and, if so,											//
//					Checks for fall height ( could lead to player death )												//
//					Updates Animation State Machine, stops falling sound effect											//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void OnLanded();																									//
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
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ClimbUpBrickLedge																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Forces the player to regain sideways momentum.														//
//					This is a very subtle mechanic from the original Manic Miner game									//
//					Please watch https://youtu.be/BgUzteADsRI?t=84														//
//					and see how the player climbs up the brick wall on his left											//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
//																														//
// See Also		:	https://youtu.be/BgUzteADsRI?t=84																	//
// -------------------------------------------------------------------------------------------------------------------- //
	void ClimbUpBrickLedge();																							//
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
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	LeftConveyorBelt																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Runs when the contact with a conveyor belt ends.													//
//					Sets m_bIsOnConveyorBelt to false																	//
//					Checks if the player is still on some hard surface, and if so, calls LandedOnWalkablePlatform()		//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void LeftConveyorBelt();																							//
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
// -------------------------------------------------------------------------------------------------------------------- //


// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// ----- Player Sound Effect Handling --------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	StopVerticalMovementSound																			//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Stops Jump / Falling Mid-Air Sound Effects															//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void StopVerticalMovementSound();																					//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	StopRunningSound																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Stops Running Sound Effect																			//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void StopRunningSound();																							//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	LandedOnWalkablePlatform																			//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Plays Running Sound Effect																			//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void PlayRunningSound();																							//
																														//
// -------------------------------------------------------------------------------------------------------------------- //


// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// ----- Player Animation State Machine - U.R ------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
																														//
	// true - Loads Animations || false - Releases Animations															//
	void LoadAnimations(bool bShouldLoadAnimations);																	//
																														//
	// reflects which animation to play																					//
	EAnimationState m_eAnimationState;																					//
																														//
	// Serves as a checkpoint to disable functionality of the current state, the animation state machine is in,			//
	// before switching to the new state																				//
	// Can be thought of as the end point of the current state															//
	void InitiateAnimationStateChange(EAnimationState eNewAnimationState);												//
																														//
	// Can be thought of as the starting point of the new state															//
	// Switches the current State m_eAnimationState is in, to the given eNewAnimationState								//
	void AnimationStateChange(EAnimationState* eNewAnimationState);														//
																														//
	// contains all animations for the player character																	//																						//
	std::map<char*, cocos2d::Animation*> m_pcPlayerAnimationList;														//
																														//
	//
	int m_iStartAlternatingTime;																						//
																														//
	int m_iAlternateIdleTimer;																							//
																														//
	bool m_bSelectedStandardIdle;																						//
																														//
	// Alternates between different Idle Animations, which is triggered after "m_iStartAlternatingTime" Seconds			//
	// while the character is standing still																			//
	void AlternateIdleAnimation(bool bPlayStandardIdle);																//
																														//
	void ResetIdle();																									//
// -------------------------------------------------------------------------------------------------------------------- //
};
#endif // #ifndef _CPLAYER_H_