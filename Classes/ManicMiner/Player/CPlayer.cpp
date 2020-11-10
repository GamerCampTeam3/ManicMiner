//////////////////////////////////////////////////////////////////////////////////////
// Gamer Camp 2020 / Henrique & Bib													//
//////////////////////////////////////////////////////////////////////////////////////

#include "Classes/ManicMiner/Player/CPlayer.h"

#include "AppDelegate.h"
#include "Box2D/Dynamics/b2Fixture.h" 
#include "GamerCamp/GameController/GCController.h"
#include "ManicMiner/Helpers/Helpers.h"
#include "ManicMiner/Layers/CManicLayer.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

USING_NS_CC;

// action map arrays must match in length - in the templated controller class we use they map from the user define enum to cocos2d::Controller::Key 
static EPlayerActions			s_aePlayerActions[] = { EPlayerActions::EPA_AxisMove_X,								EPlayerActions::EPA_AxisMove_Y,								EPlayerActions::EPA_Jump };
static cocos2d::Controller::Key	s_aeKeys[]			= { cocos2d::Controller::Key::JOYSTICK_LEFT_X,	cocos2d::Controller::Key::JOYSTICK_LEFT_Y,	cocos2d::Controller::Key::BUTTON_A };

// Constructor -------------------------------------------------------------------------------------------------------- //
CPlayer::CPlayer( b2World& rcB2World, const cocos2d::Vec2& startingPos )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CPlayer ) )
	, m_rcB2World						( rcB2World )
	, m_kfGravitionalPull				( 2.3f )
	, m_ePlayerDirection				( EPlayerDirection::Static )
	, m_ePendingDirection				( EPlayerDirection::Static )
	, m_bCanJump						( true )
	, m_bCanBeControlled				( true )
	, m_bIsPendingDirection				( false )
	, m_bIsGrounded						( false )
	, m_bIsAlive						( true )
	, m_iSensorContactCount				( 0 )
	, m_iHardContactCount				( 0 )
	, m_fWalkSpeed						( 4.0f )
	, m_fJumpSpeed						( 10.6f )
	, m_iMaxLives						( 3 )
	, m_iLives							( m_iMaxLives )
	, m_pcControllerActionToKeyMap		( nullptr )
{
	SetResetPosition( startingPos );
}

// Destructor --------------------------------------------------------------------------------------------------------- //
CPlayer::~CPlayer()
{}


// Getters ------------------------------------------------------------------------------------------------------------ //
																														//
EPlayerDirection CPlayer::GetCurrentDirection() const																	//
{																														//
	return m_ePlayerDirection;																							//
}																														//
																														//
bool CPlayer::GetCanJump() const																						//
{																														//
	return m_bCanJump;																									//
}																														//
																														//
bool CPlayer::GetCanBeControlled() const																				//
{																														//
	return m_bCanBeControlled;																							//
}																														//
																														//
bool CPlayer::GetIsGrounded() const																						//
{																														//
	return m_bIsGrounded;																								//
}																														//
																														//
bool CPlayer::GetIsOnConveyorBelt() const																				//
{																														//
	return ( !m_bCanBeControlled || m_bIsPendingDirection );															//
}																														//
																														//
int CPlayer::GetMaxLives() const																						//
{																														//
	return m_iMaxLives;																									//
}																														//
																														//
int CPlayer::GetLives() const																							//
{																														//
	return m_iLives;																									//
}																														//
																														//
bool CPlayer::GetIsAlive() const																						//
{																														//
	return m_bIsAlive;																									//
}																														//
																														//
int CPlayer::GetHardContactCount() const																				//
{																														//
	return m_iHardContactCount;																							//
}																														//
																														//
int CPlayer::GetSensorContactCount() const																				//
{																														//
	return m_iSensorContactCount;																						//
}																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //


// Setters ------------------------------------------------------------------------------------------------------------ //
																														//
void CPlayer::SetCanJump( const bool bCanJump )																			//
{																														//
	m_bCanJump = bCanJump;																								//
}																														//
																														//
void CPlayer::SetCanBeControlled( const bool bCanControl )																//
{																														//
	m_bCanBeControlled = bCanControl;																					//
}																														//
																														//
