#include <memory.h>

#include "AppDelegate.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GameController/GCController.h"


#include "Classes/ManicMiner/Player/CPlayer.h"
#include "ManicMiner/Layers/CManicLayer.h"

USING_NS_CC;


// action map arrays must match in length - in the templated controller class we use they map from the user define enum to cocos2d::Controller::Key 
static EPlayerActions			s_aePlayerActions[] = { EPA_AxisMove_X,								EPA_AxisMove_Y,								EPA_Jump };
static cocos2d::Controller::Key	s_aeKeys[] = { cocos2d::Controller::Key::JOYSTICK_LEFT_X,	cocos2d::Controller::Key::JOYSTICK_LEFT_Y,	cocos2d::Controller::Key::BUTTON_A };



//////////////////////////////////////////////////////////////////////////
// GetGCTypeIDOf uses the template in GCTypeID to generate a unique ID for 
// this type - need this to construct our base type
CPlayer::CPlayer()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CPlayer ) )
	, m_ePlayerDirection( EPlayerDirection::EPD_Static )
	, m_eLastPlayerDirection( EPlayerDirection::EPD_Static )
	, m_bCanJump( true )
	, m_bCanBeControlled( true )
	, m_bIsOnLadder( false )
	, m_iMaxLives( 3 )
	, m_iLives( m_iMaxLives )
	, m_v2Movement( 0.0f, 0.0f )
	, m_v2Jump( 0.0f, 0.0f )
	, m_fMovementSpeed( -4.f )
	, m_fJumpHeight( 3000.0f )
	, m_pcManicLayer(nullptr)
	, m_pcControllerActionToKeyMap( nullptr )
	, isalive(true)
{

}

CPlayer::CPlayer( CManicLayer& cLayer, cocos2d::Vec2 startingPos )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CPlayer ) )
	, m_ePlayerDirection( EPlayerDirection::EPD_Static )
	, m_eLastPlayerDirection( EPlayerDirection::EPD_Static )
	, m_bCanJump( true )
	, m_bIsOnLadder( false )
	, m_bCanBeControlled( true )
	, m_iMaxLives( 3 )
	, m_iLives( m_iMaxLives )
	, m_v2Movement( 0.0f, 0.0f )
	, m_v2Jump( 0.0f, 0.0f )
	, m_fMovementSpeed( -4.f )
	, m_pcControllerActionToKeyMap( nullptr )
	, m_pcManicLayer( &cLayer )
	, isalive( true )
{
	SetResetPosition( startingPos );
}

