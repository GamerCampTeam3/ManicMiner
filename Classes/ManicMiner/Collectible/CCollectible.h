//////////////////
/// Bib
//////////////////

#ifndef _CCOLLECTIBLE_H_
#define _CCOLLECTIBLE_H_

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"

class CGameManager;
// Forward class decleration
class CCollectiblesGroup;

class CCollectible
: public CGCObjSpritePhysics
{
private:
	CGCFactoryCreationParams&	m_FactoryCreationParams;		// Reference to creation params that will be set via the collectible group
	bool						m_bHasBeenCollected;			// Used to stop multi function calls in one frame 
	cocos2d::Vec2				m_v2ResetPosition;				// Used to set the initial position of the collectible
	//CCollectiblesGroup*			m_pcCollectiblesGroup;			// Pointer to a group to call on it score and collectible tracking functions
	CGameManager*				m_pcGameManager;


public:
	// Constructor, takes in the following params:
	// 
	// Creation params: To set it's sprite/animation
	// Collectible Type: Determines if it is either a switch or a collectible
	// Vector 2: Sets its initial and reset position
	// CollectibleGroup: Reference to it to allow it to call functions on it
	CCollectible( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition, CGameManager& GameManager );

	// Overrides from CGCObjSpritePhysics
	virtual void VOnResourceAcquire( void ) override;
	virtual void VOnReset() override;

	// The interact event called in collision
	// Has a switch depending on type on creation.
	void InteractEvent();

};
#endif // #ifndef _CCOLLECTIBLE_H_