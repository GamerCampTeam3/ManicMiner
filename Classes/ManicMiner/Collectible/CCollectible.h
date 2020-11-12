//////////////////
/// Bib
//////////////////

#ifndef _CCOLLECTIBLE_H_
#define _CCOLLECTIBLE_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif


class CCollectible
: public CGCObjSpritePhysics
{
private:
	std::unique_ptr< CGCFactoryCreationParams > m_pCustomCreationParams;			// Reference to creation params that will be set via the collectible group
	bool										m_bHasBeenCollected;				// Used to stop multi function calls in one frame 

	// Overrides from CGCObjSpritePhysics
	virtual void VOnResourceAcquire( void ) override;
	virtual void VOnReset() override;
	virtual void VHandleFactoryParams( const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition ) override;
public:
	// Constructor, takes in the following params:
	CCollectible();
	GCFACTORY_DECLARE_CREATABLECLASS( CCollectible );

	// The interact event called in collision
	void InteractEvent();

};
#endif // #ifndef _CCOLLECTIBLE_H_