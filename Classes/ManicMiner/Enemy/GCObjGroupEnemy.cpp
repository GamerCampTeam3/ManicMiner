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
static CGCFactoryCreationParams s_cCreationParams_CGCObj_Type1("CGCObjEnemy_Type1", "TexturePacker/Sprites/Coin/Coin.plist", "coin", b2_dynamicBody, true);
static CGCFactoryCreationParams s_cCreationParams_CGCObj_Type2("CGCObjEnemy_Type2", "TexturePacker/Sprites/KoopaTrooper/KoopaTrooper.plist", "koopa", b2_dynamicBody, true);

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


	pAnimation1 = nullptr;
	pAnimation2 = nullptr;
	
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




	// Experimenting below with moving the animations out of VOnGroupResourceAcquire
	// so pAnimation1 and pAnamiation2 are pointers in the .h file.
	// Not working at present 



	// set up animations for pAnimation group 1
	const char* pszPlist_Coin = "TexturePacker/Sprites/Coin/Coin.plist";
	const char* pszAnim_Coin_Rotate = "Rotate";

	ValueMap& rdicPList1 = GCCocosHelpers::CreateDictionaryFromPlist(pszPlist_Coin);
	pAnimation1 = GCCocosHelpers::CreateAnimation(rdicPList1, pszAnim_Coin_Rotate);






	// set up animations for pAnimation group 2
	const char* pszPlist2 = "TexturePacker/Sprites/KoopaTrooper/KoopaTrooper.plist";
	const char* pszAnim_Fly2 = "Fly";

	// make an animation
	ValueMap& rdictPList2 = GCCocosHelpers::CreateDictionaryFromPlist(pszPlist2);
	pAnimation2 = GCCocosHelpers::CreateAnimation(rdictPList2, pszAnim_Fly2);

	CreateEnemys();
	
	
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupEnemy::VOnGroupResourceAcquire_PostObject()
{
	
	// parent class version
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();
	   


	ForEachObject( [&] ( CGCObject* pObject) -> bool
	{
		CCAssert( ( GetGCTypeIDOf( CGCObjEnemy ) == pObject->GetGCTypeID() ),
			"CGCObject derived type mismatch!" );

		CGCObjSprite* pItemSprite = (CGCObjSprite*)pObject;
		CGCObjEnemy* pItemObj = (CGCObjEnemy*)pObject;



		// set up animations for pAnimation group 1
		const char* pszPlist_Coin = "TexturePacker/Sprites/Coin/Coin.plist";
		const char* pszAnim_Coin_Rotate = "Rotate";

		ValueMap& rdicPList1 = GCCocosHelpers::CreateDictionaryFromPlist(pszPlist_Coin);
		Animation* pAnimation1 = GCCocosHelpers::CreateAnimation(rdicPList1, pszAnim_Coin_Rotate);


		// set up animations for pAnimation group 2
		const char* pszPlist2 = "TexturePacker/Sprites/KoopaTrooper/KoopaTrooper.plist";
		const char* pszAnim_Fly2 = "Fly";

		// make an animation
		ValueMap& rdictPList2 = GCCocosHelpers::CreateDictionaryFromPlist(pszPlist2);
		Animation* pAnimation2 = GCCocosHelpers::CreateAnimation(rdictPList2, pszAnim_Fly2);



		// This is a temporary solution - plan to pass a pointer to a animation into the Enemy constructor instead,
		// so there will be no need to query the Enemy package for each EnemyIdentifierType as here.
		// (also Animation declarations above will be factored out into a separate EnemyDataStore).

		switch (pItemObj->GetEnemyIdentifier())
		{
		case CGCObjEnemy::EEnemyIdentifier_Type1:

			//pItemObj->SetAnimationAction();

			pItemSprite->RunAction(GCCocosHelpers::CreateAnimationActionLoop(pAnimation1));
			break;

		case CGCObjEnemy::EEnemyIdentifier_Type2:
	
			//pItemObj->SetAnimationAction();
			
			pItemSprite->RunAction(GCCocosHelpers::CreateAnimationActionLoop(pAnimation2));
			break;
		}
		
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
	DestroyEnemys();
}


//////////////////////////////////////////////////////////////////////////
// 

//////////////////////////////////////////////////////////////////////////
void CGCObjGroupEnemy::CreateEnemys()
{

	    // Basic initial version of Enemy creation - just instantiate separately for now until strategy developed further.
	    // Factory Creation params lifted out from GCObjEnemy to here.  This is because CGCObjEnemy (originally copied from CGCObjInvader) is 
	    // a generic Enemy store so can represent different Enemy's.

	    // Re-factoring planned to remove unncessary EEnemyTypeIdentifier_Type(x) input into constructor.
	    
	    CGCFactoryCreationParams& rFactoryCreationParams_Type1 = s_cCreationParams_CGCObj_Type1;
		CGCFactoryCreationParams& rFactoryCreationParams_Type2 = s_cCreationParams_CGCObj_Type2;




		// koopas

		CGCObjEnemy* pEnemy = new CGCObjEnemy(CGCObjEnemy::EMovementAxis_LeftRight, cocos2d::Vec2(50.0f,100.0f), 500.0f, 0.0f, 5.0f, CGCObjEnemy::EEnemyIdentifier_Type2, rFactoryCreationParams_Type2, pAnimation2);
		pEnemy->SetName("Derek2");

	

		CGCObjEnemy* pEnemy2 = new CGCObjEnemy(CGCObjEnemy::EMovementAxis_LeftRight, cocos2d::Vec2(100.0f, 200.0f), 400.0f, 0.0f, 8.0f, CGCObjEnemy::EEnemyIdentifier_Type2, rFactoryCreationParams_Type2, pAnimation2);
		pEnemy2->SetName("Derek3");

		


		CGCObjEnemy* pEnemy3 = new CGCObjEnemy(CGCObjEnemy::EMovementAxis_LeftRight, cocos2d::Vec2(150.0f, 300.0f), 250.0f, 0.0f, 2.5f, CGCObjEnemy::EEnemyIdentifier_Type2, rFactoryCreationParams_Type2, pAnimation2);
		pEnemy3->SetName("Derek4");


		

		// coins

		CGCObjEnemy* pEnemy4 = new CGCObjEnemy(CGCObjEnemy::EMovementAxis_UpDown, cocos2d::Vec2(700.0f, 500.0f), 50.0f, 0.0f, 2.5f, CGCObjEnemy::EEnemyIdentifier_Type1, rFactoryCreationParams_Type1, pAnimation1);
		pEnemy4->SetName("Derek5");

		

		CGCObjEnemy* pEnemy5 = new CGCObjEnemy(CGCObjEnemy::EMovementAxis_UpDown, cocos2d::Vec2(800.0f, 550.0f), 100.0f, 0.0f, 10.0f, CGCObjEnemy::EEnemyIdentifier_Type1, rFactoryCreationParams_Type1, pAnimation1);
		pEnemy5->SetName("Derek6");

		

		CGCObjEnemy* pEnemy6 = new CGCObjEnemy(CGCObjEnemy::EMovementAxis_UpDown, cocos2d::Vec2(900.0f, 100.0f), 500.0f, 0.0f, 30.0f, CGCObjEnemy::EEnemyIdentifier_Type1, rFactoryCreationParams_Type1, pAnimation1);
		pEnemy6->SetName("Derek7");


		
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupEnemy::DestroyEnemys()
{
	// this iterates the array of registered CGCObjects 
	// calling the supplied functor then deleting them

	DestroyObjectsReverseOrder([&](CGCObject* pObject)
		{
			// do nothing - DestroyObjectsReverseOrder calls delete!
			GCASSERT(GetGCTypeIDOf(CGCObjEnemy) == pObject->GetGCTypeID(), "wrong type!");

		});
}
