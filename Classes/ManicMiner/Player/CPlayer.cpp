//////////////////////////////////////////////////////////////////////////////////////
// Gamer Camp 2020 / Henrique & Bib													//
//////////////////////////////////////////////////////////////////////////////////////
	

//#define PLAYER_DEBUG_DIRECTION
#define PLAYER_DEBUG_CONTACTS
//#define PLAYER_DEBUG_CONTACTS_REALTIME
//#define PLAYER_DEBUG_LANDING


#include <corecrt_math.h>


#include "Classes/ManicMiner/Player/CPlayer.h"

#include "AppDelegate.h"
#include "Box2D/Dynamics/b2Fixture.h" 
#include "ManicMiner/AudioHelper/ManicAudio.h"
#include "ManicMiner/Helpers/Helpers.h"
#include "ManicMiner/Layers/CManicLayer.h"
#include "GamerCamp/GameController/GCController.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

USING_NS_CC;

// action map arrays must match in length - in the templated controller class we use they map from the user define enum to cocos2d::Controller::Key 
static EPlayerActions			s_aePlayerActions[] = { EPlayerActions::EPA_AxisMove_X,								EPlayerActions::EPA_AxisMove_Y, EPlayerActions::EPA_Cheat,					EPlayerActions::EPA_Jump };
static cocos2d::Controller::Key	s_aeKeys[]			= { cocos2d::Controller::Key::JOYSTICK_LEFT_X,	cocos2d::Controller::Key::JOYSTICK_LEFT_Y,	 cocos2d::Controller::Key::BUTTON_X,   cocos2d::Controller::Key::BUTTON_A };

// Constructor -------------------------------------------------------------------------------------------------------- //
CPlayer::CPlayer( CManicLayer& rcManicLayer, const cocos2d::Vec2& startingPos, const bool spriteFlipStatus )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CPlayer ) )
	, m_rcB2World						( *rcManicLayer.B2dGetWorld() )
	, m_rcManicLayer					( rcManicLayer )
	, m_ePlayerDirection				( EPlayerDirection::Static )
	, m_eJumpDirection					( EPlayerDirection::Static )
	, m_ePendingDirection				( EPlayerDirection::Static )
	, m_bCanJump						( true )
	, m_bCanBeControlled				( true )
	, m_bIsPendingDirection				( false )
	, m_bIsOnConveyorBelt				( false )
	, m_bIsGrounded						( false )
	, m_bIsAlive						( true )
	, m_iSensorContactCount				( 0 )
	, m_iHardContactCount				( 0 )
	, m_fLastGroundedY					( 0.0f )
	, m_fLastHighestY					( 0.0f )
	, m_kfWalkSpeed						( 4.0f )
	, m_fJumpSpeed						( 9.0f )
	, m_kfGravitionalPull				( 1.6f )
	, m_kfMaxFallDistance				( 4.8f )
	, m_fVerticalSpeedAdjust			( 0.0f )
	, m_uiRunningSoundID				( 0 )
	, m_uiJumpSoundID					( 0 )
	, m_uiFallingSoundID				( 0 )
	, m_iMaxLives						( 3 )
	, m_iLives							( m_iMaxLives )
	, m_pcControllerActionToKeyMap		( nullptr )
	, m_bSpriteXFlip					( spriteFlipStatus )
	, m_eAnimationState					( EAnimationState::None )
	, m_iAlternateIdleTimer				( 0 )
	, m_iStartAlternatingTime			( 200 )
	, m_bSelectedStandardIdle			( false )
{
	SetResetPosition( startingPos );
}

// Destructor --------------------------------------------------------------------------------------------------------- //
CPlayer::~CPlayer()
{}

#pragma region Getters
// Getters ------------------------------------------------------------------------------------------------------------ //
																														//
EPlayerDirection CPlayer::GetCurrentDirection() const																	//
{																														//
	return m_ePlayerDirection;																							//
}																														//

EPlayerDirection CPlayer::GetJumpDirection() const
{
	return m_eJumpDirection;
}

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
	return m_bIsOnConveyorBelt;																							//
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

#pragma endregion Getters

#pragma region Setters
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
																														//
// -------------------------------------------------------------------------------------------------------------------- //
#pragma endregion Setters



// ---------- CGCObjSprite Interface ---------------------------------------------------------------------------------------------------------- //
																																				//
