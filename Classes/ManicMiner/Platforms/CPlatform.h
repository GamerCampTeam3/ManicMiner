////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CPLATFORM_H_
#define _CPLATFORM_H_

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"


enum class EPlatformType
{
	Brick,
	Crumbling,
	Ground,
	Moving,
	Regular
};

class CPlatform :
    public CGCObjSpritePhysics
{
public:
	CPlatform( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition );

	void VOnResourceAcquire() override;

	void VOnReset() override;
	
	inline EPlatformType GetPlatformType() { return m_ePlatformType; }

	bool GetCollisionEnabled();
	void SetCollisionEnabled( bool bCollisionEnabled );
	
	bool GetTriggersHardContactEvent();
	void SetTriggersHardContactEvent( bool bShouldTrigger ); 

	
protected:

	CGCFactoryCreationParams& m_FactoryCreationParams;

	// V2 position in the level
	cocos2d::Vec2 m_v2ResetPosition;

	EPlatformType m_ePlatformType;

	bool m_bCollisionEnabled;
	bool m_bTriggersHardContactEvent;


	cocos2d::Director* m_pcDirector;
};

#endif

