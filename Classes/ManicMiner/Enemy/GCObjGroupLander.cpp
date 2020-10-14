////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <vector>

#include "ManicMiner/Enemy/GCObjGroupLander.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/Core/GCTypes.h"
#include "GamerCamp/GCObject/GCObject.h"
#include "ManicMiner/Enemy/GCObjLander.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

// Static Creation Params lifted out of GCObjEnemy to here to allow GCObjEnemy to be a class which can represent differnt Enemy sprites/animations/physics.
static CGCFactoryCreationParams s_cCreationParams_CGCObj_ELander("CGCObjEnemy_ELander", "TexturePacker/Sprites/Coin/Coin.plist", "coin", b2_dynamicBody, true);

//////////////////////////////////////////////////////////////////////////
// using
using namespace cocos2d;

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CGCObjGroupLander::CGCObjGroupLander()
{
	m_v2FormationOrigin = Vec2::ZERO;
	bFirstPassDone = false;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjGroupLander::~CGCObjGroupLander()
{}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupLander::SetFormationOrigin( Vec2 v2FormationOrigin )
{
	m_v2FormationOrigin = v2FormationOrigin;
}

//////////////////////////////////////////////////////////////////////////
// only handle invaders
//////////////////////////////////////////////////////////////////////////
//virtual 
bool CGCObjGroupLander::VHandlesThisTypeId( GCTypeID idQueryType )
{
	return( GetGCTypeIDOf( CGCObjLander ) == idQueryType );
}

//////////////////////////////////////////////////////////////////////////
// must return the typeid of the CGCObjectGroup derived class
//////////////////////////////////////////////////////////////////////////
//virtual 
GCTypeID CGCObjGroupLander::VGetTypeId()
{
	return GetGCTypeIDOf( CGCObjGroupLander );
}

//////////////////////////////////////////////////////////////////////////
// Function to be run at initialisation to remove unwanted landers
//////////////////////////////////////////////////////////////////////////

void CGCObjGroupLander::KillLandersAccordingToLanderActivators()
{
	int iPosition = 0;
	ForEachObject([&](CGCObject* pObject) -> bool
		{
			CCAssert((GetGCTypeIDOf(CGCObjLander) == pObject->GetGCTypeID()),
				"CGCObject derived type mismatch!");
			CGCObjLander* pItemObj = (CGCObjLander*)pObject;
			if (!vLanderActivators[iPosition]) {
				CGCObjectManager::ObjectKill(pItemObj);
			}
			iPosition++;
			return true;
		});

}

// Function to initialise the Lander Activator array to represent random lander(s).
// (ie, Keep iterating through the vector until 'iNumberToAllocate' have been chosen).
// Algorithim written this way to avoid any left/right bias to the allocation.
void CGCObjGroupLander::AllocateRandomLander(int iNumberToAllocate)
{
	int iTotalPicked = 0;
	while (iTotalPicked < iNumberToAllocate)
	{
		//pick a random anchor point 
		int iRand = cocos2d::RandomHelper::random_int(0, k_iMaxLanderAnchorPoints - 1);

		// if that anchor point not already been chosen then set it True and advance total picked.
		if (!vLanderActivators[iRand])
		{
			vLanderActivators[iRand] = true;
			iTotalPicked++;
		}
	}
}

void CGCObjGroupLander::VOnGroupResourceAcquire()
{
	CGCObjectGroup::VOnGroupResourceAcquire();
	
	// Initialse the vector of Lander Activations to the max allowed.
	vLanderActivators = std::vector<bool>(k_iMaxLanderAnchorPoints, false);
	AllocateRandomLander(k_iMaxActiveLanders);

	// Initialise the vector of Lander anchor positions to step across the screen.
	vLanderAnchorPoints = std::vector<cocos2d::Vec2>(k_iMaxLanderAnchorPoints);
	
	int step = k_iAnchorPointStepDistance;
	for (auto &AnchorPoint : vLanderAnchorPoints)
	{
		AnchorPoint = cocos2d::Vec2(step, k_iAnchorPointYPosition);
	    step += k_iAnchorPointStepDistance;
	}
	
	CreateLanders();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupLander::VOnGroupResourceAcquire_PostObject()
{
	
	// parent class version
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();


    // set up animations for pAnimation group 2
	const char* pszPlist_Coin = "TexturePacker/Sprites/Coin/Coin.plist";
	const char* pszAnim_Coin_Rotate = "Rotate";

	ValueMap& rdicPList1 = GCCocosHelpers::CreateDictionaryFromPlist(pszPlist_Coin);
	Animation* pAnimation1 = GCCocosHelpers::CreateAnimation(rdicPList1, pszAnim_Coin_Rotate);

	ForEachObject( [&] ( CGCObject* pObject) -> bool
	{
		CCAssert( ( GetGCTypeIDOf( CGCObjLander ) == pObject->GetGCTypeID() ),
			"CGCObject derived type mismatch!" );

		CGCObjSprite* pItemSprite = (CGCObjSprite*)pObject;
		CGCObjLander* pItemObj = (CGCObjLander*)pObject;

		// Create the animation for each Enemy in the group according to the cAnimationLookup map
		pItemSprite->RunAction(GCCocosHelpers::CreateAnimationActionLoop( pAnimation1 ));

		
		return true;
	} );

}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupLander::VOnGroupResourceRelease()
{
	// N.B. need to do this first as it clears internal lists
	CGCObjectGroup::VOnGroupResourceRelease();
	DestroyLanders();
}

//////////////////////////////////////////////////////////////////////////
void CGCObjGroupLander::CreateLanders()
{
	for (int iLoop = 0; iLoop < k_iMaxLanderAnchorPoints; ++iLoop)
	{
		CGCObjLander* pLander = new CGCObjLander(vLanderAnchorPoints[iLoop], 2.0f, s_cCreationParams_CGCObj_ELander);
		pLander->SetName("Lander");
	}
}

void CGCObjGroupLander::VOnGroupReset()
{
	CGCObjectGroup::VOnGroupReset();
}

void CGCObjGroupLander::VOnGroupUpdate(f32 fTimeStep)
{

	CGCObjectGroup::VOnGroupUpdate(fTimeStep);
	
	// NOTE - THIS FIRST PASS CONCEPT IS A TEMPORARY SOLUTION UNTIL I DECIDE WHERE THE LANDER
	// INITIAL KILLING OF LANDERS NOT REQUIRED IS TO GO
	if (!bFirstPassDone) {
		KillLandersAccordingToLanderActivators();
	}

	bFirstPassDone = true;
	
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupLander::DestroyLanders()
{
	// this iterates the array of registered CGCObjects 
	// calling the supplied functor then deleting them

	DestroyObjectsReverseOrder([&](CGCObject* pObject)
		{
			// do nothing - DestroyObjectsReverseOrder calls delete!
			GCASSERT(GetGCTypeIDOf(CGCObjLander) == pObject->GetGCTypeID(), "wrong type!");

		});
}
