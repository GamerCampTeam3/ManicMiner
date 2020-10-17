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


// action map arrays must match in length - in the templated controller class we use they map from the user define enum to cocos2d::Controller::Key 
static EPlayerActions			s_aePlayerActions[] = { EPlayerActions::EPA_AxisMove_X,								EPlayerActions::EPA_AxisMove_Y,								EPlayerActions::EPA_Jump };
static cocos2d::Controller::Key	s_aeKeys[]			= { cocos2d::Controller::Key::JOYSTICK_LEFT_X,	cocos2d::Controller::Key::JOYSTICK_LEFT_Y,	cocos2d::Controller::Key::BUTTON_A };



//////////////////////////////////////////////////////////////////////////
/// Base constructor that initializes player with typical values
CPlayer::CPlayer()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CPlayer ) )
	, m_ePlayerDirection				( EPlayerDirection::EPD_Static )
	, m_eLastPlayerDirection			( EPlayerDirection::EPD_Static )
	, m_bCanJump						( true )
	, m_bCanBeControlled				( true )
	, m_bIsOnLadder						( false )
	, m_bIsAlive						( true )
	, m_fMovementSpeed					( -4.f )
	, m_fJumpForce						( 10.0f )
	, m_v2Movement						( 0.0f, 0.0f )
	, m_iMaxLives						( 3 )
	, m_iLives							( m_iMaxLives )
	, m_pcManicLayer					( nullptr )
	, m_pcControllerActionToKeyMap		( nullptr )
{

}

//////////////////////////////////////////////////////////////////////////
/// Overloaded constructor that takes in a reference to the layer and Vec2
CPlayer::CPlayer( CManicLayer& cLayer, const cocos2d::Vec2& startingPos )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CPlayer ) )
	, m_ePlayerDirection				( EPlayerDirection::EPD_Static )
	, m_eLastPlayerDirection			( EPlayerDirection::EPD_Static )
	, m_bCanJump						( true )
	, m_bCanBeControlled				( true )
	, m_bIsOnLadder						( false )
	, m_bIsGrounded						( false )
	, m_bIsAlive						( true )
	, m_v2Movement						( 0.0f, 0.0f )
	, m_fMovementSpeed					( -4.f )
	, m_fJumpForce						( 8.0f )
	, m_iMaxLives						( 3 )
	, m_iLives							( m_iMaxLives )
	, m_pcManicLayer					( &cLayer )
	, m_pcControllerActionToKeyMap		( nullptr )
{
	SetResetPosition( startingPos );
}

//////////////////////////////////////////////////////////////////////////
/// Overloaded constructor that takes in a reference to the layer, V2 and lives
CPlayer::CPlayer( CManicLayer& cLayer, const cocos2d::Vec2& startingPos, const int startingLives )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CPlayer ) )
	, m_ePlayerDirection				( EPlayerDirection::EPD_Static )
	, m_eLastPlayerDirection			( EPlayerDirection::EPD_Static )
	, m_bCanJump						( true )
	, m_bCanBeControlled				( true )
	, m_bIsOnLadder						( false )
	, m_bIsAlive						( true )
	, m_v2Movement						( 0.0f, 0.0f )
	, m_fMovementSpeed					( -4.f )
	, m_fJumpForce						( 10.0f )
	, m_iMaxLives						( startingLives )
	, m_iLives							( m_iMaxLives )
	, m_pcManicLayer					( &cLayer )
	, m_pcControllerActionToKeyMap		( nullptr )
{
	SetResetPosition( startingPos );
}


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

void CPlayer::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();

	// reset velocity and flip state
	SetFlippedX			( false );
	SetFlippedY			( false );
	ApplyDirectionChange( EPlayerDirection::EPD_Static, 0.0f, 0.0f );
	
	m_bIsAlive = true;	
	m_bCanJump = true;
	m_bCanBeControlled = true;

	// reset
	if (GetPhysicsBody())
	{
		Vec2 v2SpritePos = GetSpritePosition();
		GetPhysicsBody()->SetLinearVelocity	( b2Vec2( 0.0f, 0.0f ) );
		GetPhysicsBody()->SetTransform		( IGCGameLayer::B2dPixelsToWorld( b2Vec2( v2SpritePos.x, v2SpritePos.y ) ), 0.0f );
		GetPhysicsBody()->SetFixedRotation	( true );
		GetPhysicsBody()->SetGravityScale	( m_kfGravitionalPull );
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

void CPlayer::LandedOnWalkablePlatform()
{
	if( GetVelocity().y < 0.0f )
	{
		CCLOG( "Landed" );
		m_bCanJump = true;
		m_bCanBeControlled = true;
		m_bIsGrounded = true;



		//ApplyDirectionChange( EPlayerDirection::EPD_Static, 0.0f, 0.0f );
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CPlayer::VOnUpdate( f32 fTimeStep )
{
	KeyboardInput();
	//UpdateMovement( fTimeStep );
}


// Reads the keyboard input and moves the player accordingly
void CPlayer::KeyboardInput()
{
	const CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();
	TGCController< EPlayerActions > cController = TGetActionMappedController( CGCControllerManager::eControllerOne, (*m_pcControllerActionToKeyMap) );
	
	if ( m_bCanJump && GetVelocity().y == 0.0f)
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// JUMP																							//
		//////////////////////////////////////////////////////////////////////////////////////////////////
		if ((pKeyManager->ActionIsPressed( CManicLayer::EPA_Jump )))
		{
			CCLOG( "jump input received" );
			JumpEvent();
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////
		// LEFT																							//
		//////////////////////////////////////////////////////////////////////////////////////////////////
		else if ((pKeyManager->ActionIsPressed( CManicLayer::EPA_Left )))
		{
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
	SetVelocity( cocos2d::Vec2 ( GetVelocity().x, m_fJumpForce ) );
	m_eLastPlayerDirection = m_ePlayerDirection;
	m_ePlayerDirection = EPlayerDirection::EPD_Jumping;

	m_bCanJump = false;
	m_bIsGrounded = false;
}


void CPlayer::ApplyDirectionChange( EPlayerDirection eNewDirection, float fHorizontalVelocity, float fVerticalVelocity )
{
	// If changing direction
	if( eNewDirection != m_ePlayerDirection )
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

		m_ePlayerDirection = eNewDirection;
		m_eLastPlayerDirection = m_ePlayerDirection;
		
		UpdateMovement();
	}
}