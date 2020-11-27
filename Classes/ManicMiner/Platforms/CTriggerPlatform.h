#ifndef _CTRIGGERPLATFORMS_H_
#define _CTRIGGERPLATFORMS_H_

#ifndef _GCOBJSPRITEPHYSICS_H_
#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"
#endif

class CTriggerPlatform
	: public CGCObjSpritePhysics
{
private:

        // Whether Crumbling has been triggered 
	bool m_bHasBeenTriggered;
	  
        // Holds the Crumble Animation
	cocos2d::Animation* m_pcCrumbleAnimation;

public:

	CTriggerPlatform(cocos2d::Vec2 v2ResetPosition);
        ~CTriggerPlatform();
	
	void VOnResourceAcquire() override;

	void VOnResourceRelease() override;
	 
	void VOnReset() override;

        // Plays Crumble Animation and Disables Physics Body
        void TriggerCrumble();

};

#endif
