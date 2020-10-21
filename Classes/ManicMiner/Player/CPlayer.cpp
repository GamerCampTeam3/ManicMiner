#include <memory.h>

#include "AppDelegate.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GameController/GCController.h"
#include "ManicMiner/Helpers/Helpers.h"


#include "Classes/ManicMiner/Player/CPlayer.h"
#include "ManicMiner/Layers/CManicLayer.h"

USING_NS_CC;

static int testnum = 0;
// action map arrays must match in length - in the templated controller class we use they map from the user define enum to cocos2d::Controller::Key 
static EPlayerActions			s_aePlayerActions[] = { EPlayerActions::EPA_AxisMove_X,								EPlayerActions::EPA_AxisMove_Y,								EPlayerActions::EPA_Jump };
static cocos2d::Controller::Key	s_aeKeys[]			= { cocos2d::Controller::Key::JOYSTICK_LEFT_X,	cocos2d::Controller::Key::JOYSTICK_LEFT_Y,	cocos2d::Controller::Key::BUTTON_A };

//////////////////////////////////////////////////////////////////////////
/// Base constructor that initializes player with typical values
//CPlayer::CPlayer()
//	: CGCObjSpritePhysics( GetGCTypeIDOf( CPlayer ) )
//	, m_ePlayerDirection				( EPlayerDirection::EPD_Static )
//	, m_eLastPlayerDirection			( EPlayerDirection::EPD_Static )
//	, m_bCanJump						( true )
//	, m_bCanBeControlled				( true )
//	, m_bIsOnLadder						( false )
//	, m_bIsAlive						( true )
//	, m_fMovementSpeed					( -4.f )
//	, m_fJumpForce						( 10.0f )
//	, m_v2Movement						( 0.0f, 0.0f )
//	, m_iMaxLives						( 3 )
//	, m_iLives							( m_iMaxLives )
//	, m_pcManicLayer					( nullptr )
//	, m_pcControllerActionToKeyMap		( nullptr )
//{
//
//}

//////////////////////////////////////////////////////////////////////////
/// Overloaded constructor that takes in a reference to the layer and Vec2
CPlayer::CPlayer( CManicLayer& cLayer, const cocos2d::Vec2& startingPos )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CPlayer ) )
	, m_kfGravitionalPull				( 2.3f )
	, m_ePlayerDirection				( EPlayerDirection::EPD_Static )
	, m_eLastPlayerDirection			( EPlayerDirection::EPD_Static )
	, m_ePendingDirection				( EPlayerDirection::EPD_Static )
	, m_bCanJump						( true )
	, m_bCanBeControlled				( true )
	, m_bIsPendingDirection				( false )
	, m_bIsGrounded						( false )
	, m_bIsAlive						( true )
	, m_iSensorContactCount				( 0 )
	, m_iHardContactCount				( 0 )
	, m_v2Movement						( 0.0f, 0.0f )
	, m_fMovementSpeed					( -4.f )
	, m_fJumpSpeed						( 10.6f )
	, m_iMaxLives						( 3 )
	, m_iLives							( m_iMaxLives )
	, m_pcManicLayer					( &cLayer )
	, m_pcControllerActionToKeyMap		( nullptr )
{
	SetResetPosition( startingPos );
}

////////////////////////////////////////////////////////////////////////////
///// Overloaded constructor that takes in a reference to the layer, V2 and lives
//CPlayer::CPlayer( CManicLayer& cLayer, const cocos2d::Vec2& startingPos, const int startingLives )
//	: CGCObjSpritePhysics( GetGCTypeIDOf( CPlayer ) )
//	, m_ePlayerDirection				( EPlayerDirection::EPD_Static )
//	, m_eLastPlayerDirection			( EPlayerDirection::EPD_Static )
//	, m_bCanJump						( true )
//	, m_bCanBeControlled				( true )
//	, m_bIsOnLadder						( false )
//	, m_bIsAlive						( true )
//	, m_v2Movement						( 0.0f, 0.0f )
//	, m_fMovementSpeed					( -4.f )
//	, m_fJumpForce						( 10.0f )
//	, m_iMaxLives						( startingLives )
//	, m_iLives							( m_iMaxLives )
//	, m_pcManicLayer					( &cLayer )
//	, m_pcControllerActionToKeyMap		( nullptr )
//{
//	SetResetPosition( startingPos );
//}
//

