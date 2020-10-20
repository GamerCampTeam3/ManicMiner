////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CPLATFORM_H
#include "CPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"


CPlatform::CPlatform(CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition, EPlatformType PlatformType)
	: CGCObjSpritePhysics(GetGCTypeIDOf(CPlatform))
	, m_FactoryCreationParams(CreationParams)
	, m_v2ResetPosition(ResetPosition)
	, m_ePlatformType( PlatformType )
	, m_bCollisionEnabled( false )
{
}

void CPlatform::VOnResourceAcquire()
{
	VHandleFactoryParams(m_FactoryCreationParams, GetResetPosition());
	
	CGCObjSpritePhysics::VOnResourceAcquire();

	SetResetPosition(m_v2ResetPosition);

	
}