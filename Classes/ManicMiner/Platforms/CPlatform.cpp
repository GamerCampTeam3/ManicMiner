////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CPLATFORM_H
#include "CPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"


CPlatform::CPlatform(CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition, EPlatformType PlatformType)
	: CGCObjSpritePhysics(GetGCTypeIDOf(CPlatform))
	, m_FactoryCreationParams(CreationParams)
	, m_v2ResetPosition(ResetPosition)
	, m_ePlatformType( PlatformType )
	, m_bInitiatedCrumbling(false)
	, m_fCurrentCrumblingTimer(1.f)
	, m_eCrumbleState(ECrumbleState::ECS_0)
	, m_bCollisionEnabled( false )
	, m_bTriggersHardContactEvent ( false )

{
}

void CPlatform::VOnResourceAcquire()
{
	VHandleFactoryParams(m_FactoryCreationParams, GetResetPosition());
	
	CGCObjSpritePhysics::VOnResourceAcquire();

	SetResetPosition(m_v2ResetPosition);


	m_pcDirector = cocos2d::Director::getInstance();

}

void CPlatform::VOnUpdate(float fTimeStep)
{
	if (m_bInitiatedCrumbling)
	{
		switch (m_ePlatformType)
		{
		case EPT_Crumbling:

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
			break;
		}
	}
}

void CPlatform::InitiateCrumbling(float fSecondsToStartCrumbling)
{
	switch (m_ePlatformType)
	{
	case EPT_Crumbling:
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
		
		break;
	}
}

void CPlatform::UpdateCrumblingPlatform(ECrumbleState eNewCrumbleState)
{
	if (eNewCrumbleState != m_eCrumbleState)
	{
		// for extra functionality - playing sound on each crumble
		switch (eNewCrumbleState)
		{
		case ECS_1 :
			m_eCrumbleState = ECrumbleState::ECS_1;
			
			break;
		case ECS_2 :
			m_eCrumbleState = ECrumbleState::ECS_2;
			
			break;
		case ECS_3 :
			m_eCrumbleState = ECrumbleState::ECS_3;
			
			break;
		case ECS_Destroy :
			m_eCrumbleState = ECrumbleState::ECS_Destroy;
			GetPhysicsBody()->SetActive(false);
			break;
		}
	}
}



