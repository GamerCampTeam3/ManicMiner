#include "ManicMiner/Collectible/CCollectible.h"
#include "ManicMiner/CollectiblesGroup/CCollectiblesGroup.h"

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "ManicMiner/Helpers/Helpers.h"


#include "ManicMiner/AudioHelper/ManicAudio.h"

USING_NS_CC;



CCollectible::CCollectible( CGCFactoryCreationParams& CreationParams, ECollectibleType eType, cocos2d::Vec2 ResetPosition, CCollectiblesGroup& collectibleGroup )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CCollectible ) )
	, m_FactoryCreationParams( CreationParams )
	, m_eCollectibleType( eType )
	, m_bHasBeenCollected( false )
	, m_v2ResetPosition( ResetPosition)
	, m_pcCollectiblesGroup( &collectibleGroup )
{
	
}


void CCollectible::VOnResourceAcquire( void )
{
	VHandleFactoryParams( m_FactoryCreationParams, GetResetPosition() );
	CGCObjSpritePhysics::VOnResourceAcquire();
	SetResetPosition( m_v2ResetPosition );
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
		PlaySoundEffect( ESoundName::ESN_KeyCollected );
		
		switch (m_eCollectibleType)
		{
			case ECollectibleType::Collectible:

				m_pcCollectiblesGroup->CollectibleEvent();
				m_pcCollectiblesGroup->AddScore();
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