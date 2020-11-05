////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/Core/GCTypes.h"
#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"

#include "GamerCamp/GameSpecific/Player/GCObjProjectilePlayer.h"
#include "GamerCamp/GameSpecific/Player/GCObjGroupProjectilePlayer.h"



//////////////////////////////////////////////////////////////////////////
// using
using namespace cocos2d;



//////////////////////////////////////////////////////////////////////////
// N.B. we want reset to put the projectiles on the dead list
//////////////////////////////////////////////////////////////////////////
CGCObjGroupProjectilePlayer::CGCObjGroupProjectilePlayer()
{
	SetResetBehaviour( CGCObjectGroup::EResetDead );
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjGroupProjectilePlayer::~CGCObjGroupProjectilePlayer()
{}



//////////////////////////////////////////////////////////////////////////
// only handle invaders
//////////////////////////////////////////////////////////////////////////
//virtual 
bool CGCObjGroupProjectilePlayer::VHandlesThisTypeId( GCTypeID idQueryType )
{
	return( GetGCTypeIDOf( CGCObjProjectilePlayer ) == idQueryType );
}



//////////////////////////////////////////////////////////////////////////
// must return the typeid of the CGCObjectGroup derived class
//////////////////////////////////////////////////////////////////////////
//virtual 
GCTypeID CGCObjGroupProjectilePlayer::VGetTypeId()
{
	return GetGCTypeIDOf( CGCObjGroupProjectilePlayer );
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupProjectilePlayer::CreateProjectiles()
{
	// n.b. these register themselves with this class on creation via CGCObject & CGCObjectManager
	for( u32 uLoop = 0; uLoop < k_uNumInvaders; ++uLoop )
	{
		new CGCObjProjectilePlayer(); 
	}
}



//////////////////////////////////////////////////////////////////////////
//
struct SArrayOfProjectiles
{
	u32 uCount;
	CGCObjProjectilePlayer* apProjectiles[ CGCObjectGroup::EMaxGCObjects ];
};
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupProjectilePlayer::DestroyProjectiles()
{
	// this iterates the array of registered CGCObjects 
	// calling the supplied functor then deleting them
	DestroyObjectsReverseOrder( [&]( CGCObject* pObject )
	{
		// do nothing - DestroyObjectsReverseOrder calls delete!
		GCASSERT( GetGCTypeIDOf( CGCObjProjectilePlayer ) == pObject->GetGCTypeID(), "wrong type!" );
	});
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupProjectilePlayer::SpawnProjectile( Vec2 v2Position, Vec2 v2Velocity, f32 fMaxLifeTime )
{
	// check we have a projectile to spawn...
	if( GetCountDead() )
	{
		// this case is safe because we know what type this group manages
		CGCObjProjectilePlayer* pProjectile = static_cast< CGCObjProjectilePlayer* >( GetDeadObject() );

		// set up the projectile
		pProjectile->SetSpritePosition( v2Position );
		pProjectile->SetVelocity( v2Velocity );
		pProjectile->SetLifeTime( fMaxLifeTime );

		// resurrect it to fire	it
		VOnObjectResurrect( pProjectile	);
	}
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// called from CGCObjectManager::Initialise
//virtual 
void CGCObjGroupProjectilePlayer::VOnGroupResourceAcquire()
{
	CreateProjectiles();
	CGCObjectGroup::VOnGroupResourceAcquire();
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupProjectilePlayer::VOnGroupResourceRelease()
{
	// need to do this first as it resets the state of internal lists
	CGCObjectGroup::VOnGroupResourceRelease();
	DestroyProjectiles();
}