void CPlayer::SetLives( const int iLives )																				//
{																														//
	m_iLives = iLives;																									//
}																														//
																														//
void CPlayer::SetLastYPos( const float fYPos )																			//
{																														//
	m_fLastYPosition = fYPos;																							//
}																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //





// ---------- CGCObjSprite Interface ---------------------------------------------------------------------------------------------------------- //
																																				//
IN_CPP_CREATION_PARAMS_DECLARE( CPlayer, "TexturePacker/Sprites/TempCharacter/TempCharacter.plist", "TempCharacter", b2_dynamicBody, true );	//
void CPlayer::VOnResourceAcquire()																												//
{																																				//
	IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CPlayer );																				//
																																				//
	CGCObjSpritePhysics::VOnResourceAcquire();																									//
																																				//
	// because we're just storing a vanilla pointer we must call delete on it in VOnResourceRelease or leak memory 								//
	// 																																			//
	// n.b. m_pcControllerActionToKeyMap is a "perfect use case" for std::unique_ptr...															//
	// 																																			//
	// n.n.b. ... however if we did use std::unique_ptr 																						//
	// we'd need to use std::unique_ptr::reset in VOnResourceRelease if we wanted the memory allocate / free behaviour to be the same...		//
	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions, s_aeKeys );														//
}																																				//
																																				//
void CPlayer::VOnResourceRelease()																												//
{																																				//
	safeDelete( m_pcControllerActionToKeyMap );																									//
																																				//
	CGCObjSpritePhysics::VOnResourceRelease();																									//
}																																				//
																																				//
// -------------------------------------------------------------------------------------------------------------------- //						//
// Function		:	VOnResurrected																						//						//
// -------------------------------------------------------------------------------------------------------------------- //						//
// Parameters	:	none																								//						//
//																														//						//
// Returns		:	void																								//						//
// -------------------------------------------------------------------------------------------------------------------- //						//																													
void CPlayer::VOnResurrected()																													//
{																																				//
	CGCObjSpritePhysics::VOnResurrected();																										//
																																				//
// Reset sprite orientation																														//
	SetFlippedX( false );																														//
	SetFlippedY( false );																														//
																																				//
// Reset all member variable flags																												//
	m_bIsAlive = true;																															//
	m_bCanJump = true;																															//
	m_bCanBeControlled = true;																													//
	m_bIsGrounded = true;																														//
	m_bIsPendingDirection = false;																												//
																																				//
																																				//
// Reset physics body related components																										//
	if( GetPhysicsBody() )																														//
	{																																			//
		Vec2 v2SpritePos = GetSpritePosition();																									//
		ApplyDirectionChange( EPlayerDirection::Static, true );																					//
		GetPhysicsBody()->SetTransform( IGCGameLayer::B2dPixelsToWorld( b2Vec2( v2SpritePos.x, v2SpritePos.y ) ), 0.0f );						//
		GetPhysicsBody()->SetFixedRotation( true );																								//
		GetPhysicsBody()->SetGravityScale( m_kfGravitionalPull );																				//
	}																																			//

// Reset Keyboard State
	CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();
	pKeyManager->Update();
	pKeyManager->Reset();
}																																				//
																																				//
