#include <string.h>

#include "GamerCamp/GameSpecific/Items/GCObjGroupItem.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/Core/GCTypes.h"
#include "GamerCamp/GCObject/GCObject.h"
#include "ManicMiner/Collectible/CCollectible.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"


#include "ManicMiner/CollectiblesGroup/CCollectiblesGroup.h"
#include "ManicMiner/Layers/CManicLayer.h"


using namespace cocos2d;



CCollectiblesGroup::CCollectiblesGroup( CManicLayer& cLayer, ECollectibleTypeRequired typeCollectibles, int numCollectibles )
	: m_iCollectibles			( 0 )
	, m_iSwitches				( 0 )
	, m_iMaxCollectibles		( numCollectibles )
	, m_iMaxSwitches			( 0 )
	, m_iScore					( 0 )
	, m_eCollectibleTypeRequired( typeCollectibles )
	, m_pcManicLayer			( &cLayer )
{
}

// This will be overriden.
void CCollectiblesGroup::GenerateCollectibles()
{	
}


void CCollectiblesGroup::CheckIfEnoughToOpenExit()
{
	switch (m_eCollectibleTypeRequired)
	{
		case ECollectibleTypeRequired::Collectible:
			if (m_iCollectibles == m_iMaxCollectibles)
			{
				m_pcManicLayer->SetGameState( EGameState::Escaping );
			}
			break;
		case ECollectibleTypeRequired::Switch:
			if (m_iSwitches == m_iMaxSwitches)
			{
				m_pcManicLayer->SetGameState( EGameState::Escaping );
			}
			break;

		case ECollectibleTypeRequired::Both:
			if ((m_iCollectibles == m_iMaxCollectibles) && (m_iSwitches == m_iMaxSwitches))
			{
				m_pcManicLayer->SetGameState( EGameState::Escaping );
			}
			break;
	}
}




// Increments the collectible count then checks if enough have been gathered
void CCollectiblesGroup::CollectibleEvent()
{
	m_iCollectibles++;
	CheckIfEnoughToOpenExit();
}

// Increments the switches count then checks if enough have been gathered
void CCollectiblesGroup::SwitchEvent()
{
	m_iSwitches++;
	CheckIfEnoughToOpenExit();
	
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// virtual
bool CCollectiblesGroup::VHandlesThisTypeId( GCTypeID idQueryType )
{
	return(GetGCTypeIDOf( CCollectible ) == idQueryType);
}

GCTypeID CCollectiblesGroup::VGetTypeId( void )
{
	return GetGCTypeIDOf( CCollectiblesGroup );
}

void CCollectiblesGroup::VOnGroupResourceAcquire()
{
	CGCObjectGroup::VOnGroupResourceAcquire();
	GenerateCollectibles();
}

void CCollectiblesGroup::VOnGroupResourceAcquire_PostObject( void )
{
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();
}

void CCollectiblesGroup::VOnGroupResourceRelease( void )
{
	// n.b. this must happen first, as will fail if objects destroyed before 
	CGCObjectGroup::VOnGroupResourceRelease();
	DestroyItems();
}

void CCollectiblesGroup::DestroyItems( void )
{
	// this iterates the array of registered CGCObjects 
	// calling the supplied functor then deleting them
	DestroyObjectsReverseOrder( [&]( CGCObject* pObject )
	{
		// do nothing - DestroyObjectsReverseOrder calls delete!
		GCASSERT( GetGCTypeIDOf( CCollectible ) == pObject->GetGCTypeID(), "wrong type!" );
	} );
}

CCollectiblesGroup::~CCollectiblesGroup()
{}
/////////////////////////////////////////////////////////////////////////