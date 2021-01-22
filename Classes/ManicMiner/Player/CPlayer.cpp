//////////////////////////////////////////////////////////////////////////////////////
// Gamer Camp 2020 / Henrique & Bib													//
//////////////////////////////////////////////////////////////////////////////////////
	
#pragma region DebugLogDefines
// Uncomment these #define's to get extra information during runtime through the Visual Studio Output Log
//#define PLAYER_DEBUG_DIRECTION
//#define PLAYER_DEBUG_DIRECTION_REALTIME
//#define PLAYER_DEBUG_CONTACTS
//#define PLAYER_DEBUG_CONTACTS_REALTIME
//#define PLAYER_DEBUG_LANDING
//#define PLAYER_DEBUG_PHYSICS_REALTIME
#pragma endregion DebugLogDefines


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
																														
EPlayerDirection CPlayer::GetCurrentDirection() const
{
	return m_ePlayerDirection;
}

EPlayerDirection CPlayer::GetJumpDirection() const
{
	return m_eJumpDirection;
}

bool CPlayer::GetCanJump() const
{
	return m_bCanJump;
}

bool CPlayer::GetCanBeControlled() const
{
	return m_bCanBeControlled;
}

bool CPlayer::GetIsGrounded() const
{
	return m_bIsGrounded;
}

bool CPlayer::GetIsOnConveyorBelt() const
{
	return m_bIsOnConveyorBelt;
}

int CPlayer::GetMaxLives() const
{
	return m_iMaxLives;
}

int CPlayer::GetLives() const
{
	return m_iLives;
}

bool CPlayer::GetIsAlive() const
{
	return m_bIsAlive;
}

int CPlayer::GetHardContactCount() const
{
	return m_iHardContactCount;
}

int CPlayer::GetSensorContactCount() const
{
	return m_iSensorContactCount;
}

// -------------------------------------------------------------------------------------------------------------------- //

#pragma endregion Getters

#pragma region Setters
// Setters ------------------------------------------------------------------------------------------------------------ //

void CPlayer::SetCanJump( const bool bCanJump )
{
	m_bCanJump = bCanJump;
}

void CPlayer::SetCanBeControlled( const bool bCanControl )
{
	m_bCanBeControlled = bCanControl;
}

void CPlayer::SetLives( const int iLives )
{
	m_iLives = iLives;
}

// -------------------------------------------------------------------------------------------------------------------- //
#pragma endregion Setters


// ---------- CGCObjSprite Interface ---------------------------------------------------------------------------------------------------------- //
																																				//
IN_CPP_CREATION_PARAMS_DECLARE( CPlayer, "TexturePacker/Sprites/TempCharacter/mm_character_willy.plist", "TempCharacter", b2_dynamicBody, true );//
// -------------------------------------------------------------------------------------------------------------------- //						//
// Function		:	VOnResourceAcquire																					//						//
// -------------------------------------------------------------------------------------------------------------------- //						//
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
	LoadAnimations(true);																														//
}																																				//
// -------------------------------------------------------------------------------------------------------------------- //						//
// Function		:	VOnResourceRelease																					//						//
// -------------------------------------------------------------------------------------------------------------------- //						//
void CPlayer::VOnResourceRelease()																												//
{																																				//
	safeDelete( m_pcControllerActionToKeyMap );																									//
	m_pcPlayerAnimationList.clear();																											//
																																				//
	CGCObjSpritePhysics::VOnResourceRelease();																									//
																																				//
	LoadAnimations(false);																														//
																																				//
	// Stop Jump/Fall sound effects																												//
	StopVerticalMovementSound();																												//
	StopRunningSound();																															//
																																				//
}																																				//
																																				//
// -------------------------------------------------------------------------------------------------------------------- //						//
// Function		:	VOnResurrected																						//						//
// -------------------------------------------------------------------------------------------------------------------- //						//
void CPlayer::VOnResurrected()																													//
{																																				//
	CGCObjSpritePhysics::VOnResurrected();																										//
																																				//
// Create player body collision fixtures																										//
	CreatePlayerBoxCollision();																													//
																																				//
																																				//
// Reset sprite orientation																														//
	SetFlippedX( m_bSpriteXFlip );																												//
	SetFlippedY( false );																														//
																																				//
	InitiateAnimationStateChange(EAnimationState::Idle);																						//
																																				//
// Reset all member variable flags																												//
	m_bIsAlive = true;																															//
	m_bCanJump = true;																															//
	m_bCanBeControlled = true;																													//
	m_bIsGrounded = true;																														//
	m_bIsPendingDirection = false;																												//
	m_bIsOnConveyorBelt = false;																												//
	m_fLastHighestY = 0.0f;																														//
	m_fLastGroundedY = 0.0f;																													//
	m_iHardContactCount = 0;																													//
	m_iSensorContactCount = 0;																													//
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
}																																				//
																																				//