IN_CPP_CREATION_PARAMS_DECLARE( CPlayer, "TexturePacker/Sprites/TempCharacter/mm_character_willy.plist", "TempCharacter", b2_dynamicBody, true );	//
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
																																				//
																																				//
	LoadAnimations(true);																														//
																																				//																						//
}																																				//
																																				//
void CPlayer::VOnResourceRelease()																												//
{																																				//
	safeDelete( m_pcControllerActionToKeyMap );																								//
	m_pcPlayerAnimationList.clear();																											//
																																				//
	CGCObjSpritePhysics::VOnResourceRelease();																									//
																																				//

	LoadAnimations(false);																														//
	// Stop Jump/Fall sound effect
	StopVerticalMovementSound();
	StopRunningSound();
																																				//
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

	//	SOLUTION FOR THE COLLISION WITH TOP CORNERS OF THE FLOOR PLATFORMS
	// Very very sadly, only now - the day before our final review - I came across an explanation of why the player was getting stuck ( and only sometimes ) when walking on a flat surface
	// made up of multiple collision segments. Throughout these 2 modules I formulated my own theory as to why this happened, and it's pretty much the same as what is explained
	// on the following link:
	// http://www.iforce2d.net/b2dtut/ghost-vertices
	// Conveniently, the link also gives 3 solutions. The first one is making the edges of our player's feet rounded.
	// The second solution is turning our feet collision into an edge shape and not a polygon.
	// The third solution is the same as the second one, but it's even better as the second one doesn't fix the problem 100% of the time, it just minimizes the chances of the bug to happen.
	// Besides making our collision an edge shape, we also need to add what is called "ghost vertices", which is like an imaginary extension of the edge shape.
	// When a contact is handled with the physics engine, the ghost vertices are taken into account when it comes to redirecting the shape, and so instead of getting stuck
	// Because the shape is colliding and getting pushed to the other side, it will instead push the shape upwards, effectively never getting our player stuck, but also remaining on the floor.

	// I really tried my best to implement this functionality, although my time was extremely limited.
	// Turns out the PhysicsEditor software does not support Edge Shapes at all, only polygons (shapes with 3+ vertices).
	// This being said, I tried to manually create an edge shape in code, and attach it to the player body
	// However, this made it impossible to adjust properties such as the ID string which I do try to access in other parts of the code when handling collisions.
	// I tried to, again, set these manually here, but because of the way this framework is set up it's impossible to do it without messing with a lot of source code.


	// The following link has source code in which they create an edge shape, although the source code there is quite broken
	// But I used it and corrected it in hopes of including the edge shape for the player feet
	// https://rotatingcanvas.com/edge-shape-in-box2d/


	// In an ideal world I would make a BodyDef because this has a list of fixtures, fixtures that can read more info than standard b2fixtures
	// specifically ID string and magic number
	// But a lot of the classes needed to make up this class are hidden inside GB2ShapeCache-x.cpp
	// cocos2d::BodyDef playerBodyDef;
	// then it goes something like playerBodyDef.fixtures->sUserData.IDString = "my id string for this fixture"
	// It's not exactly like that but something along those lines, I never got to finish the line of code above because of the hidden classes issues I stated

	// Now we create the box2d things that would reside inside the higher level classes that are hidden in GB2ShapeCache-x.cpp
	// ( The code below actually works, it's got some magic numbers because I was going to end up changing them anyways, the numbers were exaggerated for testing purposes )


	const cocos2d::Vec2 v2BottomLeft(-5.0f, -60.0f);
	const cocos2d::Vec2 v2BottomRight(65.0f, -60.0f);
	const cocos2d::Vec2 v2TopLeft(-5.0f,  56.0f);
	const cocos2d::Vec2 v2TopRight(65.0f, 56.0f);
	// Bottom
	CreateEdgeShape( v2BottomLeft, v2BottomRight, false, true );
	
	// Left
	CreateEdgeShape( v2BottomLeft, v2TopLeft, true, false );
	
	// Top
	CreateEdgeShape( v2TopLeft, v2TopRight, true, true );
	
	// Right
	CreateEdgeShape( v2TopRight, v2BottomRight, true, false );

	// So the code above actually attached a line segment, not on the player's feet but on the centre of the body
	// It is visible when toggling on the debug collision ( line 326 of IGCGameLayer.cpp )
	// However it was already causing multiple bugs and it wasn't properly set up


																																				//
// Reset sprite orientation																														//
	SetFlippedX( m_bSpriteXFlip );																												//
	SetFlippedY( false );																														//
																																				//
	InitiateAnimationStateChange(EAnimationState::Idle);
																																				//
// Reset all member variable flags																												//
	m_bIsAlive = true;																															//
	m_bCanJump = true;																															//
	m_bCanBeControlled = true;																													//
	m_bIsGrounded = true;																														//
	m_bIsPendingDirection = false;																												//
	m_bIsOnConveyorBelt = false;
	m_fVerticalSpeedAdjust = 0.0f;
	m_fLastHighestY = 0.0f;
	m_fLastGroundedY = 0.0f;
	m_iHardContactCount = 0;
	m_iSensorContactCount = 0;

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
	//CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();
	//pKeyManager->Update();
	//pKeyManager->Reset();
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

	if( m_fVerticalSpeedAdjust != 0.0f )
	{
		Vec2 v2ExpectedVelocity = Vec2( GetVelocity().x, GetVelocity().y - m_fVerticalSpeedAdjust );
		//CCLOG( "Fixing velocity to %f", v2ExpectedVelocity.y );
		SetVelocity( v2ExpectedVelocity );
		m_fVerticalSpeedAdjust = 0.0f;
	}
	
	// If player is in mid-air movement
	if( !GetIsGrounded() )
	{
		float fCurrentY = GetPhysicsTransform().p.y;

		// Update m_fLastHighestY if needed
		if( fCurrentY > m_fLastHighestY )
		{
			m_fLastHighestY = fCurrentY;
		}

		// If player is moving sideways
		// && player is below the initial jumping position
		if( m_ePlayerDirection != EPlayerDirection::Static && fCurrentY < m_fLastGroundedY - 0.15f )
		{
			// End arch-like movement  - >   just drop straight down from now on
			ApplyDirectionChange( EPlayerDirection::Static );
		}
	}
	
	//CCLOG( "Current Vx: %f", GetVelocity().x );
	//CCLOG( "Current y: %f", GetPhysicsTransform().p.y );
	//switch( m_ePlayerDirection )
	//{
	//case EPlayerDirection::Static:
	//	CCLOG( "Static" );
	//	break;
	//case EPlayerDirection::Right:
	//	CCLOG( "Right" );
	//	break;
	//case EPlayerDirection::Left:
	//	CCLOG( "Left" );
	//	break;
	//}


																																				//
	// DEBUG SECTION -------------------------------------------------------------- //															//
#ifdef PLAYER_DEBUG_CONTACTS_REALTIME												//															//
	// ---------- Number of Hard Contacts this frame ------------------ //			//															//
	//																	//			//															//
	std::string string1 = std::to_string( m_iHardContactCount );		//			//															//
	char const* pchar1 = string1.c_str();								//			//															//
	CCLOG( "Hard Count:" );												//			//															//
	CCLOG( pchar1 );													//			//															//
	//																	//			//															//
	// ---------------------------------------------------------------- //			//															//
																					//															//
	// ----------- Number of Soft Contacts this frame ----------------- //			//															//
	//																	//			//															//
	std::string string2 = std::to_string( m_iSensorContactCount );		//			//															//
	char const* pchar2 = string2.c_str();								//			//															//
	CCLOG( "Soft Count:" );												//			//															//
	CCLOG( pchar2 );													//			//															//
	//																	//			//															//
	// ---------------------------------------------------------------- //			//															//
																					//															//
#endif																				//															//
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

	// This is used to cheat and go to the next level.
	// Toggle off by setting to false.
	const bool bCheatMode = true;
	if ( bCheatMode )
	{
		if ( pKeyManager->ActionIsPressed( CManicLayer::EPA_Cheat ) )
		{
			m_rcManicLayer.RequestNextLevel();
		}
	}

	if ( m_bCanJump)
	{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// JUMP																							//
	//////////////////////////////////////////////////////////////////////////////////////////////////
		if( pKeyManager->ActionIsPressed( CManicLayer::EPA_Jump ) )
		{
			//CCLOG( "jump input received" );
			JumpEvent();
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////
		// LEFT																							//
		//////////////////////////////////////////////////////////////////////////////////////////////////
		else if( pKeyManager->ActionIsPressed( CManicLayer::EPA_Left ) )
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
		else if( pKeyManager->ActionIsPressed( CManicLayer::EPA_Right ) )
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

				// Alternate between Idle Animations
				m_iAlternateIdleTimer++;
				//m_iAlternateIdleTimer = m_iAlternateIdleTimer % m_iStartAlternatingTime;
				if(m_iAlternateIdleTimer >= m_iStartAlternatingTime)
				{
					m_iAlternateIdleTimer = 0;

					AlternateIdleAnimation(m_bSelectedStandardIdle); // z check
					m_bSelectedStandardIdle = !m_bSelectedStandardIdle; // z check
				}
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
			float fHorizontalSpeed = 0.0f;
		
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
#ifdef PLAYER_DEBUG_DIRECTION
				CCLOG( "Player going Static" );
#endif
				// Static -> no speed
				fHorizontalSpeed = 0.0f;
				StopRunningSound();
				if (m_bIsAlive && GetIsGrounded())
				{
					InitiateAnimationStateChange( EAnimationState::Idle );
				}
				break;
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// RIGHT																						//
		//////////////////////////////////////////////////////////////////////////////////////////////////
			case EPlayerDirection::Right:
				
				// Debug new direction
#ifdef PLAYER_DEBUG_DIRECTION
				CCLOG( "Player going Right" );
#endif				
				// Right -> move with walk speed
				fHorizontalSpeed = m_kfWalkSpeed;

				// Adjust sprite orientation
				SetFlippedX( true );
				if ( m_bIsAlive && GetIsGrounded() )
				{
					InitiateAnimationStateChange( EAnimationState::Run );
					PlayRunningSound();
				}
				break;
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// LEFT																							//
		//////////////////////////////////////////////////////////////////////////////////////////////////
			case EPlayerDirection::Left:
				
			// Debug new direction
#ifdef PLAYER_DEBUG_DIRECTION
				CCLOG( "Player going Left" );
#endif
			// Left -> move with negative value of walk speed
				fHorizontalSpeed = m_kfWalkSpeed * -1.0f;

			// Adjust sprite orientation
				SetFlippedX( false );
				if( m_bIsAlive && GetIsGrounded() )
				{
					InitiateAnimationStateChange( EAnimationState::Run );
					PlayRunningSound();
				}
				break;
			}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// End of switch

		// Set new player direction enum
			m_ePlayerDirection = eNewDirection;

		// Update the pre jump variable for conveyor belt
			if( m_bIsGrounded )
			{
				m_eJumpDirection = m_ePlayerDirection;
			}
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
	b2Vec2 v2HorizontalOffset( 0.55f, 0.0f );

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
		//CCLOG( "Head Bumped, Jump Cancelled" );
		ApplyDirectionChange( EPlayerDirection::Static );
	}
	else
	{
		const float fVerticalSpeed = m_fJumpSpeed + m_fVerticalSpeedAdjust;
		switch( m_ePlayerDirection )
		{
		case EPlayerDirection::Right:
			SetVelocity( cocos2d::Vec2( m_kfWalkSpeed, fVerticalSpeed ) );
			break;
		case EPlayerDirection::Left:
			SetVelocity( cocos2d::Vec2( m_kfWalkSpeed * -1.0f, fVerticalSpeed ) );
			break;
		case EPlayerDirection::Static:
			SetVelocity( cocos2d::Vec2( 0.0f, fVerticalSpeed ) );
			break;
		}

		m_bCanJump = false;

		// Set Jump Direction
		m_eJumpDirection = m_ePlayerDirection;

	// Unlock from conveyor belt always
		m_bCanBeControlled = true;

		GetPhysicsBody()->SetGravityScale( m_kfGravitionalPull );
		
		StopRunningSound();

		m_uiJumpSoundID = PlaySoundEffect( ESoundEffectName::Jump );

		InitiateAnimationStateChange(EAnimationState::Jump);
	}
}


