#ifndef _CTRIGGERPLATFORMS_H_
#define _CTRIGGERPLATFORMS_H_

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"

class CTriggerPlatform
	: public CGCObjSpritePhysics
{
	CTriggerPlatform(cocos2d::Vec2 SpawnLocation);
	
public:

	void VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;
	
	void VOnResourceAcquire() override;

	void VOnResourceRelease() override;
	 
	void VOnReset() override;

	void TriggerCrumble();
};

#endif
