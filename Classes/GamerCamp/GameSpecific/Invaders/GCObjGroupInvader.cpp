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
// invaders are created from level file, we just need to init their anims
// this has to be done after the invaders have acquired their resources
// hence the name of this function..
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupInvader::VOnGroupResourceAcquire_PostObject( void )
{
	if( GetCountRegistered() > 0 )
	{
		const CGCObjInvader* pcInvader = static_cast<const CGCObjInvader*>( GetRegisteredObjectAtIndex( 0 ) );
		CCAssert( ( GetGCTypeIDOf( CGCObjInvader ) == pcInvader->GetGCTypeID() ),
				  "CGCObject derived type mismatch!" );

		// set up animations for all items - get the creation params from the first one in the group
		const CGCFactoryCreationParams* psObjCreateParams = pcInvader->GetFactoryCreationParams();

		CCAssert( psObjCreateParams, "0th object in group has no creation params" );

		// make an animation
		ValueMap	cDicPList = GCCocosHelpers::CreateDictionaryFromPlist( psObjCreateParams->strPlistFile );
		Animation*	pAnimation = GCCocosHelpers::CreateAnimation( cDicPList, "Fly" );

		// apply it to all the objects
		ForEachObject
		(
			[&] ( CGCObject* pcItemAsObject )
			{
				CGCObjSprite* pItemSprite = (CGCObjSprite*)pcItemAsObject;
				pItemSprite->RunAction( GCCocosHelpers::CreateAnimationActionLoop( pAnimation ) );
				return true;
			}
		);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupInvader::VOnGroupResourceRelease( void )
{
	// N.B. need to do this first as it clears internal lists
	CGCObjectGroup::VOnGroupResourceRelease();
}

