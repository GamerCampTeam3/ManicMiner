////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CSUNLIGHT_H
#include "CSunlight.h"
#endif

CSunlight::CSunlight()
	: CGCObjSpritePhysics(GetGCTypeIDOf(CSunlight))
{
	
}

void CSunlight::VOnResourceAcquire()
{
	CGCObjSpritePhysics::VOnResourceAcquire();
}

void CSunlight::VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition)
{
	CGCObjSpritePhysics::VHandleFactoryParams(rCreationParams, v2InitialPosition);
}

void CSunlight::VOnResourceRelease()
{
	CGCObjSpritePhysics::VOnResourceRelease();
}

void CSunlight::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
}

