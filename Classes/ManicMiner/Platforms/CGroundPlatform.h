////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CGROUNDPLATFORM_H_
#define _CGROUNDPLATFORM_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

#include "CPlatform.h"

class CGroundPlatform : public CPlatform
{
public:
	
	CGroundPlatform();
	//CGroundPlatform( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition );
	GCFACTORY_DECLARE_CREATABLECLASS(CGroundPlatform);
	
	void VOnResourceAcquire() override;

	void VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;

	void VOnReset() override;

};

#endif
