
#ifndef _CTRIGGERPLATFORMS_H_
#include "CTriggerPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

const char* pszAnim_Crumble = "Crumble";

CTriggerPlatform::CTriggerPlatform(cocos2d::Vec2 SpawnLocation)
	: CGCObjSpritePhysics(GetGCTypeIDOf(CTriggerPlatform))
        , m_bHasBeenTriggered		   ( false )
        , m_pcCrumbleAnimation		   ( nullptr )
{
	
}

IN_CPP_CREATION_PARAMS_DECLARE(CTriggerPlatform, "TexturePacker/Sprites/Platform/CentralCavern/CentralCavern/Crumbling/KongBeast/KG_CrumblingPlatform.plist", "CN_Platform_Regular_2x1", b2_staticBody, true);
void CTriggerPlatform::VOnResourceAcquire()
{
      IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE(CTriggerPlatform);

      // Load Animation from CreationParams and retain it
      cocos2d::ValueMap dicPlist = GCCocosHelpers::CreateDictionaryFromPlist(GetFactoryCreationParams()->strPlistFile);
      m_pcCrumbleAnimation = GCCocosHelpers::CreateAnimation(dicPlist, pszAnim_Crumble);
      if(nullptr != m_pcCrumbleAnimation) 
      {
          m_pcCrumbleAnimation->retain();
      }
}

void CTriggerPlatform::VOnResourceRelease()
{
    GetSprite()->stopAllActions();
    
}

void CTriggerPlatform::VOnReset()
{
    GetSprite()->stopAllActions();
    m_bHasBeenTriggered = false;
}

void CTriggerPlatform::TriggerCrumble()
{
	if(nullptr != m_pcCrumbleAnimation) 
	{
		RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcCrumbleAnimation));
                m_bHasBeenTriggered = true;
                GetPhysicsBody()->SetActive(false);
	}
}
