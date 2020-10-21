////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CCRUMBLINGPLATFORM_H_
#include "CCrumblingPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

CCrumblingPlatform::CCrumblingPlatform( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition )
	: CPlatform( CreationParams, ResetPosition )
	, m_bInitiatedCrumbling(false)
	, m_fCurrentCrumblingTimer(1.f)
	, m_eCrumbleState(ECS_0)
{
	m_ePlatformType = EPlatformType::EPT_Crumbling;
}

void CCrumblingPlatform::VOnResourceAcquire()
{
	CPlatform::VOnResourceAcquire();

	m_pcDirector = cocos2d::Director::getInstance();
}

void CCrumblingPlatform::VOnUpdate(float fTimeStep)
{
	CPlatform::VOnUpdate(fTimeStep);

	if (m_bInitiatedCrumbling)
	{
		m_fReduceCrumblingTimerBy = 1.f / m_pcDirector->getFrameRate();

		m_fCurrentCrumblingTimer -= m_fReduceCrumblingTimerBy;
		
		if (m_fCurrentCrumblingTimer <= 0.75f && m_fCurrentCrumblingTimer >= 0.56f)
		{
			UpdateCrumblingPlatform(ECrumbleState::ECS_1);
		}
		else if (m_fCurrentCrumblingTimer <= 0.5f && m_fCurrentCrumblingTimer >= 0.26f)
		{
			UpdateCrumblingPlatform(ECrumbleState::ECS_2);
		}
		else if (m_fCurrentCrumblingTimer <= 0.25f && m_fCurrentCrumblingTimer >= 0.01f)
		{
			UpdateCrumblingPlatform(ECrumbleState::ECS_3);
		}
		else if (m_fCurrentCrumblingTimer <= 0.0f)
		{
			// destroy platform 
			UpdateCrumblingPlatform(ECrumbleState::ECS_Destroy);
		}
		
	}
}

void CCrumblingPlatform::VOnReset()
{
	CPlatform::VOnReset();
	
	m_fCurrentCrumblingTimer = 0;

	m_v2ResetPosition = cocos2d::Vec2(0.f, 0.f);
	m_bInitiatedCrumbling = false;
	m_fCurrentCrumblingTimer = 1.f;
	m_eCrumbleState = ECrumbleState::ECS_0;
	m_bCollisionEnabled = false;
	m_bTriggersHardContactEvent = false;

}

void CCrumblingPlatform::InitiateCrumbling(float fSecondsToStartCrumbling)
{
	if (m_bInitiatedCrumbling == false)
	{
		m_fCurrentCrumblingTimer = fSecondsToStartCrumbling;
		m_bInitiatedCrumbling = true;

		const char* pszAnim_Crumble = "Crumble";

		cocos2d::ValueMap& rdictPlist = GCCocosHelpers::CreateDictionaryFromPlist(m_FactoryCreationParams.strPlistFile);
		m_pcCrumbleAnim = GCCocosHelpers::CreateAnimation(rdictPlist, pszAnim_Crumble);
		
		m_pcCrumbleAnim->setDelayPerUnit(0.30f);
		RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcCrumbleAnim));

	}
}

void CCrumblingPlatform::UpdateCrumblingPlatform(ECrumbleState eNewCrumbleState)
{
	if (eNewCrumbleState != m_eCrumbleState)
	{
		// for extra functionality - playing sound on each crumble
		switch (eNewCrumbleState)
		{
		case ECS_1:
			m_eCrumbleState = ECrumbleState::ECS_1;

			break;
		case ECS_2:
			m_eCrumbleState = ECrumbleState::ECS_2;

			break;
		case ECS_3:
			m_eCrumbleState = ECrumbleState::ECS_3;

			break;
		case ECS_Destroy:
			m_eCrumbleState = ECrumbleState::ECS_Destroy;
			GetPhysicsBody()->SetActive(false);
			break;
		}
	}
}


