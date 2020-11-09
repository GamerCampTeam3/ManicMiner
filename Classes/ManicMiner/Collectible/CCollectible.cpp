#include "ManicMiner/Collectible/CCollectible.h"
#include "ManicMiner/CollectiblesGroup/CCollectiblesGroup.h"

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "ManicMiner/Helpers/Helpers.h"


#include "ManicMiner/AudioHelper/ManicAudio.h"
#include "ManicMiner/GameManager/CGameManager.h"

USING_NS_CC;



CCollectible::CCollectible( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition, CGameManager& GameManager )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CCollectible ) )
	, m_FactoryCreationParams			( CreationParams )
	, m_bHasBeenCollected				( false )
	, m_v2ResetPosition					( ResetPosition)
	, m_pcGameManager					( &GameManager )
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

/// Main Interact Event
/// Depending if this object is a ECollectibleType::Collectible or ECollectibleType::Switch
/// Will have a different interact logic.
/// if ::Collectible, it will increment score and run the collectible event then delete itself
/// if ::Switch, it will flip it's sprite and run the switch event
void CCollectible::InteractEvent()
{
	if (!m_bHasBeenCollected)
	{
		PlaySoundEffect( ESoundName::KeyCollected );

		m_pcGameManager->CCollectibleInteractEvent();
		CGCObjectManager::ObjectKill( this );
		
		m_bHasBeenCollected = true;
	}
}