// -------------------------------------------------------------------------------------------------------------------- //						//
// Function		:	VOnUpdate																							//						//
// -------------------------------------------------------------------------------------------------------------------- //						//
																																				//
void CPlayer::VOnUpdate( f32 fTimeStep )																										//
{																																				//
// Get player input and change movement if needed																								//
	CheckKeyboardInput();																														//
																																				//
	// If player is in mid-air movement																											//
	if( !GetIsGrounded() )																														//
	{																																			//
		float fCurrentY = GetPhysicsTransform().p.y;																							//
																																				//
		// Update m_fLastHighestY if needed																										//
		if( fCurrentY > m_fLastHighestY )																										//
		{																																		//
			m_fLastHighestY = fCurrentY;																										//
		}																																		//
																																				//
		// If player is moving sideways																											//
		// && player is below the initial jumping position																						//
		if( m_ePlayerDirection != EPlayerDirection::Static && fCurrentY < m_fLastGroundedY - 0.15f )											//
		{																																		//
			// End arch-like movement  - >   just drop straight down from now on																//
			ApplyDirectionChange( EPlayerDirection::Static );																					//
		}																																		//
	}																																			//
																																				//
#pragma region DebugLogging																														//
	// DEBUG SECTION -------------------------------------------------------------- //															//
																					//															//
#ifdef PLAYER_DEBUG_DIRECTION_REALTIME												//															//
	// -- Player Direction Realtime ----------------------------------- //			//															//
	switch( m_ePlayerDirection )										//			//															//
	{																	//			//															//
	case EPlayerDirection::Static:										//			//															//
	CCLOG( "Static" );													//			//															//
	break;																//			//															//
	case EPlayerDirection::Right:										//			//															//
	CCLOG( "Right" );													//			//															//
	break;																//			//															//
	case EPlayerDirection::Left:										//			//															//
	CCLOG( "Left" );													//			//															//
	break;																//			//															//
	}																	//			//															//
	// ---------------------------------------------------------------- //			//															//
#endif // PLAYER_DEBUG_DIRECTION_REALTIME											//															//
																					//															//
#ifdef PLAYER_DEBUG_PHYSICS_REALTIME												//															//
	// -- Player Velocity Realtime ------------------------------------ //			//															//
	CCLOG( "Current Vx: %f", GetVelocity().x );							//			//															//
	// -- Player Y Coordinate Realtime -------------------------------- //			//															//
	CCLOG( "Current y: %f", GetPhysicsTransform().p.y );				//			//															//
	// ---------------------------------------------------------------- //			//															//
#endif // PLAYER_DEBUG_PHYSICS_REALTIME												//															//
																					//															//
																					//															//
#ifdef PLAYER_DEBUG_CONTACTS_REALTIME												//															//
	// ---------- Number of Hard Contacts this frame ------------------ //			//															//
	//																	//			//															//
	CCLOG( "Hard Count: %d", m_iHardContactCount );						//			//															//
	//																	//			//															//
	// ---------------------------------------------------------------- //			//															//
																					//															//
	// ----------- Number of Soft Contacts this frame ----------------- //			//															//
	//																	//			//															//
	CCLOG( "Soft Count: %d", m_iSensorContactCount );					//			//															//
	//																	//			//															//
	// ---------------------------------------------------------------- //			//															//
#endif	// PLAYER_DEBUG_CONTACTS_REALTIME											//															//
																					//															//
	// END OF DEBUG SECTION ------------------------------------------------------- //															//
#pragma endregion DebugLogging
}																																				//
																																				//
