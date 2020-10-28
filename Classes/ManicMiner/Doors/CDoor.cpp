#include "ManicMiner/Doors/CDoor.h"
#include "ManicMiner/Layers/CManicLayer.h"

// Constructor that initializes all values.
// The references are fed in by the door managers.
CDoor::CDoor(CManicLayer& cLayer, CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition)
	: CGCObjSpritePhysics( GetGCTypeIDOf( CDoor ) )
	, m_cManicLayer						( cLayer )
	, m_FactoryCreationParams			( CreationParams )
	, m_v2ResetPosition					( ResetPosition )
{	
}


// Checks if player is escaping and if so request the next level
void CDoor::InteractEvent()
{
	if ( m_cManicLayer.GetGameState() == EGameState::Escaping )
	{
		m_cManicLayer.RequestNextLevel();
	}
}

// Sets the sprites as well as reset position
void CDoor::VOnResourceAcquire()
{
	VHandleFactoryParams( m_FactoryCreationParams, GetResetPosition() );
	CGCObjSpritePhysics::VOnResourceAcquire();
	SetResetPosition( m_v2ResetPosition );
}

// Calls parent Reset 
void CDoor::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
}