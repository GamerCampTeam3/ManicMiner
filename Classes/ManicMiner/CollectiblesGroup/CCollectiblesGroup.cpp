


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

//////////////////////////////////////////////////////////////////////////
// using
using namespace cocos2d;



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CCollectiblesGroup::CCollectiblesGroup()
	: m_iCollectibles(0)
	, m_iSwitches(0)
	, m_iMaxCollectibles(0)
	, m_iMaxSwitches(0)
	, m_eCollectibleTypeRequired(ECollectibleTypeRequired::Collectible)
{
}

CCollectiblesGroup::CCollectiblesGroup(ECollectibleTypeRequired typeCollectibles )
	: m_iCollectibles( 0 )
	, m_iSwitches( 0 )
	, m_iMaxCollectibles( 0 )
	, m_iMaxSwitches( 0 )
	, m_eCollectibleTypeRequired( typeCollectibles )
{
	
}

CCollectiblesGroup::CCollectiblesGroup( ECollectibleTypeRequired typeCollectibles, int numCollectibles )
	: m_iCollectibles( 0 )
	, m_iSwitches( 0 )
	, m_iMaxCollectibles( numCollectibles )
	, m_iMaxSwitches( 0 )
	, m_eCollectibleTypeRequired( typeCollectibles )
{

}


bool CCollectiblesGroup::CheckIfEnoughToOpenExit()
{
	switch (m_eCollectibleTypeRequired)
	{
		case ECollectibleTypeRequired::Collectible:
			return (m_iCollectibles == m_iMaxCollectibles);

		case ECollectibleTypeRequired::Switch:
			return (m_iSwitches == m_iMaxSwitches);

		case ECollectibleTypeRequired::Both:
			return ((m_iCollectibles == m_iMaxCollectibles) && (m_iSwitches == m_iMaxSwitches));
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
}


void CCollectiblesGroup::SwitchEvent()
{
	m_iSwitches++;
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



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CCollectiblesGroup::VOnGroupResourceAcquire_PostObject( void )
{
	// parent class version
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();

	// set up animations for all items
	const char* pszPlist_Coin = "TexturePacker/Sprites/Coin/Coin.plist";
	const char* pszAnim_Coin_Rotate = "Rotate";
	// To add different animations here
	
	// make an animation
	// N.B. pdictPList is returned autoreleased - will clean itself at end of frame if not retained
	ValueMap& rdicPList = GCCocosHelpers::CreateDictionaryFromPlist( pszPlist_Coin );
	Animation* pAnimation = GCCocosHelpers::CreateAnimation( rdicPList, pszAnim_Coin_Rotate );

	ForEachObject( [&]( CGCObject* pcItemAsObject ) -> bool
	{
		// this check is essentially redundant, but they say assumption is the mother of all something or others...
		CCAssert( (GetGCTypeIDOf( CCollectible ) == pcItemAsObject->GetGCTypeID()),
			"CGCObject derived type mismatch!" );

		CGCObjSprite* pItemSprite = (CGCObjSprite*)pcItemAsObject;
		pItemSprite->RunAction( GCCocosHelpers::CreateAnimationActionLoop( pAnimation ) );
		return true;
	} );
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

