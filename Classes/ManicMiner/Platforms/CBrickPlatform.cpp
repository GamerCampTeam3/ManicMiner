////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CBRICKPLATFORM_H_
#include "CBrickPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

CBrickPlatform::CBrickPlatform( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition )
	: CPlatform( CreationParams, ResetPosition )
{
	m_ePlatformType = EPlatformType::EPT_Brick;
}

void CBrickPlatform::VOnResourceAcquire()
{
	CPlatform::VOnResourceAcquire();
}

void CBrickPlatform::VOnReset()
{
	CPlatform::VOnReset();
	m_bCollisionEnabled = true;
	m_bTriggersHardContactEvent = true;
}