void CPlayer::CreateEdgeShape( const cocos2d::Vec2& v2StartPoint, const cocos2d::Vec2& v2EndPoint, bool bBrickCollisionOnly, bool bIsHorizontalEdge )
{

	const b2Vec2 b2v2Start ( IGCGameLayer::B2dPixelsToWorld( v2StartPoint.x ), IGCGameLayer::B2dPixelsToWorld( v2StartPoint.y ) );
	const b2Vec2 b2v2End ( IGCGameLayer::B2dPixelsToWorld( v2EndPoint.x ), IGCGameLayer::B2dPixelsToWorld( v2EndPoint.y ) );



	//CALCULATE CENTER OF LINE SEGMENT																									//
	float fPixelsPosX = ( v2StartPoint.x + v2EndPoint.x ) * 0.5f;																								//
	float fPixelsPosY = ( v2StartPoint.y + v2EndPoint.y ) * 0.5f;																		//

	//CALCULATE LENGTH OF LINE SEGMENT																									//
	float fPixelsLength = sqrt( ( v2StartPoint.x - v2EndPoint.x ) * ( v2StartPoint.x - v2EndPoint.x ) + ( v2StartPoint.y - v2EndPoint.y ) * ( v2StartPoint.y - v2EndPoint.y ) );

	//CONVERT CENTER TO BOX COORDINATES																									//
	float fB2PosX = IGCGameLayer::B2dPixelsToWorld( fPixelsPosX );																	   //
	float fB2PosY = IGCGameLayer::B2dPixelsToWorld( fPixelsPosY );																											   //

	//ADD EDGE FIXTURE TO BODY																									   //
	b2EdgeShape b2PlayerEdge;																										   //
	//SET LENGTH IN BOX COORDINATES																								   //

	float fB2Length = IGCGameLayer::B2dPixelsToWorld( fPixelsLength );																		   //

	//SETTING THE POINTS AS OFFSET DISTANCE FROM CENTER																			   //
	if( bIsHorizontalEdge )
	{
		//const b2Vec2 b2v0( fB2Length * 0.5f, fB2PosY );
		//const b2Vec2 b2v1( -fB2Length * 0.5f, fB2PosY );

		const b2Vec2 b2v0( -fB2Length * 0.5f, fB2PosY );
		const b2Vec2 b2v1( fB2Length * 0.5f, fB2PosY );


		b2PlayerEdge.Set( b2v0, b2v1 );			
	}
	else 
	{

		const b2Vec2 b2v0( fB2PosX - 0.5f, b2v2Start.y );
		const b2Vec2 b2v1( fB2PosX - 0.5f, b2v2End.y );
		b2PlayerEdge.Set( b2v0, b2v1 );

	}
	b2FixtureDef fixtureDef;																										   //
	fixtureDef.shape = &b2PlayerEdge;																								   //

	const float fDensity = 2.0f;																									   //
	const float fRestitution = 0.0f;																								   //
	const float fFriction = 0.0f;																									   //

	// Set most of the properties for this fixture																						//
	fixtureDef.density = fDensity;																										//
	fixtureDef.restitution = fRestitution;																								//
	fixtureDef.friction = fFriction;																									//
	fixtureDef.isSensor = false;																										//

	// Collision matrix data																											//
	// I got these values from debugging the shark's "Body" fixture construction														//
	// So categoryBits = 1 means this fixture's category is only marked as part of "player" layer										//
	// Groups aren't set in any of our game shapes																						//
	// And it only collides with the layer "platform", which is the 3rd layer so the value for that mask is 4							//
	if( bBrickCollisionOnly )
	{
		fixtureDef.filter.maskBits = 32;																								//
		fixtureDef.filter.categoryBits = 1;																								//
		fixtureDef.density = 0.0f;																										//
	}
	else
	{
		fixtureDef.filter.maskBits = 36;																								//
		fixtureDef.filter.categoryBits = 1;																							   //
	}
	fixtureDef.filter.groupIndex = 0;																								   //

	// This attaches our new fixture to the existing player body																	   //
	GetPhysicsBody()->CreateFixture( &fixtureDef );																						//
}

