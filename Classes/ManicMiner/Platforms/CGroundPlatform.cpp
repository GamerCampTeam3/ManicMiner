////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CGROUNDPLATFORM_H_
#include "CGroundPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

CGroundPlatform::CGroundPlatform(CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition, EPlatformType PlatformType)
	: CPlatform(CreationParams, ResetPosition, PlatformType)
{

}

void CGroundPlatform::VOnResourceAcquire()
{
	CPlatform::VOnResourceAcquire();
}

void CGroundPlatform::VOnReset()
{
	CPlatform::VOnReset();
}