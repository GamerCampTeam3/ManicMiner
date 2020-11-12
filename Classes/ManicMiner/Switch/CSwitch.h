//////////////////
/// Bib
//////////////////

#ifndef _CSWITCH_H_
#define _CSWITCH_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
	#include "GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif 


class CSwitch
	: public CGCObjSpritePhysics
{
private:
	std::unique_ptr< CGCFactoryCreationParams > m_pCustomCreationParams;
	bool										m_bHasBeenCollected;			// Used to stop multi function calls in one frame 


	// Overrides from CGCObjSpritePhysics
	virtual void VOnResourceAcquire( void ) override;
	virtual void VOnReset() override;
	virtual void VHandleFactoryParams( const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition ) override;
	
public:
	// Default Constructor
	CSwitch();
	GCFACTORY_DECLARE_CREATABLECLASS( CDoor );

	// The interact event called in collision
	// Has a switch depending on type on creation.
	void InteractEvent();

};
#endif // #ifndef _CSWITCH_H_