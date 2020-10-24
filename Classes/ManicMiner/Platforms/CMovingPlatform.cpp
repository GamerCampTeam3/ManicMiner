////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CMOVINGPLATFORM_H_
#include "CMovingPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

CMovingPlatform::CMovingPlatform( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition, const EPlayerDirection eDirectionLock )
	: CPlatform( CreationParams, ResetPosition )
	, m_pcMovingAnim ( nullptr )
	, m_eDirectionLock ( eDirectionLock )
{
	m_ePlatformType = EPlatformType::EPT_Moving;
}

void CMovingPlatform::VOnResourceAcquire()
{
	CPlatform::VOnResourceAcquire();

	const char* pszAnim_Move = "MoveLeft";

	cocos2d::ValueMap& rdictPlist = GCCocosHelpers::CreateDictionaryFromPlist(m_FactoryCreationParams.strPlistFile);
	m_pcMovingAnim = GCCocosHelpers::CreateAnimation(rdictPlist, pszAnim_Move);
	RunAction(GCCocosHelpers::CreateAnimationActionLoop(m_pcMovingAnim));
}

void CMovingPlatform::VOnReset()
{
	CPlatform::VOnReset();
}

const EPlayerDirection CMovingPlatform::GetDirectionLock()
{
	return m_eDirectionLock;
}