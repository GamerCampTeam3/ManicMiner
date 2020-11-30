#ifndef _CBLOCK_H_
#define _CBLOCK_H_

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"

class CBlock : public CGCObjSpritePhysics
{
public:
	CBlock( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition );

	void DisableCollisions();

private:
	CGCFactoryCreationParams&	m_rFactoryCreationParams;
	cocos2d::Vec2				m_v2ResetPosition;

	virtual void VOnResourceAcquire( void ) override;
	virtual void VOnReset()					override;
	
};

#endif