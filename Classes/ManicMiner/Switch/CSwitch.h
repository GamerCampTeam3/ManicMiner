//////////////////
/// Bib
//////////////////

#ifndef _CSWITCH_H_
#define _CSWITCH_H_

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"


class CSwitch
	: public CGCObjSpritePhysics
{
private:
	CGCFactoryCreationParams&	m_FactoryCreationParams;		// Reference to creation params that will be set via the collectible group
	bool						m_bHasBeenCollected;			// Used to stop multi function calls in one frame 
	cocos2d::Vec2				m_v2ResetPosition;				// Used to set the initial position of the collectible


	// Overrides from CGCObjSpritePhysics
	virtual void VOnResourceAcquire( void ) override;
	virtual void VOnReset() override;

public:
	// Constructor, takes in the following params:
	// @param							CreationParams		Sets the sprite and animation
	// @param							ResetPosition		The original/reset position of this sprite
	// @param							CollectibleGroup	Reference to the collectibles group
	CSwitch( CGCFactoryCreationParams&	CreationParams, 
			 cocos2d::Vec2				ResetPosition   );


	// The interact event called in collision
	// Has a switch depending on type on creation.
	void InteractEvent();

};
#endif // #ifndef _CSWITCH_H_