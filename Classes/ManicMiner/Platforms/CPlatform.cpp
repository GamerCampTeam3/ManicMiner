////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CPLATFORM_H
#include "CPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"


CPlatform::CPlatform()
	: CGCObjSpritePhysics(GetGCTypeIDOf(CPlatform))
	, m_pcCustomCreationParams	( nullptr )
{
	
}

CPlatform::~CPlatform()
{
	if (nullptr != m_pcDirector)
	{
		m_pcDirector = nullptr;
	}
}

void CPlatform::VOnResourceAcquire()
{	
	CGCObjSpritePhysics::VOnResourceAcquire();

	m_pcDirector = cocos2d::Director::getInstance();

}

void CPlatform::VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition)
{

	CGCObjSpritePhysics::VHandleFactoryParams(rCreationParams, v2InitialPosition);
}

void CPlatform::VOnResourceRelease()
{
	CGCObjSpritePhysics::VOnResourceRelease();
	
	m_v2ResetPosition = cocos2d::Vec2(0.f, 0.f);
	m_bCollisionEnabled = false;
	m_bTriggersHardContactEvent = false;
}


void CPlatform::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();

	m_v2ResetPosition = cocos2d::Vec2(0.f, 0.f);
	m_bCollisionEnabled = false;
	m_bTriggersHardContactEvent = false;
	m_bIsInContact = false;
	m_bIsSensorOverlapped = false;
	m_bIsTriggeringHardContact = false;
}

bool CPlatform::GetCollisionEnabled() const
{
	return m_bCollisionEnabled;
}

void CPlatform::SetCollisionEnabled( bool bCollisionEnabled )
{
	m_bCollisionEnabled = bCollisionEnabled;
}

bool CPlatform::GetTriggersHardContactEvent() const
{
	return m_bTriggersHardContactEvent;
}

void CPlatform::SetTriggersHardContactEvent( bool bShouldTrigger )
{
	m_bTriggersHardContactEvent = bShouldTrigger;
}

bool CPlatform::GetIsInContact() const
{
	return m_bIsInContact;
}

void CPlatform::SetIsInContact( bool bIsInContact )
{
	m_bIsInContact = bIsInContact;
}

bool CPlatform::GetIsSensorOverlapped() const
{
	return m_bIsSensorOverlapped;
}

void CPlatform::SetIsSensorOverlapped( bool bIsSensorOverlapped )
{
	m_bIsSensorOverlapped = bIsSensorOverlapped;
}

bool CPlatform::GetIsTriggeringHardContact() const
{
	return m_bIsTriggeringHardContact;
}

void CPlatform::SetIsTriggeringHardContact( bool bIsTriggeringHardContact )
{
	m_bIsTriggeringHardContact = bIsTriggeringHardContact;
}
