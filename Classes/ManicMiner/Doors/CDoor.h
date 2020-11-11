//////////////////
/// Bib
//////////////////
#ifndef _CDOOR_H_
#define _CDOOR_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif 

// Forward class declaration
class CManicLayer;

class CDoor
	: public CGCObjSpritePhysics
{
private:
	cocos2d::Vec2				m_v2ResetPosition;				// The position of the door inside the level
	
public:
	CDoor();
	CGCFactoryCreationParams* m_pCustomCreationParams;
	
	GCFACTORY_DECLARE_CREATABLECLASS( CDoor );

	// CDoor( CManicLayer& cLayer, CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition);

	// Overriden functions from ObjSpritePhysics
	virtual void VOnResourceAcquire( void ) override;
	virtual void VOnReset()					override;
	virtual void VHandleFactoryParams( const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition ) override;
};
#endif // #ifndef _CDOOR_H_