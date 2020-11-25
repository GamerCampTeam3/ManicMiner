////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjLander.h"

//#ifndef TINYXML2_INCLUDED
//    #include "external\tinyxml2\tinyxml2.h"
//#endif

//#ifndef _GCLEVELLOADER_OGMO_H_
//    #include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
//#endif

USING_NS_CC;




//GCFACTORY_IMPLEMENT_CREATEABLECLASS( CGCObjLander );

//CGCObjLander::CGCObjLander()
	//: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjLander))
	//, m_pCustomCreationParams(nullptr)
//{
	//m_fMoveDelta = 0.0;
//}

//CGCObjLander::CGCObjLander(GCTypeID idDerivedType)
	//: m_pCustomCreationParams(nullptr)
//{
	//m_fMoveDelta = 0.0;

//}



CGCObjLander::CGCObjLander(const cocos2d::Vec2& rcAnchorPoint, const cocos2d::Vec2& rcDestinationPoint, const float fSpeed, const float fStartDelay, const float fReDeployDelay)
{


}



//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjLander::~CGCObjLander()
{
	
}

//////////////////////////////////////////////////////////////////////////
// This function initialises the enemies velocity, reset position and 
// initial facing direction.
//////////////////////////////////////////////////////////////////////////
// virtual function
//////////////////////////////////////////////////////////////////////////
void CGCObjLander::VOnResourceAcquire( void )
{

    // Removed maco call so the reference m_rFactorCreationParams could be passed 
	// into VHandleFactoryParms.  Pending module 2 framework his may be done differently.
	//IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjLander );    
	//VHandleFactoryParams(m_rFactoryCreationParams, GetResetPosition());

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




	
	m_cAnchorPoint = GetResetPosition();

	m_cCurrentPos = m_cAnchorPoint;

	SetResetPosition(m_cAnchorPoint);


}

void CGCObjLander::VOnReset()
{
	// Call base class version first.
	CGCObjSpritePhysics::VOnReset();


}






//////////////////////////////////////////////////////////////////////////
// This function is called when an enemy is resurected from the dead-list to the 
// live list.
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjLander::VOnResurrected( void )
{
	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetGravityScale( 0.0f );
}

//////////////////////////////////////////////////////////////////////////
//Function to provide the frame update of this object
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjLander::VOnUpdate(float fTimeStep)
{
	// Call base class version first.
	CGCObject::VOnUpdate(fTimeStep);
	   	 



	// Calulate a vector to represent the movement window.
	cocos2d::Vec2 fVectorWindow = m_cAnchorPoint - m_cDest;


	// Calculate movement % increment amount as a function of the movment window length and the frame rate * speed modifier.
	// This value is the input into the LERP function to calculate m_cCurrentPos.
	float fLerpInput = ((fTimeStep * m_fSpeed) / fVectorWindow.length()) * 100.0f;


	m_fMoveDelta += fLerpInput;


	m_cCurrentPos = m_cAnchorPoint.lerp(m_cDest, m_fMoveDelta);





	if (m_fMoveDelta > 0.0f && m_fMoveDelta < 1.0f)
	{
		MoveToPixelPosition(m_cCurrentPos);
	}
	else
	{
		// Crash logic

		
		
	}
}





void CGCObjLander::VOnResourceRelease()
{
	// call base class first.
	CGCObjSpritePhysics::VOnResourceRelease();


	if (m_pszAnimation.length() > 0)
	{
		pAnimation->release();
		pAnimation = nullptr;

	}
}