// -------------------------------------------------------------------------------------------------------------------- //						//
// Function		:	VOnUpdate																							//						//
// -------------------------------------------------------------------------------------------------------------------- //						//
// Parameters	:	f32 fTimeStep																						//						//
//																														//						//
// Returns		:	void																								//						//
// -------------------------------------------------------------------------------------------------------------------- //						//
void CPlayer::VOnUpdate( f32 fTimeStep )																										//
{																																				//
// Get player input and change movement if needed																								//
	CheckKeyboardInput();																														//
														
	b2Vec2 v2RayStart = GetPhysicsTransform().p;
	b2Vec2 v2RayEnd = v2RayStart + b2Vec2( 0.0f, 1.5f );
	//draw a line
	
																																				//
	// DEBUG SECTION -------------------------------------------------------------- //															//
																					//															//
	// ---------- Number of Hard Contacts this frame ------------------ //			//															//
	//																	//			//															//
	//std::string string1 = std::to_string( m_iHardContactCount );		//			//															//
	//char const* pchar1 = string1.c_str();								//			//															//
	//CCLOG( "Hard Count:" );												//			//															//
	//CCLOG( pchar1 );													//			//															//
	////																	//			//															//
	//// ---------------------------------------------------------------- //			//															//
	//																				//															//
	//// ----------- Number of Soft Contacts this frame ----------------- //			//															//
	////																	//			//															//
	//std::string string2 = std::to_string( m_iSensorContactCount );		//			//															//
	//char const* pchar2 = string2.c_str();								//			//															//
	//CCLOG( "Soft Count:" );												//			//															//
	//CCLOG( pchar2 );													//			//															//
	//																	//			//															//
	// ---------------------------------------------------------------- //			//															//
																					//															//
																					//															//
	// END OF DEBUG SECTION ------------------------------------------------------- //															//
}																																				//
																																				//
