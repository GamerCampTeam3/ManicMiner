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

	m_bInitiatedCrumbling = false;
	m_fCurrentCrumblingTimer = 1.f;
	m_eCrumbleState = ECrumbleState::ECS_0;

	const char* pszAnim_Idle = "Crumble_Stage_Idle";

	cocos2d::ValueMap& rdictPlist = GCCocosHelpers::CreateDictionaryFromPlist(m_FactoryCreationParams.strPlistFile);
	m_pcIdleAnim = GCCocosHelpers::CreateAnimation(rdictPlist, pszAnim_Idle);
	RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcIdleAnim));
}

void CCrumblingPlatform::InitiateCrumbling()
{
	if (m_bInitiatedCrumbling == false)
	{
		m_bInitiatedCrumbling = true;

		const char* pszAnim_Crumble = "Crumble_Stage_Idle";
		switch (m_eCrumbleState)
		{
		case ECS_0:
			pszAnim_Crumble = "Crumble_Stage_00";
			break;
		case ECS_1:
			pszAnim_Crumble = "Crumble_Stage_01";
			break;
		case ECS_2:
			pszAnim_Crumble = "Crumble_Stage_02";
			break;
		case ECS_3:
			pszAnim_Crumble = "Crumble_Stage_03";
			break;
		}
		cocos2d::ValueMap& rdictPlist = GCCocosHelpers::CreateDictionaryFromPlist(m_FactoryCreationParams.strPlistFile);
		m_pcCrumbleAnim = GCCocosHelpers::CreateAnimation(rdictPlist, pszAnim_Crumble);
		RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcCrumbleAnim));
	}
}

void CCrumblingPlatform::StopCrumbling()
{
	if(m_bInitiatedCrumbling == true)
	{
		m_bInitiatedCrumbling = false;
	}
}


void CCrumblingPlatform::UpdateCrumblingPlatform(ECrumbleState eNewCrumbleState)
{
	if (eNewCrumbleState != m_eCrumbleState)
	{
		const char* pszAnim_Crumble;
		// for extra functionality - playing sound on each crumble
		switch (eNewCrumbleState)
		{
		case ECS_0:
			m_eCrumbleState = ECrumbleState::ECS_0;
			pszAnim_Crumble = "Crumble_Stage_00";
		case ECS_1:
			m_eCrumbleState = ECrumbleState::ECS_1;
			pszAnim_Crumble = "Crumble_Stage_01";
			break;
		case ECS_2:
			m_eCrumbleState = ECrumbleState::ECS_2;
			pszAnim_Crumble = "Crumble_Stage_02";
			break;
		case ECS_3:
			m_eCrumbleState = ECrumbleState::ECS_3;
			pszAnim_Crumble = "Crumble_Stage_03";
			break;
		case ECS_Destroy:
			m_eCrumbleState = ECrumbleState::ECS_Destroy;
			GetPhysicsBody()->SetActive(false);
			pszAnim_Crumble = "Crumble_Stage_Destroy";
			break;
		}

		cocos2d::ValueMap& rdictPlist = GCCocosHelpers::CreateDictionaryFromPlist(m_FactoryCreationParams.strPlistFile);
		m_pcCrumbleAnim = GCCocosHelpers::CreateAnimation(rdictPlist, pszAnim_Crumble);
		RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcCrumbleAnim));
	}
}