//////////////////////////////////////////////////////////////////////////
// //virtual 
//////////////////////////////////////////////////////////////////////////
IN_CPP_CREATION_PARAMS_DECLARE( CPlayer, "TexturePacker/Sprites/TempCharacter/TempCharacter.plist", "TempCharacter", b2_dynamicBody, true );
void CPlayer::VOnResourceAcquire()
{
	IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CPlayer );

	CGCObjSpritePhysics::VOnResourceAcquire();

	const char* pszAnim_marioJog = "Jog";

	// animate!
	//ValueMap dicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	//RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( dicPList, pszAnim_marioJog ) ) );


	// because we're just storing a vanilla pointer we must call delete on it in VOnResourceRelease or leak memory 
	// 
	// n.b. m_pcControllerActionToKeyMap is a "perfect use case" for std::unique_ptr...
	// 
	// n.n.b. ... however if we did use std::unique_ptr we'd need to use std::unique_ptr::reset in VOnResourceRelease if we wanted the memory allocate / free behaviour to be the same...
	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions, s_aeKeys );
}

void CPlayer::VOnResurrected( void )
{
	CGCObjSpritePhysics::VOnResurrected();

	// reset velocity and flip state
	SetFlippedX( false );
	SetFlippedY( false );

	m_bIsAlive = true;
	m_bCanJump = true;
	m_bCanBeControlled = true;

	// reset
	if( GetPhysicsBody() )
	{
		Vec2 v2SpritePos = GetSpritePosition();
		ApplyDirectionChange( EPlayerDirection::EPD_Static, 0.0f, 0.0f );
		GetPhysicsBody()->SetTransform( IGCGameLayer::B2dPixelsToWorld( b2Vec2( v2SpritePos.x, v2SpritePos.y ) ), 0.0f );
		GetPhysicsBody()->SetFixedRotation( true );
		GetPhysicsBody()->SetGravityScale( m_kfGravitionalPull );
	}
}
//////////////////////////////////////////////////////////////////////////

void CPlayer::VOnResourceRelease()
{
	CGCObjSpritePhysics::VOnResourceRelease();
	safeDelete( m_pcControllerActionToKeyMap );
}

//////////////////////////////////////////////////////////////////////////
/// Main function called when the player takes damage
/// First checks if it can take damage to avoid multi function calls during frame
/// Then reduces lives and checks if it should reset or die accordingly
//////////////////////////////////////////////////////////////////////////
void CPlayer::Die()
{
	if ( m_bIsAlive )
	{
		m_iLives--;
		m_bIsAlive = false;
	}
}

bool CPlayer::GetIsOnConveyorBelt()
{
	return ( !m_bCanBeControlled || m_bIsPendingDirection );
}

int CPlayer::GetHardContactCount()
{
	return m_iHardContactCount;
}

int CPlayer::GetSensorContactCount()
{
	return m_iSensorContactCount;
}

void CPlayer::HardContactEvent( bool bBeganContact )
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
	if (m_iHardContactCount < 0)
	{
		m_iHardContactCount = 0;
	}
}

void CPlayer::SensorContactEvent( bool bBeganContact )
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

void CPlayer::LandedOnWalkablePlatform()
{
	CCLOG( "Landed" );
	m_bCanJump = true;
	m_bCanBeControlled = true;
	m_bIsGrounded = true;
	m_bIsPendingDirection = false;

	//ApplyDirectionChange( EPlayerDirection::EPD_Static, 0.0f, 0.0f );

	// Run another check for player input
	KeyboardInput();
}

