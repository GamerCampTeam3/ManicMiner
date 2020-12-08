////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CPLATFORM_H_
#define _CPLATFORM_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

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
	CPlatform();
	//CPlatform( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition );
	virtual ~CPlatform();

	GCFACTORY_DECLARE_CREATABLECLASS(CPlatform);
	
	void VOnResourceAcquire() override;

	void VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;

	void VOnResourceRelease() override;

	void VOnReset() override;
	
	inline EPlatformType GetPlatformType() const { return m_ePlatformType; }

	bool GetCollisionEnabled() const;
	void SetCollisionEnabled( bool bCollisionEnabled );
	
	bool GetTriggersHardContactEvent() const;
	void SetTriggersHardContactEvent( bool bShouldTrigger ); 

	bool GetIsInContact() const;
	void SetIsInContact( bool bIsInContact );

	bool GetIsSensorOverlapped() const;
	void SetIsSensorOverlapped( bool bIsSensorOverlapped );

	bool GetIsTriggeringHardContact() const;
	void SetIsTriggeringHardContact( bool bIsTriggeringHardContact );


	
protected:
	
	//CGCFactoryCreationParams& m_FactoryCreationParams;
	std::unique_ptr<CGCFactoryCreationParams> m_pcCustomCreationParams;
	
	// V2 position in the level
	cocos2d::Vec2 m_v2ResetPosition;

	EPlatformType m_ePlatformType;

	bool m_bCollisionEnabled;
	bool m_bTriggersHardContactEvent;
	bool m_bIsInContact;
	bool m_bIsSensorOverlapped;
	bool m_bIsTriggeringHardContact;

	cocos2d::Vec2	m_cAnchorPoint;
	cocos2d::Director* m_pcDirector;
};

#endif