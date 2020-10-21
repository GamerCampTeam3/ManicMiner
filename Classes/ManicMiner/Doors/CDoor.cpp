#include "ManicMiner/Doors/CDoor.h"
#include "ManicMiner/Layers/CManicLayer.h"



CDoor::CDoor(CManicLayer& cLayer, CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition)
	: CGCObjSpritePhysics( GetGCTypeIDOf( CDoor ) )
	, m_cManicLayer( cLayer )
	, m_FactoryCreationParams( CreationParams )
	, m_v2ResetPosition( ResetPosition )
{
	
}




void CDoor::InteractEvent()
{
	if ( m_cManicLayer.GetGameState() == EGameState::EGS_Escaping )
	{
		m_cManicLayer.RequestNextLevel();
	}
}


void CDoor::VOnResourceAcquire()
{
	VHandleFactoryParams( m_FactoryCreationParams, GetResetPosition() );
	CGCObjSpritePhysics::VOnResourceAcquire();
	SetResetPosition( m_v2ResetPosition );
}

void CDoor::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
}

