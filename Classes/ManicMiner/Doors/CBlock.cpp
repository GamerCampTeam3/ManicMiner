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
	SetPhysicsTransform( cocos2d::Vec2( 2000.0f, 2000.0f ), 0.0f );
	
}

void CBlock::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
	SetSpriteOpacity( 200 );	
}

void CBlock::VOnResourceAcquire()
{
	VHandleFactoryParams( m_rFactoryCreationParams, GetResetPosition() );
	CGCObjSpritePhysics::VOnResourceAcquire();
	SetResetPosition( m_v2ResetPosition );
}