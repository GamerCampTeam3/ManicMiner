////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include "ManicMiner/Hazards/GCObjGroupHazard.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "ManicMiner/Hazards/GCObjHazard.h"
#include "ManicMiner/Hazards/CentralCavern/GCObjGroupHazardCentralCavern.h"

// Static Creation Params lifted out of GCObjHazard to here to allow GCObjHazard to be a class which can represent differnt Hazard sprites/animations/physics.
//static CGCFactoryCreationParams s_cCreationParams_CGCObj_EBush("CGCObjHazard_EBush", "TexturePacker/Sprites/KoopaTrooper/KoopaTrooper.plist", "koopa", b2_staticBody, true);
static CGCFactoryCreationParams s_cCreationParams_CGCObj_EBush("CGCObjHazard_EBush", "TexturePacker/Sprites/egg/egg.plist", "egg", b2_dynamicBody, true);
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CGCObjGroupHazardCentralCavern::CGCObjGroupHazardCentralCavern()
{}

//////////////////////////////////////////////////////////////////////////
// virtual 
//////////////////////////////////////////////////////////////////////////
CGCObjGroupHazardCentralCavern::~CGCObjGroupHazardCentralCavern()
{}

//////////////////////////////////////////////////////////////////////////
// virtual - overrides base class CGCObjGroupHazard
// This function allocates an animation to an enemy.
// First the enemy type is queried via the function GetHazardIdentifier() 
// (this identifier was passed into the enemy when it was instantiated).
// Secondly the HazardIdentifier is used to index a unordered_map of
// Hazard Identifiers against the associated animation.
// Note the commented out code below shows how the system would be expanded for 
// a level with more than one enemey.
void CGCObjGroupHazardCentralCavern::VOnGroupResourceAcquire_PostObject()
{
	// Call parent class
	CGCObjGroupHazard::VOnGroupResourceAcquire_PostObject();
	
		// set up animations for pAnimation group 1
	const char* pszPlist2 = "TexturePacker/Sprites/KoopaTrooper/KoopaTrooper.plist";
	const char* pszAnim_Fly2 = "Fly";

	// make an animation
	cocos2d::ValueMap& rdictPList1 = GCCocosHelpers::CreateDictionaryFromPlist(pszPlist2);
	cocos2d::Animation* pAnimation1 = GCCocosHelpers::CreateAnimation(rdictPList1, pszAnim_Fly2);
	
	// set up animations for pAnimation group 2 - NOTE EXAMPLE FOR FUTURE LEVELS.
	//const char* pszPlist_Coin = "TexturePacker/Sprites/Coin/Coin.plist";
	//const char* pszAnim_Coin_Rotate = "Rotate";

	//cocos2d::ValueMap& rdicPList2 = GCCocosHelpers::CreateDictionaryFromPlist(pszPlist_Coin);
	//cocos2d::Animation* pAnimation2 = GCCocosHelpers::CreateAnimation(rdicPList2, pszAnim_Coin_Rotate);

	// Create a dictionary of HazardId's mapped to their corresponding animation.
	std::unordered_map<HazardTypes::EHazardId, cocos2d::Animation*> cAnimationLookup = {

		{HazardTypes::EHazardId::EBush, pAnimation1}
		//{HazardTypes::EHazardId::EPenguin, pAnimation2},  <----Example for level with multiple enemies.

	};

	ForEachObject( [&] ( CGCObject* pObject) -> bool
	{
		CCAssert( ( GetGCTypeIDOf( CGCObjHazard ) == pObject->GetGCTypeID() ),
			"CGCObject derived type mismatch!" );

		CGCObjSprite* pItemSprite = (CGCObjSprite*)pObject;
		CGCObjHazard* pItemObj = (CGCObjHazard*)pObject;

		// Create the animation for each Hazard in the group according to the Hazard Identifier indexed into the cAnimationLookup map
		//pItemSprite->RunAction(GCCocosHelpers::CreateAnimationActionLoop( cAnimationLookup[pItemObj->GetHazardIdentifier()]));

		return true;
	} );
}

//////////////////////////////////////////////////////////////////////////
// virtual - overrides base class CGCObjGroupHazard
// This function instantiates all required enemies for the Central Cavern level.
//

void CGCObjGroupHazardCentralCavern::VOnGroupResourceAcquire() 
{
	// Call parent class
	CGCObjGroupHazard::VOnGroupResourceAcquire();

	CGCObjHazard* pHazard1 = new CGCObjHazard(cocos2d::Vec2(700.0f, 200.0f), HazardTypes::EHazardId::EBush, s_cCreationParams_CGCObj_EBush);
	CGCObjHazard* pHazard2 = new CGCObjHazard(cocos2d::Vec2(1250.0f, 440.0f), HazardTypes::EHazardId::EBush, s_cCreationParams_CGCObj_EBush);
	CGCObjHazard* pHazard3 = new CGCObjHazard(cocos2d::Vec2(1450.0f, 680.0f), HazardTypes::EHazardId::EBush, s_cCreationParams_CGCObj_EBush);
	CGCObjHazard* pHazard4 = new CGCObjHazard(cocos2d::Vec2(1650.0f, 680.0f), HazardTypes::EHazardId::EBush, s_cCreationParams_CGCObj_EBush);

	CGCObjHazard* pHazard5 = new CGCObjHazard(cocos2d::Vec2(740.0f, 950.0f), HazardTypes::EHazardId::EBush, s_cCreationParams_CGCObj_EBush);
	CGCObjHazard* pHazard6 = new CGCObjHazard(cocos2d::Vec2(940.0f, 950.0f), HazardTypes::EHazardId::EBush, s_cCreationParams_CGCObj_EBush);

	//pHazard->SetName("H1");
}


//////////////////////////////////////////////////////////////////////////
// Fucntion to return the typeid of the GCObjectGroup derived class
//////////////////////////////////////////////////////////////////////////
//virtual 
GCTypeID CGCObjGroupHazardCentralCavern::VGetTypeId()
{
	return GetGCTypeIDOf(CGCObjGroupHazardCentralCavern);
}