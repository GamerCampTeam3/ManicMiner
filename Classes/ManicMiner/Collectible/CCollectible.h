#ifndef _CCOLLECTIBLE_H_
#define _CCOLLECTIBLE_H_

#include "ManicMiner/Enums/ECollectibleType.h"
#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"

class CCollectiblesGroup;

class CCollectible
: public CGCObjSpritePhysics
{
private:
	CGCFactoryCreationParams& m_FactoryCreationParams;
	ECollectibleType m_eCollectibleType;
	bool m_bHasBeenCollected;
	cocos2d::Vec2 m_v2ResetPosition;
	CCollectiblesGroup* m_pcCollectiblesGroup;


public:
	
	
	CCollectible( CGCFactoryCreationParams& CreationParams, ECollectibleType eType, cocos2d::Vec2 ResetPosition, CCollectiblesGroup& collectibleGroup );
	~CCollectible();

	virtual void VOnResourceAcquire( void ) override;
	virtual void VOnReset() override;

	void InteractEvent();

	void Collected() { m_bHasBeenCollected = true; }
	bool GetHasBeenCollected() { return m_bHasBeenCollected; }
};

#endif // #ifndef _CCOLLECTIBLE_H_