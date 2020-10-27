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
	, m_eCrumbleState(ECrumbleState::Stage_0)
{
	m_ePlatformType = EPlatformType::Crumbling;
}

void CCrumblingPlatform::VOnResourceAcquire()
{
	CPlatform::VOnResourceAcquire();

	m_pcDirector = cocos2d::Director::getInstance();

	LoadAnimations();	
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
		
		if (m_fCurrentCrumblingTimer <= 0.75f && m_fCurrentCrumblingTimer >= 0.56f)
		{
			UpdateCrumblingPlatform(ECrumbleState::Stage_1);
		}
		else if (m_fCurrentCrumblingTimer <= 0.5f && m_fCurrentCrumblingTimer >= 0.26f)
		{
			UpdateCrumblingPlatform(ECrumbleState::Stage_2);
		}
		else if (m_fCurrentCrumblingTimer <= 0.25f && m_fCurrentCrumblingTimer >= 0.01f)
		{
			UpdateCrumblingPlatform(ECrumbleState::Stage_3);
		}
		else if (m_fCurrentCrumblingTimer <= 0.0f)
		{
			// destroy platform - doesn't destroy it, just disables the physicsbody of the respective crumbling platform
			UpdateCrumblingPlatform(ECrumbleState::Stage_Destroy);

			// stop timer calculations and all these if checks, as these aren't needed anymore
			m_bInitiatedCrumbling = false;
		}
		
	}
}

void CCrumblingPlatform::VOnReset()
{
	CPlatform::VOnReset();
	
	m_fCurrentCrumblingTimer = 0;

	m_bInitiatedCrumbling = false;
	m_fCurrentCrumblingTimer = 1.f;
	m_eCrumbleState = ECrumbleState::Stage_0;

	RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcAnimations[0]));
}

void CCrumblingPlatform::VOnResourceRelease()
{
	CPlatform::VOnResourceRelease();

	// release in reverse order
	int iCounter = 5;
	// release all animations when killed
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
	if (m_bInitiatedCrumbling == false)
	{
		m_bInitiatedCrumbling = true;

		UpdateCrumblingPlatform(m_eCrumbleState);
	}
}

void CCrumblingPlatform::StopCrumbling()
{
	if(m_bInitiatedCrumbling == true)
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

	int iCounter = 0;
	for(const char* pszAnim : m_pszAnimations)
	{
		cocos2d::ValueMap& rdictPlist = GCCocosHelpers::CreateDictionaryFromPlist(m_FactoryCreationParams.strPlistFile);
		m_pcAnimations[iCounter] = GCCocosHelpers::CreateAnimation(rdictPlist, m_pszAnimations[iCounter]);
		m_pcAnimations[iCounter]->retain();
		
		iCounter++;
	}
}

void CCrumblingPlatform::UpdateCrumblingPlatform(ECrumbleState eNewCrumbleState)
{
	// only if the new state is different to the current state
	if (eNewCrumbleState != m_eCrumbleState)
	{
		// for extra functionality - playing sound on each crumble
		switch (eNewCrumbleState)
		{
		case ECrumbleState::Stage_0:
			m_eCrumbleState = ECrumbleState::Stage_0;
			RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcAnimations[1]));
		case ECrumbleState::Stage_1:
			m_eCrumbleState = ECrumbleState::Stage_1;
			RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcAnimations[2]));
			break;
		case ECrumbleState::Stage_2:
			m_eCrumbleState = ECrumbleState::Stage_2;
			RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcAnimations[3]));
			break;
		case ECrumbleState::Stage_3:
			m_eCrumbleState = ECrumbleState::Stage_3;
			RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcAnimations[4]));
			break;
		case ECrumbleState::Stage_Destroy:
			m_eCrumbleState = ECrumbleState::Stage_Destroy;
			RunAction(GCCocosHelpers::CreateAnimationActionOnce(m_pcAnimations[5]));
			// Disable Physics body to allow the player to fall through the platform
			GetPhysicsBody()->SetActive(false);
			break;
		}
	}
}