// -------------------------------------------------------------------------------------------------------------------------------------------- //


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	CheckKeyboardInput																					//
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
				
				// switch Idle Animation and invert m_bSelectStandardIdle Boolean,
				// if the Timer Count is Higher than or equals the Int specified
				// in m_iStartAlternatingTime 
				if(m_iAlternateIdleTimer >= m_iStartAlternatingTime)
				{
					// reset Alternate Idle Timer Count
					m_iAlternateIdleTimer = 0;

					AlternateIdleAnimation(m_bSelectedStandardIdle); 
					m_bSelectedStandardIdle = !m_bSelectedStandardIdle; 
				}
			}
		}
	}
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ApplyDirectionChange																				//
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
					// Set Animation State to Idle and play respective Animation
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
					// Set Animation State to Run and play respective Animation
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
					// Set Animation State to Run and play respective Animation
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
void CPlayer::JumpEvent()
{
// Raycast to check if can jump

// This Raycast was necessary to fix a bug that was present by the end of Module 1 / Pre-Production
// If the player were to stand directly below a brick platform and jump, the player would get permanently stuck
// So this implementation fixes that bug, by not allowing the player to jump in the first place
// Later on it was discovered that this was no longer needed as the bug was no longer happening,
// But I have left this active because there's no harm done anyways.


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
//				   and	http://www.iforce2d.net/b2dtut/world-querying

// Perform RayCasts -> a flag will be set in case any hits occur

	// Reset RayCast Hit Flag
	m_cRayCastCallBack.ResetFlag();

	// Right Side
	m_rcB2World.RayCast( &m_cRayCastCallBack, ( v2RayStart + v2HorizontalOffset ), ( v2RayEnd + v2HorizontalOffset ) );
	// Left Side
	m_rcB2World.RayCast( &m_cRayCastCallBack, ( v2RayStart - v2HorizontalOffset ), ( v2RayEnd - v2HorizontalOffset ) );

	// If any of the RayCasts hit, cancel jump
	if( m_cRayCastCallBack.GetDidRayHit() )
	{
#ifdef PLAYER_DEBUG_DIRECTION
		CCLOG( "Head Bumped, Jump Cancelled" );
#endif
		ApplyDirectionChange( EPlayerDirection::Static );
	}
	// If no hit detected, proceed to jump
	else
	{
		switch( m_ePlayerDirection )
		{
		case EPlayerDirection::Right:
			SetVelocity( cocos2d::Vec2( m_kfWalkSpeed, m_fJumpSpeed ) );
			break;
		case EPlayerDirection::Left:
			SetVelocity( cocos2d::Vec2( m_kfWalkSpeed * -1.0f, m_fJumpSpeed ) );
			break;
		case EPlayerDirection::Static:
			SetVelocity( cocos2d::Vec2( 0.0f, m_fJumpSpeed ) );
			break;
		}

		m_bCanJump = false;

	// Set Jump Direction
		m_eJumpDirection = m_ePlayerDirection;

	// Unlock from conveyor belt always
		m_bCanBeControlled = true;
		
		StopRunningSound();

		m_uiJumpSoundID = PlaySoundEffect( ESoundEffectName::Jump );

		// Set Animation State to Jump and play respective Animation
		InitiateAnimationStateChange(EAnimationState::Jump);

#ifdef PLAYER_DEBUG_DIRECTION
		CCLOG( "Player Jumped!" );
#endif
	}
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	CreatePlayerBoxCollision																			//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::CreatePlayerBoxCollision()
{
//	SOLUTION FOR THE COLLISION WITH TOP CORNERS OF THE FLOOR PLATFORMS

// There's a tl;dr at the bottom

// Very very sadly, only now - the day before our final review - I came across an explanation as to why the player
// quite often gets stuck when walking on a flat surface made up of multiple collision segments.
// Throughout these 2 modules I formulated my own theory as to why this happened, and it's pretty much the same as what is explained
// on the following link: http://www.iforce2d.net/b2dtut/ghost-vertices
// Conveniently, the link also gives 3 solutions to this problem.
//
// 1.
//	First solution: making the edges of our player's feet rounded. I had tried this before; Sounds good, doesn't work.
//  The player would just slide off the edges, completely breaking the gameplay and caused more issues than it solved.
//
// 2.
//	The second solution is turning our feet collider into an edge shape instead of using a polygon.
//  This is said to fix the issue 99% of the time.
//
// 3.
//  The third solution is the same as the second one, but on steroids, and will fix the issue 100% of the time:
//  Besides making our collision an edge shape, we also need to add what are called "ghost vertices", which are like imaginary extensions of the edge shape.
//  These extra vertices will extend our edge shape, yet in reality won't collide with anything.
//  However, when the box2d engine handles a collision with our edge shape, it will take these ghost vertices into account
//  and will never allow the shape to get stuck, as it will always push the edge shape upwards,
//  enough so none of the vertices are inside the blocks / platforms.

// I really tried my best to implement this functionality, although my time was extremely limited - 1 day for the final review.
// I was aiming to implementing the 3rd solution, because I appreciate robustness, but I ran out of time and was only able to implement the 2nd one.
// Turns out the PhysicsEditor software does not support Edge Shapes at all, only Polygons (convex shapes with 3+ vertices).
// This being said, I had to manually create an edge shape in code, and attach it to the player body
// However, this made it impossible to adjust some special properties that are not inherently part of the Box2D engine,
// specifically the ID string, which I use in other parts of the code when handling collisions.
// I tried to, again, set these manually here, but because of the way this framework is set up it's impossible to do it without messing with a lot of the engine source code.
// Not enough is exposed.
// But I made it work though 8)
// Since this implementation I have not witnessed the player getting stuck a single time

// TL;DR
//  I fixed the infamous walking-on-platforms collision bug by not using a PhysicsEditor collision shape and using these EdgeShape's instead

// Set our 4 Vec2 Points that will define our player collision box
// Remember this "box" will in fact be empty inside, only the 4 edges will be used in collisions

	const float kfMaxY = 56.0f;
	const float kfMinY = -60.0f;
	const float kfMaxX = 65.0f;
	const float kfMinX = -5.0f;

	const cocos2d::Vec2 kV2BottomLeft( kfMinX, kfMinY );
	const cocos2d::Vec2 kV2BottomRight( kfMaxX, kfMinY );
	const cocos2d::Vec2 kV2TopLeft( kfMinX, kfMaxY );
	const cocos2d::Vec2 kV2TopRight( kfMaxX, kfMaxY );

	// Bottom Edge
	CreateEdgeShape( kV2BottomLeft, kV2BottomRight, false, true );

	// Left Edge
	CreateEdgeShape( kV2BottomLeft, kV2TopLeft, true, false );

	// Top Edge
	CreateEdgeShape( kV2TopLeft, kV2TopRight, true, true );

	// Right Edge
	CreateEdgeShape( kV2TopRight, kV2BottomRight, true, false );
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	CreateEdgeShape																						//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::CreateEdgeShape( const cocos2d::Vec2& kV2StartPoint, const cocos2d::Vec2& kV2EndPoint, const bool kbBrickCollisionOnly, const bool kbIsHorizontalEdge )
{
// The following link has source code in which they create an edge shape, although the source code there is very broken and unreadable
// https://rotatingcanvas.com/edge-shape-in-box2d/
// I have fixed it, and modified it so it's applicable for this scenario

// Turn v2StartPoint and v2EndPoint into their respective Box2D Vec2 Coordinates
	const b2Vec2 b2v2Start ( IGCGameLayer::B2dPixelsToWorld( kV2StartPoint.x ), IGCGameLayer::B2dPixelsToWorld( kV2StartPoint.y ) );
	const b2Vec2 b2v2End ( IGCGameLayer::B2dPixelsToWorld( kV2EndPoint.x ), IGCGameLayer::B2dPixelsToWorld( kV2EndPoint.y ) );


// Calculate centre of the line segment																									//
	float fPixelsPosX = ( kV2StartPoint.x + kV2EndPoint.x ) * 0.5f;																								//
	float fPixelsPosY = ( kV2StartPoint.y + kV2EndPoint.y ) * 0.5f;																		//

// Calculate length of the line segment																									//
	float fPixelsLength = sqrt( ( kV2StartPoint.x - kV2EndPoint.x ) * ( kV2StartPoint.x - kV2EndPoint.x ) + ( kV2StartPoint.y - kV2EndPoint.y ) * ( kV2StartPoint.y - kV2EndPoint.y ) );

// Convert the centre to Box2D Coordinates																									//
	float fB2PosX = IGCGameLayer::B2dPixelsToWorld( fPixelsPosX );																	   //
	float fB2PosY = IGCGameLayer::B2dPixelsToWorld( fPixelsPosY );																											   //

// Add edge fixture to our body
	b2EdgeShape b2PlayerEdge;																										   //

// Set length in Box2D units
	float fB2Length = IGCGameLayer::B2dPixelsToWorld( fPixelsLength );																		   //

	// Setting the points with offset distance from the centre
	// If this is a horizontal edge ( Top / Bottom )
	if( kbIsHorizontalEdge )
	{
		const b2Vec2 b2v0( -fB2Length * 0.5f, fB2PosY );
		const b2Vec2 b2v1( fB2Length * 0.5f, fB2PosY );
		b2PlayerEdge.Set( b2v0, b2v1 );			
	}
	// Else, it's a vertical edge ( Left / Right )
	else 
	{
		const b2Vec2 b2v0( fB2PosX - 0.5f, b2v2Start.y );
		const b2Vec2 b2v1( fB2PosX - 0.5f, b2v2End.y );
		b2PlayerEdge.Set( b2v0, b2v1 );
	}

	// Create fixtureDef struct
	b2FixtureDef fixtureDef;																										   //
	fixtureDef.shape = &b2PlayerEdge;																								   //

	const float fDensity = 2.0f;																									   //
	const float fRestitution = 0.0f;																								   //
	const float fFriction = 0.0f;																									   //

	// Set most of the properties for this fixtureDef																						//
	fixtureDef.density = fDensity;																										//
	fixtureDef.restitution = fRestitution;																								//
	fixtureDef.friction = fFriction;																									//
	fixtureDef.isSensor = false;																										//

	// Collision matrix data																											//
	// I got these values from debugging the shark's "Body" fixture construction														//
	// So categoryBits = 1 means this fixture's category is only marked as part of "player" layer										//
	// If bBrickCollisionOnly == true, this fixture will only collide with the "brick" layer, 0x20
	// Else, it will collide both with "brick" layer and "platform" layer, 0x4
	// Therefore, the maskBits value will be 32 ( 0x20 ) or 36 (0x20 & 0x4)

	// Extra Info: be weary of editing the maskBits manually on PhysicsEditor, because it takes in hexadecimal.							//
	if( kbBrickCollisionOnly )
	{
		fixtureDef.filter.maskBits = 32;																								//
		fixtureDef.density = 0.0f;																										//
	}
	else
	{
		fixtureDef.filter.maskBits = 36;																								//
	}
		fixtureDef.filter.categoryBits = 1;																							   //
	// Groups aren't set in any of our game shapes																						//
	fixtureDef.filter.groupIndex = 0;																								   //

	// Create and attach our new fixture to the existing player body																	   //
	GetPhysicsBody()->CreateFixture( &fixtureDef );																						//
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	OnEscape																							//
// -------------------------------------------------------------------------------------------------------------------- //
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

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	OnLanded																					//
// -------------------------------------------------------------------------------------------------------------------- //
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
				// Set Animation State to Idle and play respective Animation
				InitiateAnimationStateChange( EAnimationState::Idle );
				StopRunningSound();
				break;
			case EPlayerDirection::Right:
				// Set Animation State to Run and play respective Animation
				InitiateAnimationStateChange( EAnimationState::Run );
				PlayRunningSound();
				break;
			case EPlayerDirection::Left:
				// Set Animation State to Run and play respective Animation
				InitiateAnimationStateChange( EAnimationState::Run );
				PlayRunningSound();
				break;
			}

#ifdef PLAYER_DEBUG_LANDING
		CCLOG( "Landed" );
#endif
		}
	}
	// Update highest grounded Y coordinate
	SetHighestGroundedY();

	// Stop jump / falling sound effect
	StopVerticalMovementSound();
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	StopVerticalMovementSound																					//
// -------------------------------------------------------------------------------------------------------------------- //
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

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	StopRunningSound																					//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::StopRunningSound()
{
	if( m_uiRunningSoundID != 0 )
	{
		StopSoundEffect( m_uiRunningSoundID );
		m_uiRunningSoundID = 0;
	}
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	PlayRunningSound																					//
// -------------------------------------------------------------------------------------------------------------------- //
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

		InitiateAnimationStateChange( EAnimationState::Jump );
	}

	SetHighestGroundedY();
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ClimbUpBrickLedge																					//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::ClimbUpBrickLedge()
{
	// https://youtu.be/BgUzteADsRI?t=84
	// Only climb if going upwards
	if( GetVelocity().y > 0.0f )
	{
		ApplyDirectionChange( m_eJumpDirection );
	}
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	BumpedWithBricks																					//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::BumpedWithBricks()
{
#ifdef PLAYER_DEBUG_CONTACTS
	CCLOG( "Wall Bump" );
#endif // PLAYER_DEBUG_CONTACTS

// Make sure player is not standing in conveyor belt
	if ( !GetIsOnConveyorBelt() )
	{
		ApplyDirectionChange( EPlayerDirection::Static );
	}
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	LandedOnConveyorBelt																				//
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
void CPlayer::ForceConveyorBeltMovement( )
{
// Debug
#ifdef PLAYER_DEBUG_DIRECTION
	CCLOG( "Forced Conveyor Direction" );
#endif // PLAYER_DEBUG_DIRECTION
		
// Set as grounded, can jump, cannot be controlled (direction locked), and no longer pending direction
	m_bIsGrounded = true;
	m_bCanJump = true;
	m_bCanBeControlled = false;
	m_bIsPendingDirection = false;

// Set new direction
	ApplyDirectionChange( m_ePendingDirection, true );
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	LeftConveyorBelt																					//
// -------------------------------------------------------------------------------------------------------------------- //
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
void CPlayer::Die()
{
	if ( m_bIsAlive )
	{
		m_iLives--;
		m_bIsAlive = false;
	}
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	LoadAnimations																						//
// Is in charge of loading/unloading all the animations required for the Player Class									//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::LoadAnimations(bool bShouldLoadAnimations)
{
	int iCounter = 0;
	char* pszAnimations[4];
	pszAnimations[0] = "Idle";
	pszAnimations[1] = "AlternativeIdle";
	pszAnimations[2] = "Run";
	pszAnimations[3] = "Jump";
	if(bShouldLoadAnimations)
	{
		// Load Animations

		cocos2d::ValueMap& rdictPlist = GCCocosHelpers::CreateDictionaryFromPlist(GetFactoryCreationParams()->strPlistFile);
		for (const char* pszAnim : pszAnimations)
		{
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
			m_pcPlayerAnimationList.at(pszAnimations[iCounter]) = nullptr;
			m_pcPlayerAnimationList.erase(pszAnimations[iCounter]);
		}
	}
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	InitiateAnimationStateChange																		//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::InitiateAnimationStateChange(EAnimationState eNewAnimationState)
{
	// This is called to stop all currently running animations,
	// if RunAction is called a second time to play a different animation,
	// while an animation is already playing, then it will not stop the
	// previous animation, but start playing the new animation as well.
	// So we have to stop the currently playing animations, then play/run
	// the new one.
	GetSprite()->stopAllActions();
	switch(m_eAnimationState)
	{
	case EAnimationState::None:
		break;
	case EAnimationState::Idle :
		// Reset Alternating Idle Animations when the current state is idle and we're switching to a different state
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

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	AnimationStateChange																				//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::AnimationStateChange(EAnimationState* eNewAnimationState)
{
	char* pszAnim = nullptr;
	bool bHasAnimation = false;
	if (nullptr != eNewAnimationState)
	{
		switch (*eNewAnimationState)
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
	}

	if (bHasAnimation && nullptr != pszAnim)
	{
		auto pAnimation = m_pcPlayerAnimationList[pszAnim];
		if (nullptr != pAnimation)
		{
			// Play given Animation based on the Animation State
			RunAction(GCCocosHelpers::CreateAnimationActionLoop(pAnimation));
		}
	}
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	AlternateIdleAnimation																				//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::AlternateIdleAnimation(bool bPlayStandardIdle)
{
	// stop previous actions/animations
	GetSprite()->stopAllActions();

	// choose string based on the input parameter of this function
	char* pszAnim = nullptr;
	pszAnim = (bPlayStandardIdle) ? "Idle" : "AlternativeIdle";

	// Play Animation based on the chosen string
	RunAction(GCCocosHelpers::CreateAnimationActionLoop(m_pcPlayerAnimationList.at(pszAnim)));
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ResetIdle																							//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::ResetIdle()
{
	// resets member variables that are needed for alternating between different idle animations
	m_iAlternateIdleTimer = 0;
	m_bSelectedStandardIdle = false;
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	SetHighestGroundedY																					//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::SetHighestGroundedY()
{
	// Store last grounded Y coordinate
	m_fLastGroundedY = GetPhysicsTransform().p.y;
	SetHighestMidAirY();
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	SetHighestMidAirY																					//
// -------------------------------------------------------------------------------------------------------------------- //
void CPlayer::SetHighestMidAirY()
{
	// Set last highest Y to be this current coordinate
	m_fLastHighestY = GetPhysicsTransform().p.y;
}