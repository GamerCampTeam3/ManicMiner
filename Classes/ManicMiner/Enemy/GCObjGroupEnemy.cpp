////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>

#include "ManicMiner/Enemy/GCObjGroupEnemy.h"

#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/Core/GCTypes.h"
#include "GamerCamp/GCObject/GCObject.h"
#include "ManicMiner/Enemy/GCObjEnemy.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "ManicMiner/Enemy/GCEnemyDataStore.h"

// Static Creation Params lifted out of GCObjEnemy to here to allow GCObjEnemy to be a class which can represent differnt Enemy sprites/animations/physics.
static CGCFactoryCreationParams s_cCreationParams_CGCObj_EDuck("CGCObjEnemy_EDuck", "TexturePacker/Sprites/Coin/Coin.plist", "coin", b2_dynamicBody, true);
static CGCFactoryCreationParams s_cCreationParams_CGCObj_EPenguin("CGCObjEnemy_EPenguin", "TexturePacker/Sprites/KoopaTrooper/KoopaTrooper.plist", "koopa", b2_dynamicBody, true);

//////////////////////////////////////////////////////////////////////////
// using
using namespace cocos2d;

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CGCObjGroupEnemy::CGCObjGroupEnemy()
{
	m_v2FormationOrigin = Vec2::ZERO;

	c_pcGCEnemyDataStore = new CGCEnemyDataStore();

}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjGroupEnemy::~CGCObjGroupEnemy()
{}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupEnemy::SetFormationOrigin( Vec2 v2FormationOrigin )
{
	m_v2FormationOrigin = v2FormationOrigin;
}

//////////////////////////////////////////////////////////////////////////
// only handle invaders
//////////////////////////////////////////////////////////////////////////
//virtual 
bool CGCObjGroupEnemy::VHandlesThisTypeId( GCTypeID idQueryType )
{
	return( GetGCTypeIDOf( CGCObjEnemy ) == idQueryType );
}

//////////////////////////////////////////////////////////////////////////
// must return the typeid of the CGCObjectGroup derived class
//////////////////////////////////////////////////////////////////////////
//virtual 
GCTypeID CGCObjGroupEnemy::VGetTypeId()
{
	return GetGCTypeIDOf( CGCObjGroupEnemy );
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupEnemy::VOnGroupResourceAcquire()
{
	CGCObjectGroup::VOnGroupResourceAcquire();
	CreateEnemies();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupEnemy::VOnGroupResourceAcquire_PostObject()
{
	
	// parent class version
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();

	// set up animations for pAnimation group 1
	const char* pszPlist2 = "TexturePacker/Sprites/KoopaTrooper/KoopaTrooper.plist";
	const char* pszAnim_Fly2 = "Fly";

	// make an animation
	ValueMap& rdictPList1 = GCCocosHelpers::CreateDictionaryFromPlist(pszPlist2);
	Animation* pAnimation1 = GCCocosHelpers::CreateAnimation(rdictPList1, pszAnim_Fly2);


	// set up animations for pAnimation group 2
	const char* pszPlist_Coin = "TexturePacker/Sprites/Coin/Coin.plist";
	const char* pszAnim_Coin_Rotate = "Rotate";

	ValueMap& rdicPList2 = GCCocosHelpers::CreateDictionaryFromPlist(pszPlist_Coin);
	Animation* pAnimation2 = GCCocosHelpers::CreateAnimation(rdicPList2, pszAnim_Coin_Rotate);


	// Create a dictionary of EnemyID's mapped to their corresponding animation.
	std::unordered_map<EnemyTypes::EEnemyId, cocos2d::Animation*> cAnimationLookup = {

		{EnemyTypes::EEnemyId::EDuck, pAnimation1},
		{EnemyTypes::EEnemyId::EPenguin, pAnimation2},

	};

	ForEachObject( [&] ( CGCObject* pObject) -> bool
	{
		CCAssert( ( GetGCTypeIDOf( CGCObjEnemy ) == pObject->GetGCTypeID() ),
			"CGCObject derived type mismatch!" );

		CGCObjSprite* pItemSprite = (CGCObjSprite*)pObject;
		CGCObjEnemy* pItemObj = (CGCObjEnemy*)pObject;

		// Create the animation for each Enemy in the group according to the cAnimationLookup map
		pItemSprite->RunAction(GCCocosHelpers::CreateAnimationActionLoop( cAnimationLookup[pItemObj->GetEnemyIdentifier()]));

		return true;
	} );
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupEnemy::VOnGroupResourceRelease()
{
	// N.B. need to do this first as it clears internal lists
	CGCObjectGroup::VOnGroupResourceRelease();
	DestroyEnemies();
}


//////////////////////////////////////////////////////////////////////////
// 

//////////////////////////////////////////////////////////////////////////
void CGCObjGroupEnemy::CreateEnemies()
{

	  
		// koopas

		CGCObjEnemy* pEnemy = new CGCObjEnemy(CGCObjEnemy::EMovementAxis_LeftRight, cocos2d::Vec2(50.0f,100.0f), 500.0f, 300.0f, false, 2.0f, true, EnemyTypes::EEnemyId::EDuck, s_cCreationParams_CGCObj_EDuck);
		pEnemy->SetName("E1");

		CGCObjEnemy* pEnemy2 = new CGCObjEnemy(CGCObjEnemy::EMovementAxis_LeftRight, cocos2d::Vec2(100.0f, 250.0f), 550.0f, 200.0f, true, 1.0f, true, EnemyTypes::EEnemyId::EDuck, s_cCreationParams_CGCObj_EDuck);
		pEnemy2->SetName("E2");

		CGCObjEnemy* pEnemy3 = new CGCObjEnemy(CGCObjEnemy::EMovementAxis_LeftRight, cocos2d::Vec2(150.0f, 400.0f), 250.0f, 200.0f, true, 1.0f, true, EnemyTypes::EEnemyId::EDuck, s_cCreationParams_CGCObj_EDuck);
		pEnemy3->SetName("E3");


		

		// coins

		CGCObjEnemy* pEnemy4 = new CGCObjEnemy(CGCObjEnemy::EMovementAxis_UpDown, cocos2d::Vec2(700.0f, 500.0f), 50.0f, 0.0f, true, 0.3f, true, EnemyTypes::EEnemyId::EPenguin, s_cCreationParams_CGCObj_EPenguin);
		pEnemy4->SetName("E4");

		CGCObjEnemy* pEnemy5 = new CGCObjEnemy(CGCObjEnemy::EMovementAxis_UpDown, cocos2d::Vec2(800.0f, 550.0f), 100.0f, 5.0f, false, 1.0f, true, EnemyTypes::EEnemyId::EPenguin, s_cCreationParams_CGCObj_EPenguin);
		pEnemy5->SetName("E5");

		CGCObjEnemy* pEnemy6 = new CGCObjEnemy(CGCObjEnemy::EMovementAxis_UpDown, cocos2d::Vec2(900.0f, 100.0f), 500.0f, 0.0f, true, 0.5f, true, EnemyTypes::EEnemyId::EPenguin, s_cCreationParams_CGCObj_EPenguin);
		pEnemy6->SetName("E6");
		 
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupEnemy::DestroyEnemies()
{
	// this iterates the array of registered CGCObjects 
	// calling the supplied functor then deleting them

	DestroyObjectsReverseOrder([&](CGCObject* pObject)
		{
			// do nothing - DestroyObjectsReverseOrder calls delete!
			GCASSERT(GetGCTypeIDOf(CGCObjEnemy) == pObject->GetGCTypeID(), "wrong type!");

		});
}
