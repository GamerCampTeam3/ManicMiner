////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <memory.h>

#include "AppDelegate.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GameSpecific/Player/GCObjProjectilePlayer.h"
#include "GamerCamp/GameSpecific/Player/GCObjGroupProjectilePlayer.h"
#include "GamerCamp/GameController/GCController.h"


#include "CPlayer.h"

USING_NS_CC;


// action map arrays must match in length - in the templated controller class we use they map from the user define enum to cocos2d::Controller::Key 
static EPlayerActions			s_aePlayerActions[] = { EPA_AxisMove_X,								EPA_AxisMove_Y,								EPA_Jump };
static cocos2d::Controller::Key	s_aeKeys[] = { cocos2d::Controller::Key::JOYSTICK_LEFT_X,	cocos2d::Controller::Key::JOYSTICK_LEFT_Y,	cocos2d::Controller::Key::BUTTON_A };



//////////////////////////////////////////////////////////////////////////
// GetGCTypeIDOf uses the template in GCTypeID to generate a unique ID for 
// this type - need this to construct our base type
CPlayer::CPlayer()
	: CGCObjSpritePhysics(GetGCTypeIDOf(CPlayer))
	, m_ePlayerDirection(EPlayerDirection::EPD_Static)
	, m_eLastPlayerDirection(EPlayerDirection::EPD_Static)
	, m_bCanJump(true)
	, m_bCanBeControlled(true)
	, m_iLives(m_kiStartingLives)
	, m_v2MovementLeft(-10.0f, 0.0f)
	, m_v2MovementRight(10.0f, 0.0f)
	, m_v2MovementStatic(0.0f, 0.0f)
	, m_v2JumpStatic(0.0f, 2000.0f)
	, m_v2JumpRight(100.0f, 2000.0f)
	, m_v2JumpLeft(-010.0f, 2000.0f)
	, m_pcControllerActionToKeyMap(nullptr)
{

}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
IN_CPP_CREATION_PARAMS_DECLARE(CPlayer, "TexturePacker/Sprites/Mario/mario.plist", "mario", b2_dynamicBody, true);
//virtual 
void CPlayer::VOnResourceAcquire()
{
	IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE(CPlayer);

	CGCObjSpritePhysics::VOnResourceAcquire();

	const char* pszAnim_marioJog = "Jog";

	// animate!
	ValueMap dicPList = GCCocosHelpers::CreateDictionaryFromPlist(GetFactoryCreationParams()->strPlistFile);
	RunAction(GCCocosHelpers::CreateAnimationActionLoop(GCCocosHelpers::CreateAnimation(dicPList, pszAnim_marioJog)));


	// because we're just storing a vanilla pointer we must call delete on it in VOnResourceRelease or leak memory 
	// 
	// n.b. m_pcControllerActionToKeyMap is a "perfect use case" for std::unique_ptr...
	// 
	// n.n.b. ... however if we did use std::unique_ptr we'd need to use std::unique_ptr::reset in VOnResourceRelease if we wanted the memory allocate / free behaviour to be the same...
	m_pcControllerActionToKeyMap = TCreateActionToKeyMap(s_aePlayerActions, s_aeKeys);
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CPlayer::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();

	// reset velocity and flip state
	SetFlippedX(false);
	SetFlippedY(false);

	// reset
	if (GetPhysicsBody())
	{
		Vec2 v2SpritePos = GetSpritePosition();
		GetPhysicsBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		GetPhysicsBody()->SetTransform(IGCGameLayer::B2dPixelsToWorld(b2Vec2(v2SpritePos.x, v2SpritePos.y)), 0.0f);
		GetPhysicsBody()->SetFixedRotation(true);
		GetPhysicsBody()->SetGravityScale(30.0f);
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



//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CPlayer::VOnUpdate(f32 fTimeStep)
{
	// handle movement
	KeyboardInput();
	UpdateMovement(fTimeStep);
}



void CPlayer::KeyboardInput()
{

	const CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();
	TGCController< EPlayerActions > cController = TGetActionMappedController(CGCControllerManager::eControllerOne, (*m_pcControllerActionToKeyMap));

	// Can only jump (or move) if not already jumping
	if (m_bCanJump)
	{
		if ((pKeyManager->ActionIsPressed(CGCGameLayerPlatformer::EPA_Jump)))
		{

			m_ePlayerDirection = EPlayerDirection::EPD_Jumping;
			m_bCanJump = false;
		}


		else if ((pKeyManager->ActionIsPressed(CGCGameLayerPlatformer::EPA_Left)))
		{
			if (m_bCanBeControlled)
			{
				m_ePlayerDirection = EPlayerDirection::EPD_Left;
				m_eLastPlayerDirection = m_ePlayerDirection;
			}
		}

		else if ((pKeyManager->ActionIsPressed(CGCGameLayerPlatformer::EPA_Right)))
		{
			if (m_bCanBeControlled)
			{
				m_ePlayerDirection = EPlayerDirection::EPD_Right;
				m_eLastPlayerDirection = m_ePlayerDirection;
			}
		}

		else if (pKeyManager->ActionIsPressed(CGCGameLayerPlatformer::EPA_Up))
		{
			// Ladder Up
			if (m_bIsOnLadder)
			{

			}
		}

		else if (pKeyManager->ActionIsPressed(CGCGameLayerPlatformer::EPA_Down))
		{
			// Ladder Down
			if (m_bIsOnLadder)
			{

			}
		}

		else
		{

			m_ePlayerDirection = EPlayerDirection::EPD_Static;
			m_eLastPlayerDirection = m_ePlayerDirection;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CPlayer::UpdateMovement(f32 fTimeStep)
{
	/// Switch case used for the player movement
	/// 
	switch (m_ePlayerDirection)
	{

	case EPlayerDirection::EPD_Left:
		//
		SetFlippedX(false);
		CCLOG("Movement: Left");
		SetVelocity(m_v2MovementLeft);
		break;

	case EPlayerDirection::EPD_Right:
		//
		SetFlippedX(true);
		CCLOG("Movement: Right");
		SetVelocity(m_v2MovementRight);
		break;

	case EPlayerDirection::EPD_Static:
		//
		SetVelocity(m_v2MovementStatic);
		break;

		///	Jump Case
		/// The logic is simple here, essentially locks the jump to 3 things:
		/// Static: If no movement, will just be a static jump
		/// Left Lock: If player was moving to the left, the jump will be locked to the Left
		/// Right Lock: If player was moving to the right, the jump will be locked to the Right

	case EPlayerDirection::EPD_Jumping:
		//
		CCLOG("Movement: Jump Initiated");
		GetPhysicsBody()->SetGravityScale(30.0f);

		if (m_eLastPlayerDirection == EPlayerDirection::EPD_Left)
		{
			CCLOG("Jump: Left Lock");
			ApplyForceToCenter(m_v2JumpStatic);
			m_ePlayerDirection = EPlayerDirection::EPD_Falling;
		}

		else if (m_eLastPlayerDirection == EPlayerDirection::EPD_Right)
		{
			CCLOG("Jump: Right Lock");
			ApplyForceToCenter(m_v2JumpRight);
			m_ePlayerDirection = EPlayerDirection::EPD_Falling;
		}

		else
		{
			CCLOG("Jump: Left Lock");
			ApplyForceToCenter(m_v2JumpLeft);
			m_ePlayerDirection = EPlayerDirection::EPD_Falling;
		}
		break;

	case EPlayerDirection::EPD_Falling:
		m_bCanJump = false;
		CCLOG("Movement: Falling");
		GetPhysicsBody()->SetGravityScale(30.0f);
		break;

	case EPlayerDirection::EPD_Ladder:
		CCLOG("Movement: Ladder");

	default:
		//
		break;
	}
}