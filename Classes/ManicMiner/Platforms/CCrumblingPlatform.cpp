////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CCRUMBLINGPLATFORM_H_
#include "CCrumblingPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "ManicMiner/AudioHelper/ManicAudio.h"

#ifndef TINYXML2_INCLUDED
#include "external\tinyxml2\tinyxml2.h"
#endif




#ifndef _GCLEVELLOADER_OGMO_H_
#include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

GCFACTORY_IMPLEMENT_CREATEABLECLASS(CCrumblingPlatform);


CCrumblingPlatform::CCrumblingPlatform()
	: CPlatform()
{
	m_ePlatformType = EPlatformType::Crumbling;
}

void CCrumblingPlatform::VOnResourceAcquire()
{
	CPlatform::VOnResourceAcquire();

	m_pcDirector = cocos2d::Director::getInstance();

	LoadAnimations();
	//PreloadSoundEffect( ESoundEffectName::Crumbling_Stage);
}

void CCrumblingPlatform::VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams,
	cocos2d::Vec2 v2InitialPosition)
{
	const CGCFactoryCreationParams* pParamsToPasstoBaseClass = &rCreationParams;

	//Fetch a pointer into the OGMO Xml edtior element containing the data.
	const tinyxml2::XMLElement* pCurrentObjectXmlData = CGCLevelLoader_Ogmo::GetCurrentObjectXmlData();

	if (nullptr != pCurrentObjectXmlData)
	{
		const tinyxml2::XMLAttribute* pCustomPlistPath = pCurrentObjectXmlData->FindAttribute("CustomPlist");

		if ((nullptr != pCustomPlistPath)
			&& (0 != strlen(pCustomPlistPath->Value())))
		{
			m_pcCustomCreationParams = std::make_unique< CGCFactoryCreationParams >(rCreationParams.strClassName.c_str(),
				pCustomPlistPath->Value(),
				rCreationParams.strPhysicsShape.c_str(),
				rCreationParams.eB2dBody_BodyType,
				rCreationParams.bB2dBody_FixedRotation);

			pParamsToPasstoBaseClass = m_pcCustomCreationParams.get();
		}
	}

	CPlatform::VHandleFactoryParams(*pParamsToPasstoBaseClass, v2InitialPosition);
}

void CCrumblingPlatform::VOnUpdate(float fTimeStep)
{
	CPlatform::VOnUpdate(fTimeStep);

	if (m_bInitiatedCrumbling)
	{
		// the amount to reduce the crumblingtimer by on every frame
		m_fReduceCrumblingTimerBy = (1.f / m_pcDirector->getFrameRate());

		// 
		m_fCurrentCrumblingTimer -= m_fReduceCrumblingTimerBy;

		switch(m_eCrumbleState)
		{
		case ECrumbleState::Stage_0 :
			if(IsInRange(m_fCurrentCrumblingTimer, 0.2255f, 0.3375f))
			{
				UpdateCrumblingPlatform(ECrumbleState::Stage_1);
			}
			break;
		case ECrumbleState::Stage_1 :
			if (IsInRange(m_fCurrentCrumblingTimer, 0.113f, 0.225f))
			{
				UpdateCrumblingPlatform(ECrumbleState::Stage_2);
			}
			break;
		case ECrumbleState::Stage_2 :
			if (IsInRange(m_fCurrentCrumblingTimer, 0.01f, 0.1125f))
			{
				UpdateCrumblingPlatform(ECrumbleState::Stage_3);
			}
			break;
		case ECrumbleState::Stage_3 :
			if (m_fCurrentCrumblingTimer <= 0.0f)
			{
				// destroy platform - doesn't destroy it, just disables the physicsbody of the respective crumbling platform
				UpdateCrumblingPlatform(ECrumbleState::Stage_Destroy);

				// stop timer calculations and all these if checks, as these aren't needed anymore
				m_bInitiatedCrumbling = false;
			}
			break;
		}		
	}
}

void CCrumblingPlatform::VOnReset()
{
	CPlatform::VOnReset();
	
	m_fCurrentCrumblingTimer = 0;

	m_bInitiatedCrumbling = false;
	m_fCurrentCrumblingTimer = 0.45f;
	m_eCrumbleState = ECrumbleState::Stage_0;

	RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcAnimations[0]));
}

