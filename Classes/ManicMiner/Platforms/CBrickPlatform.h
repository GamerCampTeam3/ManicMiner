////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CBRICKPLATFORM_H_
#define _CBRICKPLATFORM_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

#include "CPlatform.h"

class CBrickPlatform : public CPlatform
{
public:
	CBrickPlatform();
	//CBrickPlatform( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition );
	GCFACTORY_DECLARE_CREATABLECLASS(CBrickPlatform);
	
	void VOnResourceAcquire() override;

	void VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;

	void VOnReset() override;

};

#endif
