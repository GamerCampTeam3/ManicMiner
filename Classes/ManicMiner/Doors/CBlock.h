#ifndef _CBLOCK_H_
#define _CBLOCK_H_

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"

class CBlock : public CGCObjSpritePhysics
{
public:
	CBlock( CGCFactoryCreationParams& rcCreationParams, cocos2d::Vec2 v2ResetPosition );

	// Called by the "Door", which disables the collision and plays an animation (if any)
	void DissolveBlock();

private:
	CGCFactoryCreationParams&	m_rFactoryCreationParams;	// Used for the sprite and physics data
	cocos2d::Vec2				m_v2ResetPosition;			// The original position of the sprite
	cocos2d::Animation*			m_panimDissolve;			// The animation to be played

	virtual void VOnResourceAcquire( void ) override;
	virtual void VOnReset()					override;
	
};

#endif