void CCrumblingPlatform::VOnResourceRelease()
{
	CPlatform::VOnResourceRelease();

	// release in reverse order
	int iCounter = 5;
	// release all animations from memory and set to nullptr
	for (auto* Animations : m_pcAnimations)
	{
		m_pcAnimations[iCounter]->release();

		if(m_pcAnimations[iCounter] != nullptr)
		{
			m_pcAnimations[iCounter] = nullptr;
		}
		iCounter--;
	}
}


void CCrumblingPlatform::InitiateCrumbling()
{
	if (false == m_bInitiatedCrumbling)
	{
		// Activates Timer in VOnUpdate that plays the correct crumble animation,
		// based on the time remaining and the current state (m_eCrumbleState)
		m_bInitiatedCrumbling = true;

		// Initial Crumble
		UpdateCrumblingPlatform(m_eCrumbleState);
	}
}

void CCrumblingPlatform::StopCrumbling()
{
	if(true == m_bInitiatedCrumbling)
	{
		m_bInitiatedCrumbling = false;
	}
}

void CCrumblingPlatform::LoadAnimations()
{
	// no need for idle state, because stage 00 is actually the same animation as idle
	m_pszAnimations[0] = "Crumble_Stage_Idle";
	m_pszAnimations[1] = "Crumble_Stage_00";
	m_pszAnimations[2] = "Crumble_Stage_01";
	m_pszAnimations[3] = "Crumble_Stage_02";
	m_pszAnimations[4] = "Crumble_Stage_03";
	m_pszAnimations[5] = "Crumble_Stage_Destroy";

	// loads in animations based on animations specified in the m_pszAnimations Array
	// and calls retain on each of them so as the reference count is 0 by default
	int iCounter = 0;
	for(const char* pszAnim : m_pszAnimations)
	{
		cocos2d::ValueMap& rdictPlist = GCCocosHelpers::CreateDictionaryFromPlist(GetFactoryCreationParams()->strPlistFile);
		m_pcAnimations[iCounter] = GCCocosHelpers::CreateAnimation(rdictPlist, m_pszAnimations[iCounter]);
		if (nullptr != m_pcAnimations[iCounter])
		{
			m_pcAnimations[iCounter]->retain(); // check for nullptr
		}
		iCounter++;
	}
}

void CCrumblingPlatform::UpdateCrumblingPlatform(ECrumbleState eNewCrumbleState)
{
	// only if the new state is different to the current state
	if (eNewCrumbleState != m_eCrumbleState)
	{
		GetSprite()->stopAllActions();
		// for extra functionality - playing sound on each crumble
		switch (eNewCrumbleState)
		{
		case ECrumbleState::Stage_0:
			m_eCrumbleState = ECrumbleState::Stage_0;
			RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcAnimations[1]));
			//PlaySoundEffect(ESoundEffectName::Crumbling_Stage);
		case ECrumbleState::Stage_1:
			m_eCrumbleState = ECrumbleState::Stage_1;
			RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcAnimations[2]));
			//PlaySoundEffect(ESoundEffectName::Crumbling_Stage);
			break;
		case ECrumbleState::Stage_2:
			m_eCrumbleState = ECrumbleState::Stage_2;
			RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcAnimations[3]));
			//PlaySoundEffect( ESoundEffectName::Crumbling_Stage);
			break;
		case ECrumbleState::Stage_3:
			m_eCrumbleState = ECrumbleState::Stage_3;
			RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcAnimations[4]));
			//PlaySoundEffect( ESoundEffectName::Crumbling_Stage);
			break;
		case ECrumbleState::Stage_Destroy:
			m_eCrumbleState = ECrumbleState::Stage_Destroy;
			RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcAnimations[5]));
			//PlaySoundEffect( ESoundEffectName::Crumbling_Stage);
			// Disable Physics body to allow the player to fall through the platform
			SetCollisionEnabled( false );
			SetTriggersHardContactEvent( true );
			GetPhysicsBody()->SetActive( false );
			break;
		}
	}
}

bool CCrumblingPlatform::IsInRange(float NumToCheck, float MinRange, float MaxRange)
{
	bool bIsInRange = false;
	if(NumToCheck <= MaxRange && NumToCheck >= MinRange)
	{
		bIsInRange = true;
	}
	return bIsInRange;
}


