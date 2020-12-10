////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer December 2020 - Module 2
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjKong.h"

USING_NS_CC;

CGCObjKong::CGCObjKong(const cocos2d::Vec2& rcAnchorPoint, const cocos2d::Vec2& rcDestinationPoint, const float fSpeed, CGCFactoryCreationParams& ParamsInput)
	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjKong))
    , m_rFactoryCreationParams(ParamsInput)
	, m_cAnchorPoint(rcAnchorPoint)
{
	m_cKongState		= EKongState::EWaitingToFall;
	m_currentTime		= 0.0f;
	m_bKongIsFalling	= false;
}

//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjKong::~CGCObjKong()
{
}

//////////////////////////////////////////////////////////////////////////
// This function initialises the enemies velocity, reset position and 
// initial facing direction.
//////////////////////////////////////////////////////////////////////////
// virtual function
//////////////////////////////////////////////////////////////////////////
void CGCObjKong::VOnResourceAcquire( void )
{
	VHandleFactoryParams(m_rFactoryCreationParams, GetResetPosition());

	// Call base class verion.
	CGCObjSpritePhysics::VOnResourceAcquire();
		   	 
	if (m_pszAnimation.length() > 0)
	{
		const CGCFactoryCreationParams* const pcCreateParams = GetFactoryCreationParams();
		std::string m_pszPlist = pcCreateParams->strPlistFile;

		cocos2d::ValueMap& rdictPList = GCCocosHelpers::CreateDictionaryFromPlist(m_pszPlist);
		pAnimation = GCCocosHelpers::CreateAnimation(rdictPList, m_pszAnimation);
		pAnimation->retain();
		RunAction(GCCocosHelpers::CreateAnimationActionLoop(pAnimation));
	}

	SetResetPosition(m_cAnchorPoint);
}

void CGCObjKong::VOnReset()
{
	// Call base class version first.
	CGCObjSpritePhysics::VOnReset();
	m_bKongIsFalling = false;
	SetFlippedY( false );
}

//////////////////////////////////////////////////////////////////////////
// This function is called when an enemy is resurected from the dead-list to the 
// live list.
//////////////////////////////////////////////////////////////////////////
//virtual function from immediate base class
void CGCObjKong::VOnResurrected( void )
{
	// Call base class version.
	CGCObjSpritePhysics::VOnResurrected();

	GetPhysicsBody()->SetGravityScale( 0.0f );
}

//////////////////////////////////////////////////////////////////////////
//Function to provide the frame update of this object
//////////////////////////////////////////////////////////////////////////
//virtual function from CGCObject base class.
void CGCObjKong::VOnUpdate( float fTimeStep )
{
	// Call base class version first.
	CGCObject::VOnUpdate(fTimeStep);

	// Kong remains inactive until the m_bKongIsFalling is set true.
	if (m_bKongIsFalling)
	{
		// Flip his sprite to falling position and increase his velocity.
		SetFlippedY(true);
		SetVelocity(cocos2d::Vec2(0.0f, m_kfKongFallVelocity ));

		// increment his position for a short time to ensure Kong off the bottom of the screen, then halt so never re-appears.
		m_currentTime += fTimeStep;
		if (m_currentTime > m_kfKongFallDuration)
		{
			// Hold his position just off bottom of the screen to avoid him wrapping around onto the top again!
			SetVelocity(cocos2d::Vec2(0.0f, 0.0f));
			SetResetPosition(cocos2d::Vec2(-100.0f, -100.0f));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Function to perform any resource releasing tasks.
// (Virtual function overriding immediate base class.)
///////////////////////////////////////////////////////////////////////////////////////
void CGCObjKong::VOnResourceRelease()
{
	// call base class first.
	CGCObjSpritePhysics::VOnResourceRelease();
	
	// Release animation if one was specified.
	if (m_pszAnimation.length() > 0)
	{
		pAnimation->release();
		pAnimation = nullptr;
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Public function to trigger Kong's falling logic.
///////////////////////////////////////////////////////////////////////////////////////
void CGCObjKong::TriggerKongToFall()
{
	m_bKongIsFalling = true;
}