void CPlayer::LandedOnConveyorBelt( EPlayerDirection eDirectionLock )
{
	testnum++;
	if( testnum == 3 )
	{
 		int z = 0;
	}
	CCLOG( "Landed on Conveyor Belt" );
	m_bCanJump = true;
	m_bIsGrounded = true;


	m_ePendingDirection = eDirectionLock;
	// Now we must check the player input
	const CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();
	TGCController< EPlayerActions > cController = TGetActionMappedController( CGCControllerManager::eControllerOne, ( *m_pcControllerActionToKeyMap ) );
	if( ( pKeyManager->ActionIsPressed( CManicLayer::EPA_Jump ) ) )
	{
		m_bCanBeControlled = true;
		m_bIsPendingDirection = true;
		JumpEvent();
	}
	else if( ( pKeyManager->ActionIsPressed( CManicLayer::EPA_Left  ) ) && ( eDirectionLock == EPlayerDirection::EPD_Right ) )
	{
		ApplyDirectionChange( EPlayerDirection::EPD_Left, m_fMovementSpeed, 0.0f );
		m_bCanBeControlled = true;
		m_bIsPendingDirection = true;
	}
	else if( ( pKeyManager->ActionIsPressed( CManicLayer::EPA_Right ) ) && ( eDirectionLock == EPlayerDirection::EPD_Left ) )
	{
		ApplyDirectionChange( EPlayerDirection::EPD_Right, m_fMovementSpeed * -1.0f, 0.0f );
		m_bCanBeControlled = true;
		m_bIsPendingDirection = true;
	}
	// Not trying to move -> force
	else
	{
		m_bCanBeControlled = true;
		m_bIsPendingDirection = true;
		ForceConveyorBeltMovement();
	}
}

void CPlayer::ForceConveyorBeltMovement( )
{
	CCLOG( "Forced Conveyor Direction" );
	m_bIsGrounded = true;
	m_bCanJump = true;
	m_bCanBeControlled = false;
	m_bIsPendingDirection = false;

	switch( m_ePendingDirection )
	{
		case EPlayerDirection::EPD_Left:
		{
			ApplyDirectionChange( EPlayerDirection::EPD_Left, m_fMovementSpeed, 0.0f );
		}
		break;

		case EPlayerDirection::EPD_Right:
		{
			ApplyDirectionChange( EPlayerDirection::EPD_Left, m_fMovementSpeed * -1.0f, 0.0f );
		}
		break;
	}	

}

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

		m_eLastPlayerDirection = m_ePlayerDirection;
		m_ePlayerDirection = EPlayerDirection::EPD_Static;
	}
	m_bIsGrounded = false;
	m_bIsPendingDirection = false;
}

