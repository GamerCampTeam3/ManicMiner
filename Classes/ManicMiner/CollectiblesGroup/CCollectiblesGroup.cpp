


////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
// using
using namespace cocos2d;


// IN_CPP_CREATION_PARAMS_DECLARE( CGCObjItem, "TexturePacker/Sprites/Coin/Coin.plist", "coin", b2_staticBody, true );

// static CGCFactoryCreationParams s_cCreationParamsKey( "coin", "TexturePacker/Sprites/Coin/Coin.plist", "coin", b2_staticBody, true );


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CCollectiblesGroup::CCollectiblesGroup()
	: m_iCollectibles			( 0 )
	, m_iSwitches				( 0 )
	, m_iMaxCollectibles		( 0 )
	, m_iMaxSwitches			( 0 )
	, m_eCollectibleTypeRequired( ECollectibleTypeRequired::Collectible )
	, m_pcManicLayer			( nullptr )
	, m_iScore					( 0 )
{
}



CCollectiblesGroup::CCollectiblesGroup( CManicLayer& cLayer )
	: m_iCollectibles			( 0 )
	, m_iSwitches				( 0 )
	, m_iMaxCollectibles		( 0 )
	, m_iMaxSwitches			( 0 )
	, m_eCollectibleTypeRequired( ECollectibleTypeRequired::Collectible )
	, m_pcManicLayer			( &cLayer )
	, m_iScore					( 0 )
{
}

CCollectiblesGroup::CCollectiblesGroup( CManicLayer& cLayer, ECollectibleTypeRequired typeCollectibles )
	: m_iCollectibles			( 0 )
	, m_iSwitches				( 0 )
	, m_iMaxCollectibles		( 0 )
	, m_iMaxSwitches			( 0 )
	, m_eCollectibleTypeRequired( typeCollectibles )
	, m_pcManicLayer			( &cLayer )
	, m_iScore					( 0 )
{

}

CCollectiblesGroup::CCollectiblesGroup( CManicLayer& cLayer, ECollectibleTypeRequired typeCollectibles, int numCollectibles )
	: m_iCollectibles			( 0 )
	, m_iSwitches				( 0 )
	, m_iMaxCollectibles		( numCollectibles )
	, m_iMaxSwitches			( 0 )
	, m_eCollectibleTypeRequired( typeCollectibles )
	, m_pcManicLayer			( &cLayer )
	, m_iScore					( 0 )
{
}

void CCollectiblesGroup::SetLayer(CManicLayer& cLayer)
{
	m_pcManicLayer = &cLayer;
}


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



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// virtual
CCollectiblesGroup::~CCollectiblesGroup()
{}



void CCollectiblesGroup::CollectibleEvent()
{
	m_iCollectibles++;
	CheckIfEnoughToOpenExit();
}


void CCollectiblesGroup::SwitchEvent()
{
	m_iSwitches++;
	CheckIfEnoughToOpenExit();
	
}




//////////////////////////////////////////////////////////////////////////
// only handle items
//////////////////////////////////////////////////////////////////////////
//virtual 
bool CCollectiblesGroup::VHandlesThisTypeId( GCTypeID idQueryType )
{
	return(GetGCTypeIDOf( CCollectible ) == idQueryType);
}



//////////////////////////////////////////////////////////////////////////
// must return the typeid of the CGCObjectGroup derived class
//////////////////////////////////////////////////////////////////////////
//virtual 
GCTypeID CCollectiblesGroup::VGetTypeId( void )
{
	return GetGCTypeIDOf( CCollectiblesGroup );
}

void CCollectiblesGroup::VOnGroupResourceAcquire()
{
	CGCObjectGroup::VOnGroupResourceAcquire();
	GenerateCollectibles();
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CCollectiblesGroup::VOnGroupResourceAcquire_PostObject( void )
{

	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();
	//// parent class version
	//CGCObjectGroup::VOnGroupResourceAcquire_PostObject();
	//
	//// set up animations for all items
	//const char* pszPlist_Coin = "TexturePacker/Sprites/Coin/Coin.plist";
	//const char* pszAnim_Coin_Rotate = "Rotate";
	//// To add different animations here
	//
	//// make an animation
	//// N.B. pdictPList is returned autoreleased - will clean itself at end of frame if not retained
	//ValueMap& rdicPList = GCCocosHelpers::CreateDictionaryFromPlist( pszPlist_Coin );
	//Animation* pAnimation = GCCocosHelpers::CreateAnimation( rdicPList, pszAnim_Coin_Rotate );
	//
	//ForEachObject( [&]( CGCObject* pcItemAsObject ) -> bool
	//{
	//	// this check is essentially redundant, but they say assumption is the mother of all something or others...
	//	CCAssert( (GetGCTypeIDOf( CCollectible ) == pcItemAsObject->GetGCTypeID()),
	//		"CGCObject derived type mismatch!" );
	//
	//	CGCObjSprite* pItemSprite = (CGCObjSprite*)pcItemAsObject;
	//	pItemSprite->RunAction( GCCocosHelpers::CreateAnimationActionLoop( pAnimation ) );
	//	return true;
	//} );
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CCollectiblesGroup::VOnGroupResourceRelease( void )
{
	// n.b. this must happen first, as will fail if objects destroyed before 
	CGCObjectGroup::VOnGroupResourceRelease();
	DestroyItems();
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
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

