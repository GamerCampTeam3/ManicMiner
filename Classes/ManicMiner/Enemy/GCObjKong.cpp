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


	//SetSpritePosition(Vec2(0.0f, 0.0f));

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

    // Removed maco call so the reference m_rFactorCreationParams could be passed 
	// into VHandleFactoryParms.  Pending module 2 framework his may be done differently.
	//IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjKong );    
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


		// use below as data driven from OGMO to set animation speed if required?
		//pAnimation->setDelayPerUnit(0.0f);

	}



	
	m_cCurrentPos = m_cAnchorPoint;

	SetResetPosition(m_cAnchorPoint);


	m_fMoveDelta = 0.0f;

}

void CGCObjKong::VOnReset()
{
	// Call base class version first.
	CGCObjSpritePhysics::VOnReset();


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
	   	 



}





void CGCObjKong::VOnResourceRelease()
{
	// call base class first.
	CGCObjSpritePhysics::VOnResourceRelease();


	if (m_pszAnimation.length() > 0)
	{
		pAnimation->release();
		pAnimation = nullptr;

	}
}

