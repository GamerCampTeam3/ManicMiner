
#ifndef _CTRIGGERPLATFORMS_H_
#include "CTriggerPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"


CTriggerPlatform::CTriggerPlatform(cocos2d::Vec2 v2ResetPosition)
	: CGCObjSpritePhysics(GetGCTypeIDOf(CTriggerPlatform))
        , m_bHasBeenTriggered		   ( false )
        , m_pcCrumbleAnimation		   ( nullptr )
{
    this->SetResetPosition(v2ResetPosition);
}

CTriggerPlatform::~CTriggerPlatform() {
}

IN_CPP_CREATION_PARAMS_DECLARE(CTriggerPlatform, "TexturePacker/Sprites/Platform/CentralCavern/CentralCavern/Crumbling/KongBeast/KG_CrumblingPlatform.plist", "CN_Platform_Regular_2x1", b2_staticBody, true);
void CTriggerPlatform::VOnResourceAcquire()
{
      IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE(CTriggerPlatform);
      
      CGCObjSpritePhysics::VOnResourceAcquire();

      const char* pszAnim_Crumble = "Crumble";

      // Load Animation from CreationParams and retain it
      cocos2d::ValueMap dicPlist = GCCocosHelpers::CreateDictionaryFromPlist(GetFactoryCreationParams()->strPlistFile);
      m_pcCrumbleAnimation = GCCocosHelpers::CreateAnimation(dicPlist, pszAnim_Crumble);
      if(m_pcCrumbleAnimation) 
      {
          m_pcCrumbleAnimation->retain();
      }
}

void CTriggerPlatform::VOnResourceRelease()
{
    CGCObjSpritePhysics::VOnResourceRelease();

    if (m_bHasBeenTriggered)
    {
        GetSprite()->stopAllActions();
    }
    if(m_pcCrumbleAnimation) 
    {
        m_pcCrumbleAnimation->release();
    }
    m_pcCrumbleAnimation = nullptr;
}

void CTriggerPlatform::VOnReset()
{
    CGCObjSpritePhysics::VOnReset();
    m_bHasBeenTriggered = false;
    if (m_bHasBeenTriggered)
    {
        GetSprite()->stopAllActions();
    }
}

void CTriggerPlatform::TriggerCrumble()
{
	if(nullptr != m_pcCrumbleAnimation) 
	{
		RunAction(GCCocosHelpers::CreateAnimationActionLoop(m_pcCrumbleAnimation));
                m_bHasBeenTriggered = true;
                GetPhysicsBody()->SetActive(false);
	}
}
