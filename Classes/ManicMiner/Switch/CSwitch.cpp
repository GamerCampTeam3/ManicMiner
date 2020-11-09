#include "CSwitch.h"

#include "ManicMiner/CollectiblesGroup/CCollectiblesGroup.h"

CSwitch::CSwitch( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition, CCollectiblesGroup& CollectibleGroup )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CSwitch ) )
	, m_FactoryCreationParams	( CreationParams )
	, m_bHasBeenCollected		( false )
	, m_v2ResetPosition			( ResetPosition )
	, m_pcCollectiblesGroup		( &CollectibleGroup )
{
}


void CSwitch::VOnResourceAcquire( void )
{
	VHandleFactoryParams( m_FactoryCreationParams, GetResetPosition() );
	CGCObjSpritePhysics::VOnResourceAcquire();
	SetResetPosition( m_v2ResetPosition );
}


void CSwitch::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
	m_bHasBeenCollected = false;
}

// Main Interact Event
void CSwitch::InteractEvent()
{
	if (!m_bHasBeenCollected)
	{
		m_pcCollectiblesGroup->SwitchEvent();
		SetFlippedX( true );

		// Stops collisions from happening again.
		m_bHasBeenCollected = true;
	}
}
