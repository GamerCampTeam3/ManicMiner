////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CPLATFORM_H
#include "CPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"


CPlatform::CPlatform( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition )
	: CGCObjSpritePhysics(GetGCTypeIDOf(CPlatform))
	, m_FactoryCreationParams(CreationParams)
	, m_v2ResetPosition(ResetPosition)
	, m_ePlatformType( EPlatformType::EPT_Regular )
	, m_bCollisionEnabled( false )
	, m_bTriggersHardContactEvent ( false )

{
}

void CPlatform::VOnResourceAcquire()
{
	VHandleFactoryParams(m_FactoryCreationParams, GetResetPosition());
	
	CGCObjSpritePhysics::VOnResourceAcquire();

	SetResetPosition(m_v2ResetPosition);

	m_pcDirector = cocos2d::Director::getInstance();

}

void CPlatform::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();

	m_v2ResetPosition = cocos2d::Vec2(0.f, 0.f);
	m_bCollisionEnabled = false;
	m_bTriggersHardContactEvent = false;
}

EPlatformType CPlatform::GetPlatformType()
{
	return m_ePlatformType;
}

bool CPlatform::GetCollisionEnabled()
{
	return m_bCollisionEnabled;
}

void CPlatform::SetCollisionEnabled( bool bCollisionEnabled )
{
	m_bCollisionEnabled = bCollisionEnabled;
}

bool CPlatform::GetTriggersHardContactEvent()
{
	return m_bTriggersHardContactEvent;
}

void CPlatform::SetTriggersHardContactEvent( bool bShouldTrigger )
{
	m_bTriggersHardContactEvent = bShouldTrigger;
}