CPlayer::CPlayer( CManicLayer& cLayer, cocos2d::Vec2 startingPos, int startingLives )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CPlayer ) )
	, m_ePlayerDirection( EPlayerDirection::EPD_Static )
	, m_eLastPlayerDirection( EPlayerDirection::EPD_Static )
	, m_bCanJump( true )
	, m_bCanBeControlled( true )
	, m_iLives( startingLives )
	, m_v2Movement( 0.0f, 0.0f )
	, m_v2Jump( 0.0f, 0.0f )
	, m_fMovementSpeed( -4.f )
	, m_pcControllerActionToKeyMap( nullptr )
	, m_pcManicLayer( &cLayer )
	, isalive( true )
{
	SetResetPosition( startingPos );
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
IN_CPP_CREATION_PARAMS_DECLARE( CPlayer, "TexturePacker/Sprites/Mario/mario.plist", "mario", b2_dynamicBody, true );
//virtual 
void CPlayer::VOnResourceAcquire()
{
	IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CPlayer );

	CGCObjSpritePhysics::VOnResourceAcquire();

	const char* pszAnim_marioJog = "Jog";

	// animate!
	ValueMap dicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( dicPList, pszAnim_marioJog ) ) );


	// because we're just storing a vanilla pointer we must call delete on it in VOnResourceRelease or leak memory 
	// 
	// n.b. m_pcControllerActionToKeyMap is a "perfect use case" for std::unique_ptr...
	// 
	// n.n.b. ... however if we did use std::unique_ptr we'd need to use std::unique_ptr::reset in VOnResourceRelease if we wanted the memory allocate / free behaviour to be the same...
	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions, s_aeKeys );
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CPlayer::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();

	// reset velocity and flip state
	SetFlippedX( false );
	SetFlippedY( false );
	m_ePlayerDirection = EPlayerDirection::EPD_Static;
	m_eLastPlayerDirection = m_eLastPlayerDirection;
	isalive = true;

	// reset
	if (GetPhysicsBody())
	{
		Vec2 v2SpritePos = GetSpritePosition();
		GetPhysicsBody()->SetLinearVelocity( b2Vec2( 0.0f, 0.0f ) );
		GetPhysicsBody()->SetTransform( IGCGameLayer::B2dPixelsToWorld( b2Vec2( v2SpritePos.x, v2SpritePos.y ) ), 0.0f );
		GetPhysicsBody()->SetFixedRotation( true );
		GetPhysicsBody()->SetGravityScale( 30.0f );
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual
void CPlayer::VOnResourceRelease()
{
	CGCObjSpritePhysics::VOnResourceRelease();
	delete m_pcControllerActionToKeyMap;
	m_pcControllerActionToKeyMap = nullptr;
}


void CPlayer::TakeDamage()
{

	if (isalive)
	{
		m_iLives--;
		
		if (m_iLives > 0)
		{
			VOnReset();
			m_pcManicLayer->RequestReset();
		}

		if (m_iLives == 0)
		{
			m_pcManicLayer->OutOfLives();
		}
		isalive = false;

	}
	//if (CheckIfOutOfLives())
	//{
	//	Death();
	//	m_pcManicLayer->RequestReset();
	//}
	//
	//else
	//{
	//	m_pcManicLayer->OutOfLives();
	//}
}

bool CPlayer::CheckIfOutOfLives()
{
	if (m_iLives == 0)
	{
		return true;
	}
	return false;
}

void CPlayer::Death()
{
	Vec2 resetPos( 50.0f, 50.0f );
	SetResetPosition( resetPos );
}

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CPlayer::VOnUpdate( f32 fTimeStep )
{
	// handle movement
	KeyboardInput();
	UpdateMovement( fTimeStep );
}



void CPlayer::KeyboardInput()
{

	const CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();
	TGCController< EPlayerActions > cController = TGetActionMappedController( CGCControllerManager::eControllerOne, (*m_pcControllerActionToKeyMap) );

	// Can only jump (or move) if not already jumping
	if (m_bCanJump)
	{
		if ((pKeyManager->ActionIsPressed( CManicLayer::EPA_Jump )))
		{
			m_ePlayerDirection = EPlayerDirection::EPD_Jumping;
			m_bCanJump = false;
			CCLOG( "jump" );
		}


		else if ((pKeyManager->ActionIsPressed( CManicLayer::EPA_Left )))
		{
			if (m_bCanBeControlled)
			{
				ApplyDirectionChange( EPlayerDirection::EPD_Left, m_fMovementSpeed, 0.0f );
			}
		}

		else if ((pKeyManager->ActionIsPressed( CManicLayer::EPA_Right )))
		{
			if (m_bCanBeControlled)
			{
				ApplyDirectionChange( EPlayerDirection::EPD_Right, abs( m_fMovementSpeed ), 0.0f );
			}
		}

		else if (pKeyManager->ActionIsPressed( CManicLayer::EPA_Up ))
		{
			// Ladder Up
			if (m_bIsOnLadder)
			{
				ApplyDirectionChange( EPlayerDirection::EPD_Up, 0.0f, abs( m_fMovementSpeed ) );
			}
		}

		else if (pKeyManager->ActionIsPressed( CManicLayer::EPA_Down ))
		{
			// Ladder Down
			if (m_bIsOnLadder)
			{
				ApplyDirectionChange( EPlayerDirection::EPD_Down, 0.0f, m_fMovementSpeed );
			}
		}

		else
		{
			ApplyDirectionChange( EPlayerDirection::EPD_Static, 0.0f, 0.0f );
		}
	}
}


void CPlayer::ApplyDirectionChange( EPlayerDirection newDirection, float xOffSet, float yOffSet )
{
	const Vec2 v2Movement( xOffSet, yOffSet );
	m_v2Movement = v2Movement;
	m_ePlayerDirection = newDirection;
	m_eLastPlayerDirection = m_ePlayerDirection;
}


//////////////////////////////////////////////////////////////////////////
void CPlayer::UpdateMovement( f32 fTimeStep )
{
	/// Switch case used for the player movement
	/// 
	switch (m_ePlayerDirection)
	{
		case EPlayerDirection::EPD_Static:
			//
			SetVelocity( m_v2Movement );
			break;

		case EPlayerDirection::EPD_Right:
			SetFlippedX( true );
			SetVelocity( m_v2Movement );
			break;

		case EPlayerDirection::EPD_Left:
			SetFlippedX( false );
			SetVelocity( m_v2Movement );
			break;

		case EPlayerDirection::EPD_Up:
			SetVelocity( m_v2Movement );
			break;

		case EPlayerDirection::EPD_Down:
			SetVelocity( m_v2Movement );
			break;

			///	Jump Case
			/// The logic is simple here, essentially locks the jump to 3 things:
			/// Static: If no movement, will just be a static jump
			/// Left Lock: If player was moving to the left, the jump will be locked to the Left
			/// Right Lock: If player was moving to the right, the jump will be locked to the Right

		case EPlayerDirection::EPD_Jumping:
			//
			CCLOG( "Movement: Jump Initiated" );
			GetPhysicsBody()->SetGravityScale( m_kfGravitionalPull );

			if (m_eLastPlayerDirection == EPlayerDirection::EPD_Left)
			{
				JumpEvent( m_fMovementSpeed, m_fJumpHeight );
				CCLOG( "Movement: Jump Left" );
			}

			else if (m_eLastPlayerDirection == EPlayerDirection::EPD_Right)
			{
				JumpEvent( abs( m_fMovementSpeed ), m_fJumpHeight );
				CCLOG( "Movement: Jump Right" );
			}

			else
			{
				JumpEvent( 0.f, m_fJumpHeight );
				CCLOG( "Movement: Jump Static" );
			}
			break;

		case EPlayerDirection::EPD_Falling:
			m_bCanJump = false;
			m_bCanBeControlled = false;
			CCLOG( "Movement: Falling" );
			GetPhysicsBody()->SetGravityScale( m_kfGravitionalPull );
			break;

	}
}

void CPlayer::JumpEvent( float x, float y )
{
	Vec2 v2Jump( x, y );
	m_v2Jump = v2Jump;
	ApplyForceToCenter( m_v2Jump );
	m_ePlayerDirection = EPlayerDirection::EPD_Falling;
}

void CPlayer::SetGravity()
{
	GetPhysicsBody()->SetGravityScale( m_kfGravitionalPull );
}

