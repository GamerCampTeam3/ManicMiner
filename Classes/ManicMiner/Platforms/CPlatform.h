////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CPLATFORM_H_
#define _CPLATFORM_H_

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"

//enum class EPlatformType
//{
//	Brick,
//	Crumbling,
//	Ground,
//	Moving,
//	Regular
//};
//
//EPlatformType m_ePlatform;
//m_ePlatform = EPlatformType::Brick;


enum EPlatformType
{
	EPT_Brick,
	EPT_Crumbling,
	EPT_Ground,
	EPT_Moving,
	EPT_Regular
};

enum EPlatformSize
{
	EPS_1x1,
	EPS_2x1,
	EPS_3x1,
	EPS_4x1,
	EPS_8x1,
	EPS_13x1,
	EPS_15x1,
	EPS_Default
};

enum ECrumbleState
{
	ECS_0,
	ECS_1,
	ECS_2,
	ECS_3,
	ECS_Destroy
};

class CPlatform :
    public CGCObjSpritePhysics
{
public:
	CPlatform(CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition, EPlatformType PlatformType);

	void VOnResourceAcquire() override;

	EPlatformType GetPlatformType() { return m_ePlatformType; }

	bool GetCollisionEnabled() { return m_bCollisionEnabled; }
	void SetCollisionEnabled( bool bCollisionEnabled ) { m_bCollisionEnabled = bCollisionEnabled; }	
	
	bool GetTriggersHardContactEvent() { return m_bTriggersHardContactEvent; }
	void SetTriggersHardContactEvent( bool bShouldTrigger ) { m_bTriggersHardContactEvent = bShouldTrigger; }



	////////////////////////////////////////////////////////////////////////
	/// Crumbling Platforms

	// needs refactoring

	ECrumbleState m_eCrumbleState;
	
	void VOnUpdate(float fTimeStep) override;

	bool m_bInitiatedCrumbling;

	float m_fCurrentCrumblingTimer;
	float m_fReduceCrumblingTimerBy;

	class cocos2d::Director* m_pcDirector;

	cocos2d::Animation* m_pcCrumbleAnim;
	
	// Initiates Crumbling Timer and Animation
	void InitiateCrumbling(float fSecondsToStartCrumbling);

	/// Crumbling Platforms
	////////////////////////////////////////////////////////////////////////
	
private:

	CGCFactoryCreationParams& m_FactoryCreationParams;

	cocos2d::Vec2 m_v2ResetPosition;

	EPlatformType m_ePlatformType;


	void UpdateCrumblingPlatform(ECrumbleState eNewCrumbleState);

	bool m_bCollisionEnabled;
	bool m_bTriggersHardContactEvent;

};

#endif

