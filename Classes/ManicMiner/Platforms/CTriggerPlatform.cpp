
#ifndef _CTRIGGERPLATFORMS_H_
#include "CTriggerPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

#ifndef TINYXML2_INCLUDED
#include "external\tinyxml2\tinyxml2.h"
#endif


#ifndef _GCLEVELLOADER_OGMO_H_
#include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

GCFACTORY_IMPLEMENT_CREATEABLECLASS(CTriggerPlatform);
CTriggerPlatform::CTriggerPlatform()
	: CPlatform()
        , m_bHasBeenTriggered		   ( false )
        , m_pcCrumbleAnimation		   ( nullptr )
{
    
}

CTriggerPlatform::~CTriggerPlatform() {
}

void CTriggerPlatform::VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams,
	cocos2d::Vec2 v2InitialPosition)
{
    const CGCFactoryCreationParams* pParamsToPassToBaseClass = &rCreationParams;

    //Fetch a pointer into the OGMO Xml edtior element containing the data.
    const tinyxml2::XMLElement* pCurrentObjectXmlData = CGCLevelLoader_Ogmo::GetCurrentObjectXmlData();

    if (nullptr != pCurrentObjectXmlData)
    {
        // Read in the custom plist and shape
        const tinyxml2::XMLAttribute* pCustomPlistPath = pCurrentObjectXmlData->FindAttribute("CustomPlist");
        const tinyxml2::XMLAttribute* pCustomShapePath = pCurrentObjectXmlData->FindAttribute("CustomShape");

        if ((nullptr != pCustomPlistPath)
            && (0 != strlen(pCustomPlistPath->Value())))
        {
            m_pcCustomCreationParams = std::make_unique< CGCFactoryCreationParams >(rCreationParams.strClassName.c_str(),
                pCustomPlistPath->Value(),
                pCustomShapePath->Value(),
                rCreationParams.eB2dBody_BodyType,
                rCreationParams.bB2dBody_FixedRotation);

            pParamsToPassToBaseClass = m_pcCustomCreationParams.get();
        }
    }

    CPlatform::VHandleFactoryParams(*pParamsToPassToBaseClass, v2InitialPosition);
}

void CTriggerPlatform::VOnResourceAcquire()
{
      
      CGCObjSpritePhysics::VOnResourceAcquire();

      SetName("TriggerPlatform");
	
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

    if (m_bHasBeenTriggered)
    {
        GetSprite()->stopAllActions();
    }
    if(m_pcCrumbleAnimation) 
    {
        m_pcCrumbleAnimation->release();
    }
    m_pcCrumbleAnimation = nullptr;
    CGCObjSpritePhysics::VOnResourceRelease();
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
		RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcCrumbleAnimation));
                m_bHasBeenTriggered = true;
                GetPhysicsBody()->SetActive(false);
	}
}
