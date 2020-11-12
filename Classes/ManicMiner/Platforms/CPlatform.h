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
	~CPlatform();
	
	void VOnResourceAcquire() override;

	void VOnResourceRelease() override;

	void VOnReset() override;
	
	inline EPlatformType GetPlatformType() { return m_ePlatformType; }

	bool GetCollisionEnabled() const;
	void SetCollisionEnabled( bool bCollisionEnabled );
	
	bool GetTriggersHardContactEvent() const;
	void SetTriggersHardContactEvent( bool bShouldTrigger ); 

	bool GetIsInContact() const;
	void SetIsInContact( bool bIsInContact );

	bool GetIsSensorOverlapped() const;
	void SetIsSensorOverlapped( bool bIsSensorOverlapped );
	
protected:

	CGCFactoryCreationParams& m_FactoryCreationParams;

	// V2 position in the level
	cocos2d::Vec2 m_v2ResetPosition;

	EPlatformType m_ePlatformType;

	bool m_bCollisionEnabled;
	bool m_bTriggersHardContactEvent;
	bool m_bIsInContact;
	bool m_bIsSensorOverlapped;

	cocos2d::Director* m_pcDirector;
};

#endif