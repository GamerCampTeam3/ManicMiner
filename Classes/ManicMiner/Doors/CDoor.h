//////////////////
/// Bib
//////////////////
#ifndef _CDOOR_H_
#define _CDOOR_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif 


class CDoor
	: public CGCObjSpritePhysics
{
private:
	
	std::unique_ptr< CGCFactoryCreationParams > m_pCustomCreationParams;
	// Overriden functions from CGCObjSpritePhysics
	virtual void VOnResourceAcquire( void ) override;
	virtual void VOnReset()					override;
	virtual void VHandleFactoryParams( const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition ) override;	
public:
	
	CDoor();	
	GCFACTORY_DECLARE_CREATABLECLASS( CDoor );
	void Open();
};

#endif // #ifndef _CDOOR_H_