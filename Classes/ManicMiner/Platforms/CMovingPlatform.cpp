////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CMOVINGPLATFORM_H_
#include "CMovingPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"


#ifndef TINYXML2_INCLUDED
#include "external\tinyxml2\tinyxml2.h"
#endif




#ifndef _GCLEVELLOADER_OGMO_H_
#include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

#include "ManicMiner/Enums/EPlayerMovement.h"

GCFACTORY_IMPLEMENT_CREATEABLECLASS(CMovingPlatform);

CMovingPlatform::CMovingPlatform()
	: CPlatform()
	, m_pcMovingAnim ( nullptr )
{
	m_ePlatformType = EPlatformType::Moving;
}

void CMovingPlatform::InitDirectionalLock(const EPlayerDirection eDirectionLock)
{
	m_eDirectionLock = eDirectionLock;
}

void CMovingPlatform::VOnResourceAcquire()
{
	CPlatform::VOnResourceAcquire();

	const char* pszAnim_Move = "MoveLeft";

	cocos2d::ValueMap& rdictPlist = GCCocosHelpers::CreateDictionaryFromPlist(GetFactoryCreationParams()->strPlistFile);
	m_pcMovingAnim = GCCocosHelpers::CreateAnimation(rdictPlist, pszAnim_Move);
	RunAction(GCCocosHelpers::CreateAnimationActionLoop(m_pcMovingAnim));

	InitDirectionalLock(EPlayerDirection::Left);
}

void CMovingPlatform::VOnReset()
{
	CPlatform::VOnReset();
}


const EPlayerDirection CMovingPlatform::GetDirectionLock()
{
	return m_eDirectionLock;
}