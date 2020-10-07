
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


#include "CCollectible.h"
#include "CCollectiblePool.h"




//////////////////////////////////////////////////////////////////////////
// using
using namespace cocos2d;



//////////////////////////////////////////////////////////////////////////
// N.B. we want reset to put the projectiles on the dead list
//////////////////////////////////////////////////////////////////////////
CCollectiblePool::CCollectiblePool()
{
	SetResetBehaviour(CGCObjectGroup::EResetDead);
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// virtual
CCollectiblePool::~CCollectiblePool()
{}



//////////////////////////////////////////////////////////////////////////
// only handle invaders
//////////////////////////////////////////////////////////////////////////
//virtual 
bool CCollectiblePool::VHandlesThisTypeId(GCTypeID idQueryType)
{
	return(GetGCTypeIDOf(CCollectible) == idQueryType);
}



//////////////////////////////////////////////////////////////////////////
// must return the typeid of the CGCObjectGroup derived class
//////////////////////////////////////////////////////////////////////////
//virtual 
GCTypeID CCollectiblePool::VGetTypeId()
{
	return GetGCTypeIDOf(CGCObjGroupProjectilePlayer);
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CCollectiblePool::CreateCollectible()
{
	// n.b. these register themselves with this class on creation via CGCObject & CGCObjectManager
	for (u32 uLoop = 0; uLoop < k_uNumberOfCollectibles; ++uLoop)
	{
		new CCollectible();
	}
}



//////////////////////////////////////////////////////////////////////////
void CCollectiblePool::DestroyCollectible()
{
	// this iterates the array of registered CGCObjects 
	// calling the supplied functor then deleting them
	DestroyObjectsReverseOrder([&](CGCObject* pObject)
	{
		// this is called immediately before the instance is deleted
		GCASSERT(GetGCTypeIDOf(CCollectible) == pObject->GetGCTypeID(), "wrong type!");
	});
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CCollectiblePool::SpawnCollectible(Vec2 v2Position)
{
	// check we have a projectile to spawn...
	if (GetCountDead())
	{
		// this case is safe because we know what type this group manages
		CCollectible* pCollectible = static_cast<CCollectible*>(GetDeadObject());

		// set up the projectile
		pCollectible->SetSpritePosition(v2Position);

		// resurrect it to fire	it
		VOnObjectResurrect(pCollectible);
	}
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// called from CGCObjectManager::Initialise
//virtual 
void CCollectiblePool::VOnGroupResourceAcquire()
{
	CreateCollectible();
	CGCObjectGroup::VOnGroupResourceAcquire();
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CCollectiblePool::VOnGroupResourceRelease()
{
	// need to do this first as it resets the state of internal lists
	CGCObjectGroup::VOnGroupResourceRelease();
	DestroyCollectible();
}


