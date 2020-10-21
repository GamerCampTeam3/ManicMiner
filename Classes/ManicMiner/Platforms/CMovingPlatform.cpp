////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CMOVINGPLATFORM_H_
#include "CMovingPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

CMovingPlatform::CMovingPlatform(CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition )
	: CPlatform( CreationParams, ResetPosition )
{
	m_ePlatformType = EPlatformType::EPT_Moving;
}

void CMovingPlatform::VOnResourceAcquire()
{
	CPlatform::VOnResourceAcquire();
}

void CMovingPlatform::VOnReset()
{
	CPlatform::VOnReset();
}


