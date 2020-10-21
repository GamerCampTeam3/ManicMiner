
#ifndef _CCOLLECTIBLE_H_
#include "ManicMiner/Collectible/CCollectible.h"
#include "ManicMiner/CollectiblesGroup/CCollectiblesGroup.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "ManicMiner/Helpers/Helpers.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
IN_CPP_CREATION_PARAMS_DECLARE( CGCObjItem, "TexturePacker/Sprites/Coin/Coin.plist", "coin", b2_staticBody, true );
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

void CCollectible::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
	m_bHasBeenCollected = false;
}


void CCollectible::InteractEvent()
{
	if (!m_bHasBeenCollected)
	{
		switch (m_eCollectibleType)
		{
			case ECollectibleType::Collectible:

				m_pcCollectiblesGroup->CollectibleEvent();
				CGCObjectManager::ObjectKill( this );
				m_bHasBeenCollected = true;
				break;

			case ECollectibleType::Switch:

				m_pcCollectiblesGroup->SwitchEvent();
				SetFlippedX( true );
				m_bHasBeenCollected = true;
				break;
		}
	}
}

CCollectible::~CCollectible()
{
	safeDelete( m_pcCollectiblesGroup );
}



