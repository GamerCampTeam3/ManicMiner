#include "CBlock.h"

CBlock::CBlock( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition )
  : CGCObjSpritePhysics( GetGCTypeIDOf( CBlock ) )
   , m_rFactoryCreationParams	( CreationParams )
   , m_v2ResetPosition			( ResetPosition	 )
{
  
}

void CBlock::DisableCollisions()
{
	GetPhysicsBody()->SetActive( false );
}

void CBlock::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
}

void CBlock::VOnResourceAcquire()
{
	VHandleFactoryParams( m_rFactoryCreationParams, GetResetPosition() );
	CGCObjSpritePhysics::VOnResourceAcquire();
	SetResetPosition( m_v2ResetPosition );
}