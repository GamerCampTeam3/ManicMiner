////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <vector>

#include "ManicMiner/Enemy/SkylabLandingBay/GCObjGroupLander.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/Core/GCTypes.h"
#include "GamerCamp/GCObject/GCObject.h"
#include "ManicMiner/Enemy/SkylabLandingBay/GCObjLander.h"
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
	m_bFirstPassDone = false;
	
	fStartupCounter = 0.0;
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

void CGCObjGroupLander::RegisterLanderCollision(GCTypeID idLander)
{
	
	// First find and kill the Lander.
	int iPosition = 0;
	ForEachObjectIn_LiveList([&](CGCObject* pObject) -> bool
		{
			CCAssert((GetGCTypeIDOf(CGCObjLander) == pObject->GetGCTypeID()),
				"CGCObject derived type mismatch!");
			CGCObjLander* pItemObj = (CGCObjLander*)pObject;

			if (pItemObj->GetGCTypeID() == idLander)
			{
				// Kill the lander and reset the vector boolean to match.
				CGCObjectManager::ObjectKill(pItemObj);
			//	vLanderActivators[iPosition] = false;
				
				// Once collided object has been found we can exit.
				return true;
			}

			iPosition++;

			return true;
		});


	//Start a timer 



	//Next activate one new Lander.
	//int iAllocatedPosition = CGCObjGroupLander::AllocateRandomLanders(1);

	//ResurrectList_AddBack




}

//////////////////////////////////////////////////////////////////////////
// Function to be run at initialisation to remove unwanted landers
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupLander::KillLandersNotRequiredAtLevelStartup()
{
	int iPosition = 0;
	ForEachObject([&](CGCObject* pObject) -> bool
		{
			CCAssert((GetGCTypeIDOf(CGCObjLander) == pObject->GetGCTypeID()),
				"CGCObject derived type mismatch!");
			CGCObjLander* pItemObj = (CGCObjLander*)pObject;
		//	if (!vLanderActivators[iPosition]) {
				CGCObjectManager::ObjectKill(pItemObj);
		//	}
			iPosition++;
			return true;
		});

}

//////////////////////////////////////////////////////////////////////////
// Function to initialise the Lander Activator array to represent random lander(s).
// (ie, Keep iterating through the vector until 'iNumberToAllocate' have been chosen).
// Algorithim written this way to avoid any left/right bias to the allocation.
// Function returns the last allocation position made (usefull when when called with
// iNumberToAllocate equal to 1).
//
// eg to help visualise the Skylab landing bay level:
//      oo    = Lander Anchor points.
//		XXXXX = platform.
// -------------------------------------------------------------------------------------------
//        oo         oo         oo         oo        oo       oo        oo       oo      oo 
//
//                                                                    XXXXX
//
//                                       XXXXX
//        XXXXX                     
//
int CGCObjGroupLander::AllocateRandomLanders(int iNumberToAllocate)
{
	CCAssert((iNumberToAllocate > 0 || (iNumberToAllocate <= k_iMaxLanderAnchorPoints)),
		"CGCObjectGroupLander: AllocateRandomLander has been called with an out of range input parameter.");

	int iTotalPicked = 0;
	int iReturnValue = 0;
	while (iTotalPicked < iNumberToAllocate)
{
		//pick a random anchor point 
		int iRand = cocos2d::RandomHelper::random_int(0, k_iMaxLanderAnchorPoints - 1);

		//if that anchor point not already been chosen then set it True and advance total picked.
		if (!vLanderActivators[iRand])
		{
			vLanderActivators[iRand] = true;
			iTotalPicked++;
		}
		iReturnValue = iRand;
	}
	return iReturnValue;
}

void CGCObjGroupLander::VOnGroupResourceAcquire()
{
	CGCObjectGroup::VOnGroupResourceAcquire();
	

	// Lander 1
	LanderConfigVars.push_back({ 0.0f, 7.0f });
	
	// Lander 2
	LanderConfigVars.push_back({ 11.0f, 11.0f });
	
	// Lander 3
	LanderConfigVars.push_back({ 4.0f, 7.0f });

	StartupTimingsThresholdQueue.push(0.0);
	StartupTimingsThresholdQueue.push(4.0);
	StartupTimingsThresholdQueue.push(7.0);
	StartupTimingsThresholdQueue.push(11.0);


	// Initialse the vector of Lander Activations to the max allowed.
	//vLanderActivators = std::vector<bool>(k_iMaxLanderAnchorPoints, false);
	
	
	//AllocateRandomLanders(k_iMaxActiveLanders);

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

		// Store the GCTypeID in a map to allow an index from the GCTypeID to the anchor position.
		LanderGCTypeIDLookup.insert({pLander->GetGCTypeID(), iLoop});

	}
}

void CGCObjGroupLander::VOnGroupReset()
{
	CGCObjectGroup::VOnGroupReset();
}


void CGCObjGroupLander::ActivateLanders(float fTimeStamp)
{

	int iPosition = 0;
	ForEachObject_InDeadList([&](CGCObject* pObject) -> bool
		{
			CCAssert((GetGCTypeIDOf(CGCObjLander) == pObject->GetGCTypeID()),
				"CGCObject derived type mismatch!");
			CGCObjLander* pItemObj = (CGCObjLander*)pObject;

			
			//iPosition++;

			return true;
		});


}


void CGCObjGroupLander::VOnGroupUpdate(f32 fTimeStep)
{

	CGCObjectGroup::VOnGroupUpdate(fTimeStep);
	
	// NOTE - THIS FIRST PASS CONCEPT IS A TEMPORARY SOLUTION UNTIL I DECIDE WHERE THE LANDER
	// INITIAL KILLING OF LANDERS NOT REQUIRED IS TO GO
	if (!m_bFirstPassDone) {
		KillLandersNotRequiredAtLevelStartup();
	}


	if (!StartupTimingsThresholdQueue.empty())
	{
		fStartupCounter += fTimeStep;

		// If the next timing threshold in the queue is passed then activate the associated Landers 
		if (fStartupCounter >= StartupTimingsThresholdQueue.front())
		{
			ActivateLanders(StartupTimingsThresholdQueue.front());


			
			StartupTimingsThresholdQueue.pop();
		}

	}




	m_bFirstPassDone = true;
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
