////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjHazard.h"

#ifndef TINYXML2_INCLUDED
#include "external\tinyxml2\tinyxml2.h"
#endif

#ifndef _GCLEVELLOADER_OGMO_H_
#include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

USING_NS_CC;

GCFACTORY_IMPLEMENT_CREATEABLECLASS(CGCObjHazard);

//Module 1 
//CGCObjHazard::CGCObjHazard(const cocos2d::Vec2& rcAnchorPoint, const HazardTypes::EHazardId eHazardId, CGCFactoryCreationParams& rcFactoryCreationParamsInput)
//	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjHazard))
//	, m_rFactoryCreationParams(rcFactoryCreationParamsInput)
//	, m_cAnchorPoint(rcAnchorPoint)
//	, m_eHazardIdentifier(eHazardId)
//{
//}

CGCObjHazard::CGCObjHazard()
	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjHazard))
	, m_pCustomCreationParams(nullptr)
	, m_bCanCollide(true)
{
}

//////////////////////////////////////////////////////////////////////////
// This function initialises an enemies direction of velocity, 
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjHazard::VOnResourceAcquire( void )
{
	// Removed maco call so the reference m_rFactorCreationParams could be passed 
	// into VHandleFactoryParms.  Pending module 2 framework his may be done differently.
	//IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjHazard );    
	//VHandleFactoryParams(m_rFactoryCreationParams, GetResetPosition());

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



















	//SetResetPosition(m_cAnchorPoint);
}



void CGCObjHazard::VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition)
{

	const CGCFactoryCreationParams* pParamsToPassToBaseClass = &rCreationParams;

	//Fetch a pointer into the OGMO Xml edtior element containing the data.
	const tinyxml2::XMLElement* pCurrentObjectXmlData = CGCLevelLoader_Ogmo::GetCurrentObjectXmlData();

	// Read in the custom plist
	if (nullptr != pCurrentObjectXmlData)
	{
		// Animation name	
		const tinyxml2::XMLAttribute* pAnimationName = pCurrentObjectXmlData->FindAttribute("AnimationName");
		CCLOG((nullptr == pAnimationName) ? "AnimationName not found for Hazard!" : pAnimationName->Value());
		m_pszAnimation = pAnimationName->Value();


		// Custom Plist and shape.

		const tinyxml2::XMLAttribute* pCustomPlistPath = pCurrentObjectXmlData->FindAttribute("CustomPlist");
		const tinyxml2::XMLAttribute* pCustomShapePath = pCurrentObjectXmlData->FindAttribute("CustomShape");
		
		if ((nullptr != pCustomPlistPath)
			&& (0 != strlen(pCustomPlistPath->Value())))
		{
			m_pCustomCreationParams = std::make_unique< CGCFactoryCreationParams >(rCreationParams.strClassName.c_str(),
				pCustomPlistPath->Value(),
				pCustomShapePath->Value(),
				rCreationParams.eB2dBody_BodyType,
				rCreationParams.bB2dBody_FixedRotation);

			pParamsToPassToBaseClass = m_pCustomCreationParams.get();
		}

	}

	// Call base class version 	
	CGCObjSpritePhysics::VHandleFactoryParams((*pParamsToPassToBaseClass), v2InitialPosition);

}









//////////////////////////////////////////////////////////////////////////
// This function is called when an hazard is resurected from the dead-list to the 
// live list.
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjHazard::VOnResurrected( void )
{
	CGCObjSpritePhysics::VOnResurrected();
	m_bCanCollide = true;
	GetPhysicsBody()->SetGravityScale( 0.0f );
}

//////////////////////////////////////////////////////////////////////////
// Function to provide the frame update of this object
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjHazard::VOnUpdate(float fTimeStep)
{
	// Call base class version first.
	CGCObject::VOnUpdate(fTimeStep);

	// D.O'DWYER - THIS FUNCTION IS NOT REQUIRED AND SHOULD BE REMOVED AT MODULE 2.
}

void CGCObjHazard::VOnResourceRelease()
{
	// call base class first.
	CGCObjSpritePhysics::VOnResourceRelease();

	if (m_pszAnimation.length() > 0)
	{
		pAnimation->release();
		pAnimation = nullptr;
	}
}
