////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>

#include "GamerCamp/GameSpecific/Invaders/GCObjGroupInvader.h"

#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/Core/GCTypes.h"
#include "GamerCamp/GCObject/GCObject.h"
#include "GamerCamp/GameSpecific/Invaders/GCObjInvader.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"


//////////////////////////////////////////////////////////////////////////
// using
using namespace cocos2d;



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CGCObjGroupInvader::CGCObjGroupInvader()
{
	m_v2FormationOrigin = Vec2::ZERO;
}
										


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjGroupInvader::~CGCObjGroupInvader()
{}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupInvader::SetFormationOrigin( Vec2 v2FormationOrigin )
{
	m_v2FormationOrigin = v2FormationOrigin;
}

//////////////////////////////////////////////////////////////////////////
// only handle invaders
//////////////////////////////////////////////////////////////////////////
//virtual 
bool CGCObjGroupInvader::VHandlesThisTypeId( GCTypeID idQueryType )
{
	return( GetGCTypeIDOf( CGCObjInvader ) == idQueryType );
}



//////////////////////////////////////////////////////////////////////////
// must return the typeid of the CGCObjectGroup derived class
//////////////////////////////////////////////////////////////////////////
//virtual 
GCTypeID CGCObjGroupInvader::VGetTypeId()
{
	return GetGCTypeIDOf( CGCObjGroupInvader );
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupInvader::VOnGroupResourceAcquire()
{
	CreateInvaders();
	CGCObjectGroup::VOnGroupResourceAcquire();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupInvader::VOnGroupResourceAcquire_PostObject()
{
	// we do this here because the alternative is to do it for each invader as they're created and it's

	// parent class version
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();

	// set up animations for all items
	const char* pszPlist	= "TexturePacker/Sprites/KoopaTrooper/KoopaTrooper.plist";
	const char* pszAnim_Fly = "Fly";

	// make an animation
	ValueMap&	rdictPList = GCCocosHelpers::CreateDictionaryFromPlist( pszPlist );
	Animation*	pAnimation = GCCocosHelpers::CreateAnimation( rdictPList, pszAnim_Fly );

	ForEachObject( [&] ( CGCObject* pObject ) -> bool
	{
		CCAssert( ( GetGCTypeIDOf( CGCObjInvader ) == pObject->GetGCTypeID() ),
			"CGCObject derived type mismatch!" );

		CGCObjSprite* pItemSprite = (CGCObjSprite*)pObject;
		pItemSprite->RunAction( GCCocosHelpers::CreateAnimationActionLoop( pAnimation ) );
		return true;
	} );
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupInvader::VOnGroupResourceRelease()
{
	// N.B. need to do this first as it clears internal lists
	CGCObjectGroup::VOnGroupResourceRelease();
	DestroyInvaders();
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupInvader::CreateInvaders()
{
	i32	iOffsetX = 40;
	i32	iOffsetY = 100;
	for( u32 uLoop = 0; uLoop < k_uNumProjectiles; ++uLoop )
	{	
		// n.b. these register themselves with this class on creation
		CGCObjInvader* pInvader = new CGCObjInvader(); 
		pInvader->SetResetPosition( Vec2( m_v2FormationOrigin.x + ( iOffsetX * uLoop ), ( m_v2FormationOrigin.y + iOffsetY ) ) );
		pInvader->SetName( "Derek" );
	}
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupInvader::DestroyInvaders()
{
	// this iterates the array of registered CGCObjects 
	// calling the supplied functor then deleting them
	DestroyObjectsReverseOrder( [&]( CGCObject* pObject )
	{
		// do nothing - DestroyObjectsReverseOrder calls delete!
		GCASSERT( GetGCTypeIDOf( CGCObjInvader ) == pObject->GetGCTypeID(), "wrong type!" );
	});
}
