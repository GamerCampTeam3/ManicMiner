////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjKong.h"

USING_NS_CC;

CGCObjKong::CGCObjKong(const cocos2d::Vec2& rcAnchorPoint, const cocos2d::Vec2& rcDestinationPoint, const float fSpeed, CGCFactoryCreationParams& ParamsInput)
	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjKong))
    , m_rFactoryCreationParams(ParamsInput)
	, m_cAnchorPoint(rcAnchorPoint)
	, m_cDest(rcDestinationPoint)
	, m_fSpeed(fSpeed)

{
	m_cKongState = EKongState::EWaitingToFall;
	m_currentTime = 0.0f;
	m_bKongIsFalling = false;
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

		// Note m_pszAnimation is sourced from the data file so not set here.
		cocos2d::ValueMap& rdictPList = GCCocosHelpers::CreateDictionaryFromPlist(m_pszPlist);
		pAnimation = GCCocosHelpers::CreateAnimation(rdictPList, m_pszAnimation);
		pAnimation->retain();
		RunAction(GCCocosHelpers::CreateAnimationActionLoop(pAnimation));
	}

	m_cCurrentPos = m_cAnchorPoint;
	SetResetPosition(m_cAnchorPoint);
	m_fMoveDelta = 0.0f;
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
//virtual function
void CGCObjKong::VOnResurrected( void )
{
	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetGravityScale( 0.0f );
}

//////////////////////////////////////////////////////////////////////////
//Function to provide the frame update of this object
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjKong::VOnUpdate(float fTimeStep)
{
	// Call base class version first.
	CGCObject::VOnUpdate(fTimeStep);

	// Note this would be an ideal candiate to be event driven rather than VOnUpdate!

	if (m_bKongIsFalling)
	{
		// Flip his sprite to falling position and increase his velocity.
		SetFlippedY(true);
		SetVelocity(cocos2d::Vec2(0.0f, -10.0f));

		// increment his position for a short time to ensure off bottom of screen, then halt so never re-appears.
		m_currentTime += fTimeStep;
		if (m_currentTime > m_kfKongFallDuration)
		{
			SetVelocity(cocos2d::Vec2(0.0f, 0.0f));
			SetResetPosition(cocos2d::Vec2(-100.0f, -100.0f));
		}
	}
}

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


void CGCObjKong::TriggerKongToFall()
{
	m_bKongIsFalling = true;
}