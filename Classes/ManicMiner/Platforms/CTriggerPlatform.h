#ifndef _CTRIGGERPLATFORMS_H_
#define _CTRIGGERPLATFORMS_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

#include "CPlatform.h"

class CTriggerPlatform
	: public CPlatform
{
private:

        // Whether Crumbling has been triggered 
	bool m_bHasBeenTriggered;
	  
        // Holds the Crumble Animation
	cocos2d::Animation* m_pcCrumbleAnimation;

public:

	GCFACTORY_DECLARE_CREATABLECLASS(CTriggerPlatform);
	CTriggerPlatform();
        ~CTriggerPlatform();

	void VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;
	
	void VOnResourceAcquire() override;

	void VOnResourceRelease() override;
	 
	void VOnReset() override;

        // Plays Crumble Animation and Disables Physics Body
        void TriggerCrumble();

};

#endif
