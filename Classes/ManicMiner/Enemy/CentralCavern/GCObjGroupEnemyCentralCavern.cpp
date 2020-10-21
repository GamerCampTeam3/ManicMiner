

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include "ManicMiner/Enemy/GCObjGroupEnemy.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "ManicMiner/Enemy/GCObjEnemy.h"
#include "ManicMiner/Enemy/CentralCavern/GCObjGroupEnemyCentralCavern.h"

// Static Creation Params lifted out of GCObjEnemy to here to allow GCObjEnemy to be a class which can represent differnt Enemy sprites/animations/physics.
static CGCFactoryCreationParams s_cCreationParams_CGCObj_EDuck("CGCObjEnemy_EDuck", "TexturePacker/Sprites/Duck/Duck.plist", "cc_enemy_duck", b2_dynamicBody, true);

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CGCObjGroupEnemyCentralCavern::CGCObjGroupEnemyCentralCavern()
{}

//////////////////////////////////////////////////////////////////////////
// virtual 
//////////////////////////////////////////////////////////////////////////
CGCObjGroupEnemyCentralCavern::~CGCObjGroupEnemyCentralCavern()
{}

//////////////////////////////////////////////////////////////////////////
// virtual - overrides base class CGCObjGroupEnemy
// This function allocates an animation to an enemy.
// First the enemy type is queried via the function GetEnemyIdentifier() 
// (this identifier was passed into the enemy when it was instantiated).
// Secondly the EnemyIdentifier is used to index a unordered_map of
// Enemy Identifiers against the associated animation.
// Note the commented out code below shows how the system would be expanded for 
// a level with more than one enemey.
void CGCObjGroupEnemyCentralCavern::VOnGroupResourceAcquire_PostObject()
{
	
	// set up animations for pAnimation group 1
	//const char* pszPlist2 = "TexturePacker/Sprites/KoopaTrooper/KoopaTrooper.plist";
	//const char* pszAnim_Fly2 = "Fly";

	// make an animation
	//cocos2d::ValueMap& rdictPList1 = GCCocosHelpers::CreateDictionaryFromPlist(pszPlist2);
	//cocos2d::Animation* pAnimation1 = GCCocosHelpers::CreateAnimation(rdictPList1, pszAnim_Fly2);
	
	// set up animations for pAnimation group 2 - NOTE EXAMPLE FOR FUTURE LEVELS.
	//const char* pszPlist_Coin = "TexturePacker/Sprites/Coin/Coin.plist";
	//const char* pszAnim_Coin_Rotate = "Rotate";

	//cocos2d::ValueMap& rdicPList2 = GCCocosHelpers::CreateDictionaryFromPlist(pszPlist_Coin);
	//cocos2d::Animation* pAnimation2 = GCCocosHelpers::CreateAnimation(rdicPList2, pszAnim_Coin_Rotate);

	// Create a dictionary of EnemyId's mapped to their corresponding animation.
	//std::unordered_map<EnemyTypes::EEnemyId, cocos2d::Animation*> cAnimationLookup = {

	//	{EnemyTypes::EEnemyId::EDuck, pAnimation1}
		//{EnemyTypes::EEnemyId::EPenguin, pAnimation2},  <----Example for level with multiple enemies.

	//};

	ForEachObject( [&] ( CGCObject* pObject) -> bool
	{
		CCAssert( ( GetGCTypeIDOf( CGCObjEnemy ) == pObject->GetGCTypeID() ),
			"CGCObject derived type mismatch!" );

		CGCObjSprite* pItemSprite = (CGCObjSprite*)pObject;
		CGCObjEnemy* pItemObj = (CGCObjEnemy*)pObject;

		// Create the animation for each Enemy in the group according to the Enemy Identifier indexed into the cAnimationLookup map
		//pItemSprite->RunAction(GCCocosHelpers::CreateAnimationActionLoop( cAnimationLookup[pItemObj->GetEnemyIdentifier()]));

		return true;
	} );
}

//////////////////////////////////////////////////////////////////////////
// virtual - overrides base class CGCObjGroupEnemy
// This function instantiates all required enemies for the Central Cavern level.
//

void CGCObjGroupEnemyCentralCavern::VOnGroupResourceAcquire() 
{
	// Central Cavern requires only one enemy.
	CGCObjEnemy* pcEnemy = new CGCObjEnemy(EnemyTypes::EMovementAxis::EMovementAxis_LeftRight, cocos2d::Vec2(510.0f, 450.0f), 480.0f, 0.0f, true, 1.0f, true, EnemyTypes::EEnemyId::EDuck, s_cCreationParams_CGCObj_EDuck);
	//pEnemy->SetName("E1");
}


//////////////////////////////////////////////////////////////////////////
// Fucntion to return the typeid of the GCObjectGroup derived class
//////////////////////////////////////////////////////////////////////////
//virtual 
GCTypeID CGCObjGroupEnemyCentralCavern::VGetTypeId()
{
	return GetGCTypeIDOf(CGCObjGroupEnemyCentralCavern);
}