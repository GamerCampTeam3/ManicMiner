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

enum class EPlatformSize
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

	void VOnReset() override;
	
	EPlatformType GetPlatformType() { return m_ePlatformType; }

	bool GetCollisionEnabled() { return m_bCollisionEnabled; }
	void SetCollisionEnabled( bool bCollisionEnabled ) { m_bCollisionEnabled = bCollisionEnabled; }	
	
	bool GetTriggersHardContactEvent() { return m_bTriggersHardContactEvent; }
	void SetTriggersHardContactEvent( bool bShouldTrigger ) { m_bTriggersHardContactEvent = bShouldTrigger; }


	
protected:

	CGCFactoryCreationParams& m_FactoryCreationParams;

	cocos2d::Vec2 m_v2ResetPosition;

	EPlatformType m_ePlatformType;


	bool m_bCollisionEnabled;
	bool m_bTriggersHardContactEvent;


	cocos2d::Director* m_pcDirector;
};

#endif

