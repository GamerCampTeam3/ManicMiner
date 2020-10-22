////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CREGULARPLATFORM_H_
#include "CRegularPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

CRegularPlatform::CRegularPlatform( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition )
	: CPlatform(CreationParams, ResetPosition )
{

}

void CRegularPlatform::VOnResourceAcquire()
{
	CPlatform::VOnResourceAcquire();
}

void CRegularPlatform::VOnReset()
{
	CPlatform::VOnReset();
}