void CPlayer::OnEscape()
{
	StopRunningSound();
	StopVerticalMovementSound();
	m_bIsAlive = false;
	m_bCanBeControlled = false;
	m_bCanJump = false;
	GetPhysicsBody()->SetGravityScale( 0.0f );
	SetVelocity( cocos2d::Vec2::ZERO );
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
#ifdef PLAYER_DEBUG_CONTACTS
		CCLOG( "Started Touching Platform, Hard Count is now %d", m_iHardContactCount );
#endif
	}
// Else, a contact came to an end, decrement the sum
	else
	{
		--m_iHardContactCount;
#ifdef PLAYER_DEBUG_CONTACTS
		CCLOG( "Ended Touching Platform, Hard Count is now %d", m_iHardContactCount );
#endif
	}

// Might happen because of bricks
	if ( m_iHardContactCount < 0 )
	{
	// Count should never be less than 0
		CCLOG( " HARD COUNT < 0 " );
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
#ifdef PLAYER_DEBUG_CONTACTS
		CCLOG( "Entered Sensor, Sensor Count is now %d", m_iSensorContactCount );
#endif
	}
// Else, a contact came to an end, decrement the sum
	else
	{
		--m_iSensorContactCount;
#ifdef PLAYER_DEBUG_CONTACTS
		CCLOG( "Left Sensor, Sensor Count is now %d", m_iSensorContactCount );
#endif
	}
}


