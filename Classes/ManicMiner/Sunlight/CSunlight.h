////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CSUNLIGHT_H_
#define _CSUNLIGHT_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

class CSunlight : public CGCObjSpritePhysics
{
public:

	CSunlight();

	GCFACTORY_DECLARE_CREATABLECLASS(CSunlight);

	void VOnResourceAcquire() override;

	void VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;

	void VOnResourceRelease() override;

	void VOnReset() override;
};

#endif