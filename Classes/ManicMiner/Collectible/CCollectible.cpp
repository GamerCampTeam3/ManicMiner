
#ifndef _CCOLLECTIBLE_H_
#include "ManicMiner/Collectible/CCollectible.h"
#include "ManicMiner/CollectiblesGroup/CCollectiblesGroup.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
IN_CPP_CREATION_PARAMS_DECLARE( CGCObjItem, "TexturePacker/Sprites/Coin/Coin.plist", "coin", b2_dynamicBody, true );
//virtual 
void CCollectible::VOnResourceAcquire( void )
{
	IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjItem );
	CGCObjSpritePhysics::VOnResourceAcquire();
}

CCollectible::CCollectible(ECollectibleType eType, CCollectiblesGroup &collectiblesGroup)
	: CGCObjSpritePhysics( GetGCTypeIDOf( CCollectible ) )
	, m_eCollectibleType	( eType )
	, m_bHasBeenCollected	( false )
	, m_pcCollectiblesGroup	( &collectiblesGroup )
{
}

void CCollectible::InteractEvent()
{
	int collectibleIncrement;
	
	if (!m_bHasBeenCollected)
	{
		switch (m_eCollectibleType)
		{
			case ECollectibleType::Collectible:
				collectibleIncrement = m_pcCollectiblesGroup->GetCollectible();
				m_pcCollectiblesGroup->SetCollectibles( collectibleIncrement );
				CGCObjectManager::ObjectKill( this );
				m_bHasBeenCollected = true;
				break;

			case ECollectibleType::Switch:
				collectibleIncrement = m_pcCollectiblesGroup->GetSwitches();
				m_pcCollectiblesGroup->SetSwitches( collectibleIncrement );
				SetFlippedX( true );
				m_bHasBeenCollected = true;
				break;
		}
	}
}