void CPlayer::OnLanded()
{
// The player is grounded, can jump
	m_bCanJump = true;
	m_bIsGrounded = true;

// If first contact with ground -> landing
	if( m_iHardContactCount == 1 || ( m_iHardContactCount == 2 && m_iSensorContactCount == 1 ) )
	{
	// Check fall damage / death
	// Store last grounded Y coordinate
		float currentY = GetPhysicsTransform().p.y;
		float heightDelta = m_fLastHighestY - currentY;
	// If height difference exceeded the max fall distance
		if( heightDelta >= m_kfMaxFallDistance )
		{
			CCLOG( "Died from fall x(" );
			m_rcManicLayer.OnDeath();
		}
	// Else, not dead, proceed with more logic:
		else 
		{
			// Go back to Idle/Moving
			switch( m_ePlayerDirection )
			{
			case EPlayerDirection::Static:
				InitiateAnimationStateChange( EAnimationState::Idle );
				StopRunningSound();
				break;
			case EPlayerDirection::Right:
				InitiateAnimationStateChange( EAnimationState::Run );
				PlayRunningSound();
				break;
			case EPlayerDirection::Left:
				InitiateAnimationStateChange( EAnimationState::Run );
				PlayRunningSound();
				break;
			}

#ifdef PLAYER_DEBUG_LANDING
		CCLOG( "Landed" );
#endif
		}
	}

	StopVerticalMovementSound();
}