// -------------------------------------------------------------------------------------------------------------------------------------------- //


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	CheckKeyboardInput																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::CheckKeyboardInput()
{
	const CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();
	TGCController< EPlayerActions > cController = TGetActionMappedController( CGCControllerManager::eControllerOne, (*m_pcControllerActionToKeyMap) );
	
	if ( m_bCanJump)
	{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// JUMP																							//
	//////////////////////////////////////////////////////////////////////////////////////////////////
		if( ( pKeyManager->ActionIsPressed( CManicLayer::EPA_Jump ) ) )
		{
			//CCLOG( "jump input received" );
			JumpEvent();
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////
		// LEFT																							//
		//////////////////////////////////////////////////////////////////////////////////////////////////
		else if( ( pKeyManager->ActionIsPressed( CManicLayer::EPA_Left ) ) )
		{
			// If can be controlled and is grounded
			if( m_bCanBeControlled && m_bIsGrounded )
			{
				ApplyDirectionChange( EPlayerDirection::Left );
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////
		// RIGHT																						//
		//////////////////////////////////////////////////////////////////////////////////////////////////
		else if( ( pKeyManager->ActionIsPressed( CManicLayer::EPA_Right ) ) )
		{
			if( m_bCanBeControlled && m_bIsGrounded )
			{
				ApplyDirectionChange( EPlayerDirection::Right );
			}
		}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// STATIC																						//
	//////////////////////////////////////////////////////////////////////////////////////////////////
		else
		{
			if( m_bCanBeControlled && m_bIsGrounded )
			{
				ApplyDirectionChange( EPlayerDirection::Static, true );
			}
		}
	}
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ApplyDirectionChange																				//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	const EPlayerDirection eNewDirection																//
//																														//
//					const bool bResetVerticalVelocity		->		defaults to false									//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::ApplyDirectionChange( const EPlayerDirection eNewDirection, const bool bResetVerticalVelocity )
{
// If changing from current direction
	if( eNewDirection != m_ePlayerDirection )
	{
	// If on conveyor belt and the new direction is not the same as the pending conveyor belt direction
		if( m_bIsPendingDirection && eNewDirection != m_ePendingDirection )
		{
			ForceConveyorBeltMovement();
		}
		
	// Else, walking normally on a regular platform
		else
		{
		// Set new vertical speed according to the ``bResetVerticalVelocity`` argument
			float fVerticalSpeed = GetVelocity().y;
			if( bResetVerticalVelocity )
			{
				fVerticalSpeed = 0.0f;
			}
		// Set new horizontal speed according to the ``eNewDirection`` argument
			float fHorizontalSpeed;
		
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// Switch on eNewDirection																		//
		//////////////////////////////////////////////////////////////////////////////////////////////////
			switch( eNewDirection )
			{
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// STATIC																						//
		//////////////////////////////////////////////////////////////////////////////////////////////////
			case EPlayerDirection::Static:
				
				// Debug new direction
				CCLOG( "Player going Static" );
				
				// Static -> no speed
				fHorizontalSpeed = 0.0f;
				
				break;
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// RIGHT																						//
		//////////////////////////////////////////////////////////////////////////////////////////////////
			case EPlayerDirection::Right:
				
				// Debug new direction
				CCLOG( "Player going Right" );
				
				// Right -> move with walk speed
				fHorizontalSpeed = m_fWalkSpeed;

				// Adjust sprite orientation
				SetFlippedX( true );
				
				break;
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// LEFT																							//
		//////////////////////////////////////////////////////////////////////////////////////////////////
			case EPlayerDirection::Left:
				
			// Debug new direction
				CCLOG( "Player going Left" );

			// Right -> move with negative value of walk speed
				fHorizontalSpeed = m_fWalkSpeed * -1.0f;

			// Adjust sprite orientation
				SetFlippedX( false );
				
				break;
			}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// End of switch

		// Set new player direction enum
			m_ePlayerDirection = eNewDirection;

		// Change current velocity
			const Vec2 v2NewVelocity( fHorizontalSpeed, fVerticalSpeed );
			SetVelocity( v2NewVelocity );
		}
	}
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	JumpEvent																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::JumpEvent()
{
// Raycast to check if can jump
// Start from centre of player body
	b2Vec2 v2RayStart ( GetPhysicsTransform().p );
// We just want to check the immediate block above player head
// Because the player is 2 blocks high, and we start from its centre
// We will need a raycast length of 1.5 blocks
// This way our raycast ends in the centre of the block above the head
	float fRayLength = 1.5f;
	b2Vec2 v2RayEnd	= v2RayStart + b2Vec2( 0.0f, fRayLength );

// We will need to check on both extremities of the player
// As if we're shooting a raycast off of both shoulders
	b2Vec2 v2HorizontalOffset( 0.5f, 0.0f );

// Another possible approach would be AABB querying
// We could check a whole rectangle area above the player's head
// But because everything is axis aligned, we're fine with just 2 vertical lines
// For more info check	http://www.iforce2d.net/b2dtut/raycasting
//					and http://www.iforce2d.net/b2dtut/world-querying

// Perform RayCasts -> a flag will be set in case any hits occur

	// Reset RayCast Hit Flag
	m_cRayCastCallBack.ResetFlag();

	// Right Side
	m_rcB2World.RayCast( &m_cRayCastCallBack, ( v2RayStart + v2HorizontalOffset ), ( v2RayEnd + v2HorizontalOffset ) );
	// Left Side
	m_rcB2World.RayCast( &m_cRayCastCallBack, ( v2RayStart - v2HorizontalOffset ), ( v2RayEnd - v2HorizontalOffset ) );

	if( m_cRayCastCallBack.GetDidRayHit() )
	{
		CCLOG( "Head Bumped, Jump Cancelled" );
		ApplyDirectionChange( EPlayerDirection::Static );
	}
	else
	{
	// If on normal platform, jump normally
		if( !m_bIsPendingDirection )
		{
			SetVelocity( cocos2d::Vec2 ( GetVelocity().x, m_fJumpSpeed ) );
		}
	// Else, on top of conveyor, jump that way
		else
		{
			switch( m_ePendingDirection )
			{
			case EPlayerDirection::Right:
				SetVelocity( cocos2d::Vec2( m_fWalkSpeed, m_fJumpSpeed ) );
				break;
			case EPlayerDirection::Left:
				SetVelocity( cocos2d::Vec2( m_fWalkSpeed * -1.0f, m_fJumpSpeed ) );
				break;
			}
			m_ePlayerDirection = m_ePendingDirection;
		}

		m_bCanJump = false;

	// Unlock from conveyor belt always
		m_bCanBeControlled = true;

		CCLOG( "Jumped" );
	}
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	HardContactEvent																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	const bool bBeganContact																			//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::HardContactEvent( const bool bBeganContact )
{
// If began a new contact, add to the sum
	if ( bBeganContact )
	{
		++m_iHardContactCount;
	}
// Else, a contact came to an end, decrement the sum
	else
	{
		--m_iHardContactCount;
	}

// Might happen because of bricks
	if ( m_iHardContactCount < 0 )
	{
	// Count should never be less than 0
		m_iHardContactCount = 0;
	}
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	SensorContactEvent																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	const bool bBeganContact																			//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::SensorContactEvent( const bool bBeganContact )
{
// If began a new contact, add to the sum
	if( bBeganContact )
	{
		++m_iSensorContactCount;
	}
// Else, a contact came to an end, decrement the sum
	else
	{
		--m_iSensorContactCount;
	}
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	LandedOnWalkablePlatform																			//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::LandedOnWalkablePlatform()
{
	CCLOG( "Landed" );
	m_bCanJump = true;
	m_bCanBeControlled = true;
	m_bIsGrounded = true;
	m_bIsPendingDirection = false;

// Run another check for player input
	CheckKeyboardInput();
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	LeftGround																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::LeftGround()
{
	CCLOG( "Left the Ground" );
// If player did not initiate jump
	if( m_bCanJump )
	{
	// Drop straight down
		auto v2CurrentVelocity = GetVelocity();
		SetVelocity( cocos2d::Vec2( 0.0f, v2CurrentVelocity.y ) );
	
		CCLOG( "Dropping straight down" );
		m_bCanJump = false;

		m_ePlayerDirection = EPlayerDirection::Static;
	}
	m_bIsGrounded = false;
	m_bIsPendingDirection = false;
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	BumpedWithBricks																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::BumpedWithBricks()
{
	CCLOG( "Wall Bump" );
// Make sure player is not standing in conveyor belt
	if ( !GetIsOnConveyorBelt() )
	{
		ApplyDirectionChange( EPlayerDirection::Static );
	}
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	LandedOnConveyorBelt																				//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	const EPlayerDirection eDirectionLock																//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::LandedOnConveyorBelt( const EPlayerDirection eDirectionLock )
{
// Debug print out
	CCLOG( "Landed on Conveyor Belt" );

// The player is grounded, can jump
	m_bCanJump = true;
	m_bIsGrounded = true;

// Set new pending direction
	m_ePendingDirection = eDirectionLock;

// Check player input state:

// If as soon as Willy lands on the conveyor belt
// the player is trying to agaisn't it, Willy should be able to
// If not, we force the direction and lock it so Willy is no longer allowed to change direction ( until he lands once again )

// Get input manager
	const CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();
	TGCController< EPlayerActions > cController = TGetActionMappedController( CGCControllerManager::eControllerOne, ( *m_pcControllerActionToKeyMap ) );
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// JUMP INPUT																																	//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if( ( pKeyManager->ActionIsPressed( CManicLayer::EPA_Jump ) ) )
	{	
	// Perform Jump
		m_bCanBeControlled = true;
		m_bIsPendingDirection = true;
		JumpEvent();
	}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	LEFT INPUT																																	//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if( ( pKeyManager->ActionIsPressed( CManicLayer::EPA_Left  ) ) && ( eDirectionLock == EPlayerDirection::Right ) )
	{
		ApplyDirectionChange( EPlayerDirection::Left, true );
		m_bCanBeControlled = true;
		m_bIsPendingDirection = true;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	RIGHT INPUT																																	//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if( ( pKeyManager->ActionIsPressed( CManicLayer::EPA_Right ) ) && ( eDirectionLock == EPlayerDirection::Left ) )
	{
		ApplyDirectionChange( EPlayerDirection::Right, true);
		m_bCanBeControlled = true;
		m_bIsPendingDirection = true;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	NO INPUT																																	//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else
	{
	// Force Direction
		m_bCanBeControlled = true;
		m_bIsPendingDirection = true;
		ForceConveyorBeltMovement();
	}
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ForceConveyorBeltMovement																			//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::ForceConveyorBeltMovement( )
{
// Debug
	CCLOG( "Forced Conveyor Direction" );
	
// Set as grounded, can jump, cannot be controlled (direction locked), and no longer pending direction
	m_bIsGrounded = true;
	m_bCanJump = true;
	m_bCanBeControlled = false;
	m_bIsPendingDirection = false;

// Set new direction
	ApplyDirectionChange( m_ePendingDirection, true );
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	Die																									//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::Die()
{
	if ( m_bIsAlive )
	{
		m_iLives--;
		m_bIsAlive = false;
	}
}