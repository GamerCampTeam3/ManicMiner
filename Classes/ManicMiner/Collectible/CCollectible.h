#ifndef _CCOLLECTIBLE_H_
#define _CCOLLECTIBLE_H_

#include "ManicMiner/Enums/ECollectibleType.h"
#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"

class CCollectiblesGroup;

class CCollectible
: public CGCObjSpritePhysics
{
private:
	ECollectibleType m_eCollectibleType;
	bool m_bHasBeenCollected;
	CCollectiblesGroup* m_pcCollectiblesGroup;

public:
	
	CCollectible( void )
		: CGCObjSpritePhysics( GetGCTypeIDOf( CCollectible ) )
		, m_bHasBeenCollected( false )
	{}

	CCollectible( ECollectibleType eType, CCollectiblesGroup &collectibleGroup );
	~CCollectible();

	virtual void VOnResourceAcquire( void ) override;
	virtual void VOnReset() override;

	void InteractEvent();

	void Collected() { m_bHasBeenCollected = true; }
	bool GetHasBeenCollected() { return m_bHasBeenCollected; }
};

#endif // #ifndef _CCOLLECTIBLE_H_