////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer November 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjEugene.h"
//#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/Layers/CMLEugenesLair.h"

#ifndef TINYXML2_INCLUDED
    #include "external\tinyxml2\tinyxml2.h"
#endif

#ifndef _GCLEVELLOADER_OGMO_H_
    #include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

USING_NS_CC;

GCFACTORY_IMPLEMENT_CREATEABLECLASS( CGCObjEugene );

class  CMLEugenesLair;

CGCObjEugene::CGCObjEugene()
{
}

//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjEugene::~CGCObjEugene()
{
}

void CGCObjEugene::TriggerEugenesAlternativeAnimation(void)
{
	// When triggered, Eugene needs to provide an alternative animation.

	GetSprite ()->stopAllActions ();

	// Fetch the factory creation params and extract the plist for this object.
	const CGCFactoryCreationParams* const pcCreateParams = GetFactoryCreationParams();
	std::string m_pszPlist = pcCreateParams->strPlistFile;
	cocos2d::ValueMap& rdictPList = GCCocosHelpers::CreateDictionaryFromPlist(m_pszPlist);

	// Specifiy alternate animation name.
	
	m_pszAnimation = "Angry";

	pAnimation = GCCocosHelpers::CreateAnimation(rdictPList, m_pszAnimation);
	pAnimation->retain();
	RunAction(GCCocosHelpers::CreateAnimationActionLoop(pAnimation));

}

void CGCObjEugene::VOnReset()
{
	// Call base class version first.
	CGCObjEnemy::VOnReset();


	GetSprite ()->stopAllActions ();

	// Fetch the factory creation params and extract the plist for this object.
	const CGCFactoryCreationParams* const pcCreateParams = GetFactoryCreationParams();
	std::string m_pszPlist = pcCreateParams->strPlistFile;
	cocos2d::ValueMap& rdictPList = GCCocosHelpers::CreateDictionaryFromPlist(m_pszPlist);

	// Specifiy alternate animation name.

	m_pszAnimation = "Normal";

	pAnimation = GCCocosHelpers::CreateAnimation(rdictPList, m_pszAnimation);
	pAnimation->retain();
	RunAction(GCCocosHelpers::CreateAnimationActionLoop(pAnimation));

}

