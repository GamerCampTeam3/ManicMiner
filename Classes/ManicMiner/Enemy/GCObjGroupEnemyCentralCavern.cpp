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
#include "ManicMiner/Enemy/GCObjGroupEnemyCentralCavern.h"

// Static Creation Params lifted out of GCObjEnemy to here to allow GCObjEnemy to be a class which can represent differnt Enemy sprites/animations/physics.
static CGCFactoryCreationParams s_cCreationParams_CGCObj_EDuck("CGCObjEnemy_EDuck", "TexturePacker/Sprites/KoopaTrooper/KoopaTrooper.plist", "koopa", b2_dynamicBody, true);

//////////////////////////////////////////////////////////////////////////
// using
using namespace cocos2d;

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CGCObjGroupEnemyCentralCavern::CGCObjGroupEnemyCentralCavern()
{}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjGroupEnemyCentralCavern::~CGCObjGroupEnemyCentralCavern()
{}

//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupEnemyCentralCavern::VOnGroupResourceAcquire_PostObject()
{
	// Call parent class
	CGCObjGroupEnemy::VOnGroupResourceAcquire_PostObject();
	
		// set up animations for pAnimation group 1
	const char* pszPlist2 = "TexturePacker/Sprites/KoopaTrooper/KoopaTrooper.plist";
	const char* pszAnim_Fly2 = "Fly";

	// make an animation
	ValueMap& rdictPList1 = GCCocosHelpers::CreateDictionaryFromPlist(pszPlist2);
	Animation* pAnimation1 = GCCocosHelpers::CreateAnimation(rdictPList1, pszAnim_Fly2);


	// set up animations for pAnimation group 2
	//const char* pszPlist_Coin = "TexturePacker/Sprites/Coin/Coin.plist";
	//const char* pszAnim_Coin_Rotate = "Rotate";

	//ValueMap& rdicPList2 = GCCocosHelpers::CreateDictionaryFromPlist(pszPlist_Coin);
	//Animation* pAnimation2 = GCCocosHelpers::CreateAnimation(rdicPList2, pszAnim_Coin_Rotate);


	// Create a dictionary of EnemyID's mapped to their corresponding animation.
	std::unordered_map<EnemyTypes::EEnemyId, cocos2d::Animation*> cAnimationLookup = {

		{EnemyTypes::EEnemyId::EDuck, pAnimation1}
		//{EnemyTypes::EEnemyId::EPenguin, pAnimation2},

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


void CGCObjGroupEnemyCentralCavern::VOnGroupResourceAcquire() 
{
	// Call parent class
	CGCObjGroupEnemy::VOnGroupResourceAcquire();
	
	CGCObjGroupEnemyCentralCavern::CreateEnemies();
}


void CGCObjGroupEnemyCentralCavern::CreateEnemies()
{
	// Central Cavern requires only one enemy.
	CGCObjEnemy* pEnemy = new CGCObjEnemy(CGCObjEnemy::EMovementAxis_LeftRight, cocos2d::Vec2(480.0f,480.0f), 480.0f, 0.0f, true, 1.0f, true, EnemyTypes::EEnemyId::EDuck, s_cCreationParams_CGCObj_EDuck);
	pEnemy->SetName("E1");
 
}