void CPlayer::StopVerticalMovementSound()
{
	{
		// Stop Jump/Fall sound effect
		if( m_uiJumpSoundID != 0 )
		{
			StopSoundEffect( m_uiJumpSoundID );
			m_uiJumpSoundID = 0;
		}
		if( m_uiFallingSoundID != 0 )
		{
			StopSoundEffect( m_uiFallingSoundID );
			m_uiFallingSoundID = 0;
		}
	}
}

void CPlayer::StopRunningSound()
{
	if( m_uiRunningSoundID != 0 )
	{
		StopSoundEffect( m_uiRunningSoundID );
		m_uiRunningSoundID = 0;
	}
}

void CPlayer::PlayRunningSound()
{
	if( !m_uiRunningSoundID )
	{
		m_uiRunningSoundID = PlaySoundEffect( ESoundEffectName::RunningFootsteps );
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
	OnLanded();

	if( !m_bIsOnConveyorBelt )
	{
		m_bCanBeControlled = true;
		m_bIsPendingDirection = false;

	// Run another check for player input
		CheckKeyboardInput();
	}
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
#ifdef PLAYER_DEBUG_LANDING
	CCLOG( "LEFT THE GROUND" );
#endif

	m_bIsPendingDirection = false;
	m_bIsGrounded = false;

// If there is no ground below feet -> player is dropping off ledge
	if( m_bCanJump == true )
	{
	// Drop straight down
		ApplyDirectionChange( EPlayerDirection::Static );
		m_eJumpDirection = EPlayerDirection::Static;
#ifdef PLAYER_DEBUG_DIRECTION
		CCLOG( "Dropping straight down" );
#endif
		m_bCanJump = false;
		m_uiFallingSoundID = PlaySoundEffect( ESoundEffectName::Falling );
		GetPhysicsBody()->SetGravityScale( m_kfGravitionalPull );

		InitiateAnimationStateChange( EAnimationState::Jump );
	}

	// Store last grounded Y coordinate
	m_fLastGroundedY = GetPhysicsTransform().p.y;
	// Set last highest Y to be this current coordinate
	m_fLastHighestY = m_fLastGroundedY;
}


void CPlayer::ClimbUpBrickLedge()
{
	const float kfVerticalSpeed = GetVelocity().y;
	// Only climb if going upwards
	if( kfVerticalSpeed > 0.0f )
	{
		ApplyDirectionChange( m_eJumpDirection );
	}
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
	m_bIsOnConveyorBelt = true;
	OnLanded();

// Debug print out
#ifdef PLAYER_DEBUG_LANDING
	CCLOG( "          on Conveyor Belt" );
#endif
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
	else if( m_eJumpDirection == EPlayerDirection::Left				&&
			 pKeyManager->ActionIsPressed( CManicLayer::EPA_Left  ) &&
			 eDirectionLock == EPlayerDirection::Right  )
	{
		ApplyDirectionChange( EPlayerDirection::Left, true );
		m_bCanBeControlled = true;
		m_bIsPendingDirection = true;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	RIGHT INPUT																																	//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if( m_eJumpDirection == EPlayerDirection::Right			&&
			 pKeyManager->ActionIsPressed( CManicLayer::EPA_Right ) &&
			 eDirectionLock == EPlayerDirection::Left )
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


void CPlayer::LeftConveyorBelt()
{
	m_bIsOnConveyorBelt = false;

	if( m_iHardContactCount == 1 )
	{
		LandedOnWalkablePlatform();
	}
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

void CPlayer::LoadAnimations(bool bShouldLoadAnimations)
{
	int iCounter;
	char* pszAnimations[4];
	pszAnimations[0] = "Idle";
	pszAnimations[1] = "AlternativeIdle";
	pszAnimations[2] = "Run";
	pszAnimations[3] = "Jump";
	if(bShouldLoadAnimations)
	{
		iCounter = 0;
		// Load Animations

		int iCounter = 0;
		for (const char* pszAnim : pszAnimations)
		{
			cocos2d::ValueMap& rdictPlist = GCCocosHelpers::CreateDictionaryFromPlist(GetFactoryCreationParams()->strPlistFile);
			m_pcPlayerAnimationList.insert({ pszAnimations[iCounter], GCCocosHelpers::CreateAnimation(rdictPlist, pszAnimations[iCounter]) });
			m_pcPlayerAnimationList.at(pszAnimations[iCounter])->retain();
			iCounter++;
		}
	}
	else
	{
		iCounter = 3;
		for(iCounter; iCounter <= 0; iCounter--)
		{
			m_pcPlayerAnimationList.at(pszAnimations[iCounter])->release();
		}
	}
}

void CPlayer::InitiateAnimationStateChange(EAnimationState eNewAnimationState)
{
	GetSprite()->stopAllActions();
	switch(m_eAnimationState)
	{
	case EAnimationState::Idle :
		ResetIdle();
		break;
	case EAnimationState::Run :
		break;
	case EAnimationState::Jump :
		break;
	default:	
		break;
	}

	AnimationStateChange(&eNewAnimationState);
}

void CPlayer::AnimationStateChange(EAnimationState* eNewAnimationState)
{
	char* pszAnim = nullptr;
	bool bHasAnimation = false;
	switch(*eNewAnimationState)
	{
	case EAnimationState::None:
		m_eAnimationState = EAnimationState::None;
		bHasAnimation = false;
		break;
	case EAnimationState::Idle:
		m_eAnimationState = EAnimationState::Idle;
		pszAnim = "Idle";
		bHasAnimation = true;
		break;
	case EAnimationState::Run:
		m_eAnimationState = EAnimationState::Run;
		pszAnim = "Run";
		bHasAnimation = true;
		break;
	case EAnimationState::Jump:
		m_eAnimationState = EAnimationState::Jump;
		pszAnim = "Jump";
		bHasAnimation = true;
		break;
	default:
		GetSprite()->stopAllActions();
		bHasAnimation = false;
		break;
	}

	if (bHasAnimation)
	{
		auto pAnimation = m_pcPlayerAnimationList[pszAnim];
		if (nullptr != pAnimation)
		{
			RunAction(GCCocosHelpers::CreateAnimationActionLoop(pAnimation));
		}
	}
}

void CPlayer::AlternateIdleAnimation(bool bPlayStandardIdle)
{
	GetSprite()->stopAllActions();
	char* pszAnim = nullptr;
	if(bPlayStandardIdle)
	{
		pszAnim = "Idle";
	}
	else
	{
		pszAnim = "AlternativeIdle";
	}
	RunAction(GCCocosHelpers::CreateAnimationActionLoop(m_pcPlayerAnimationList.at(pszAnim)));
}

void CPlayer::ResetIdle()
{
	m_iAlternateIdleTimer = 0;
	m_bSelectedStandardIdle = false;
}