void CPlayer::BumpedWithBricks()
{
	CCLOG( "Wall Bump" );
	// Make sure player is not standing in conveyor belt
	if ( !GetIsOnConveyorBelt() )
	{
		ApplyDirectionChange( EPlayerDirection::EPD_Static, 0.0f, GetVelocity().y );
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CPlayer::VOnUpdate( f32 fTimeStep )
{
	KeyboardInput();

	//std::string string1 = std::to_string( m_iHardContactCount );
	//char const* pchar1 = string1.c_str();

	//CCLOG( "Hard Count:" );	
	//CCLOG( pchar1 );	
	//
	//std::string string2 = std::to_string( m_iSensorContactCount );
	//char const* pchar2 = string2.c_str();
	//CCLOG( "Soft Count:" );
	//CCLOG( pchar2 );


	//UpdateMovement( fTimeStep );
}


// Reads the keyboard input and moves the player accordingly
void CPlayer::KeyboardInput()
{
	const CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();
	TGCController< EPlayerActions > cController = TGetActionMappedController( CGCControllerManager::eControllerOne, (*m_pcControllerActionToKeyMap) );
	
	if ( m_bCanJump)
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// JUMP																							//
		//////////////////////////////////////////////////////////////////////////////////////////////////
		if ((pKeyManager->ActionIsPressed( CManicLayer::EPA_Jump )))
		{
			//CCLOG( "jump input received" );
			JumpEvent();
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////
		// LEFT																							//
		//////////////////////////////////////////////////////////////////////////////////////////////////
		else if ((pKeyManager->ActionIsPressed( CManicLayer::EPA_Left )))
		{
			// If can be controlled and is grounded
			if ( m_bCanBeControlled && !IsInMidAir())
			{
				ApplyDirectionChange( EPlayerDirection::EPD_Left, m_fMovementSpeed, GetVelocity().y );
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////
		// RIGHT																						//
		//////////////////////////////////////////////////////////////////////////////////////////////////
		else if ((pKeyManager->ActionIsPressed( CManicLayer::EPA_Right )))
		{
			if( m_bCanBeControlled && !IsInMidAir() )
			{
				ApplyDirectionChange( EPlayerDirection::EPD_Right, abs( m_fMovementSpeed ), GetVelocity().y );
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////
		// LADDER SPECIFIC																				//
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//else if( m_bIsOnLadder )
		//{
		//	if( pKeyManager->ActionIsPressed( CManicLayer::EPA_Up ) )
		//	{
		//		ApplyDirectionChange( EPlayerDirection::EPD_Up, 0.0f, abs( m_fMovementSpeed ) );
		//	}
		//	else if( pKeyManager->ActionIsPressed( CManicLayer::EPA_Down ) )
		//	{
		//		ApplyDirectionChange( EPlayerDirection::EPD_Down, 0.0f, m_fMovementSpeed );
		//	}
		//}

		//////////////////////////////////////////////////////////////////////////////////////////////////
		// STATIC																						//
		//////////////////////////////////////////////////////////////////////////////////////////////////
		else
		{
			if( m_bCanBeControlled && !IsInMidAir() )
			{
				ApplyDirectionChange( EPlayerDirection::EPD_Static, 0.0f, 0.0f );
			}
		}
	}

}

//////////////////////////////////////////////////////////////////////////
///	 Switch case used for the player movement
void CPlayer::UpdateMovement( )
{
	SetVelocity( m_v2Movement );
	switch (m_ePlayerDirection)
	{
		case EPlayerDirection::EPD_Right:
			SetFlippedX( true );
			break;

		case EPlayerDirection::EPD_Left:
			SetFlippedX( false );
			break;
	}
}

// Movement Functions called by Input/Jump
void CPlayer::JumpEvent()
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
		case EPlayerDirection::EPD_Right:
			SetVelocity( cocos2d::Vec2( m_fMovementSpeed * -1.0f, m_fJumpSpeed ) );
			break;
		case EPlayerDirection::EPD_Left:
			SetVelocity( cocos2d::Vec2( m_fMovementSpeed, m_fJumpSpeed ) );
			break;
		}
		m_eLastPlayerDirection = m_ePlayerDirection;
		m_ePlayerDirection = m_ePendingDirection;
	}
	//m_eLastPlayerDirection = m_ePlayerDirection;
	//m_ePlayerDirection = EPlayerDirection::EPD_Jumping;

	m_bCanJump = false;

	// Unlock from conveyor belt always
	m_bCanBeControlled = true;
}


void CPlayer::ApplyDirectionChange( EPlayerDirection eNewDirection, float fHorizontalVelocity, float fVerticalVelocity )
{
	// If changing direction
	if( eNewDirection != m_ePlayerDirection )
	{

		// If on conveyor belt and the new direction is not the same as belt movement
		if( m_bIsPendingDirection && eNewDirection != m_ePendingDirection )
		{
			ForceConveyorBeltMovement();
		}

		// Else, walking on normal platform
		else
		{
			switch( eNewDirection )
			{
			case EPlayerDirection::EPD_Static:
				CCLOG( "static" );
				break;
			case EPlayerDirection::EPD_Right:
				CCLOG( "right" );
				break;
			case EPlayerDirection::EPD_Left:
				CCLOG( "left" );
				break;
			}

			// Change velocity
			const Vec2 v2NewVelocity( fHorizontalVelocity, fVerticalVelocity );
			m_v2Movement = v2NewVelocity;

			m_eLastPlayerDirection = m_ePlayerDirection;
			m_ePlayerDirection = eNewDirection;
		
			UpdateMovement();
		}
	}
}