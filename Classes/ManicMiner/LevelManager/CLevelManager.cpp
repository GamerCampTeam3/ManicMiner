// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CLevelManager.h"

// Include general cocos2d framework
#include "cocos2d/cocos/base/CCDirector.h"

// Include Helpers ( safe delete )
#include "ManicMiner/Helpers/Helpers.h"

// Include IGCGameLayer ( parent of CManicLayer )
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

// Include Menu Scene
#include "ManicMiner/Layers/MenuScene.h"

// Include CManicLayer ( level super class )
#include "ManicMiner/Layers/CManicLayer.h"

// Include CGameManager ( for game elements management )
#include "ManicMiner/GameManager/CGameManager.h"

// Include levels
// Sorted by level order
#include "ManicMiner/Layers/CMLCentralCavern.h"
#include "ManicMiner/Layers/CMLTheColdRoom.h"
#include "ManicMiner/Layers/CMLTheMenagerie.h"
#include "ManicMiner/Layers/CMLAbandonedUraniumWorkings.h"
#include "ManicMiner/Layers/CMLEugenesLair.h"
#include "ManicMiner/Layers/CMLProcessingPlant.h"
#include "ManicMiner/Layers/CMLTheVat.h"
#include "ManicMiner/Layers/CMLWillyMeetsKong.h"
#include "ManicMiner/Layers/CMLWackyAmoebatrons.h"
#include "ManicMiner/Layers/CMLTheEndorianForest.h"
#include "ManicMiner/Layers/CMLAttackOfTheMutant.h"
#include "ManicMiner/Layers/CMLReturnOfKong.h"
#include "ManicMiner/Layers/CMLOreRefinery.h"
#include "ManicMiner/Layers/CMLSkyLabLandingBay.h"
#include "ManicMiner/Layers/CMLTheBank.h"
#include "ManicMiner/Layers/CMLTheSixteenthCavern.h"
#include "ManicMiner/Layers/CMLTheWarehouse.h"
#include "ManicMiner/Layers/CMLAmoebatronsRevenge.h"
#include "ManicMiner/Layers/CMLSolarPowerGenerator.h"
#include "ManicMiner/Layers/CMLTheFinalBarrier.h"
#include "ManicMiner/Scenes/Controls/CControlsScene.h"
#include "ManicMiner/Scenes/GameOver/CGameOverScene.h"


USING_NS_CC;


// Constructor -------------------------------------------------------------------------------------------------------- //
CLevelManager::CLevelManager( cocos2d::Director& rcDirector )
	: m_iCurrentLevelIndex	( 0 )
	, m_pcCGameManager		( nullptr )
{
	// Create CMenuLayer, assign a pointer to this CLevelManager instance
	Scene* pScene = CMenuLayer::scene( *this );


	// Create the Game Manager, and feed it a pointer to *this
	m_pcCGameManager = new CGameManager(*this );


	// Run CMenuLayer as main scene
	rcDirector.runWithScene( pScene );
}

// Destructor  -------------------------------------------------------------------------------------------------------- //
CLevelManager::~CLevelManager()
{
	safeDelete( m_pcCGameManager );
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	GoToMainMenu																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CLevelManager::GoToMainMenu()
{
	// Reset level index to 0 
	m_iCurrentLevelIndex = 0;

	// Create and run CMenuLayer
	Scene* pScene = CMenuLayer::scene( *this );
	cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionCrossFade::create( 1.0f, pScene ) );
}

void CLevelManager::GoToControlsScene()
{
	// Reset level index to 0 
	m_iCurrentLevelIndex = 0;

	// Create and run CMenuLayer
	Scene* pScene = CControlsScene::scene( *this );
	cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionCrossFade::create( 1.0f, pScene ) );
}


void CLevelManager::GoToGameOverScene()
{
	// Reset level index to 0 
	m_iCurrentLevelIndex = 0;
	
	Scene* pScene = CGameOverScene::scene( *this );
	cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionCrossFade::create( 1.0f, pScene ) );	
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	GoToNextLevel																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CLevelManager::GoToNextLevel()
{
	// If beat last level, go to first level
	if( m_iCurrentLevelIndex == 20 )
	{
		m_iCurrentLevelIndex = 0;
	}

	// Prepare Scene to be created
	cocos2d::Scene* pScene = nullptr;

	// Select next level
	// Runs the template function CreateScene(), and assigns created scene to pScene
	switch( m_iCurrentLevelIndex )
	{
	case 0:
		// CENTRAL CAVERN 
		pScene = TGCGameLayerSceneCreator< CMLTheFinalBarrier >::CreateScene();
		break;
	case 1:
		// THE COLD ROOM
		pScene = TGCGameLayerSceneCreator< CMLTheColdRoom >::CreateScene();
		break;
	case 2:
		// THE MENAGERIE
		pScene = TGCGameLayerSceneCreator< CMLTheMenagerie >::CreateScene();
		break;
	case 3:
		// ABANDONED URANIUM WORKINGS
		pScene = TGCGameLayerSceneCreator< CMLAbandonedUraniumWorkings >::CreateScene();
		break;
	case 4:
		// EUGENES LAIR
		pScene = TGCGameLayerSceneCreator< CMLEugenesLair >::CreateScene();
		break;
	case 5:
		// PROCESSING PLANT
		pScene = TGCGameLayerSceneCreator< CMLProcessingPlant >::CreateScene();
		break;
	case 6:
		// THE VAT
		pScene = TGCGameLayerSceneCreator< CMLTheVat >::CreateScene();
		break;
	case 7:
		// WILLY MEETS KONG
		pScene = TGCGameLayerSceneCreator< CMLWillyMeetsKong >::CreateScene();
		break;
	case 8:
		// WACKY AMOEBATRONS
		pScene = TGCGameLayerSceneCreator< CMLWackyAmoebatrons >::CreateScene();
		break;
	case 9:
		// THE ENDORIAN FOREST
		pScene = TGCGameLayerSceneCreator< CMLTheEndorianForest >::CreateScene();
		break;
	case 10:
		// ATTACK OF THE MUTANT
		pScene = TGCGameLayerSceneCreator< CMLAttackOfTheMutant >::CreateScene();
		break;
	case 11:
		// RETURN OF KONG
		pScene = TGCGameLayerSceneCreator< CMLReturnOfKong >::CreateScene();
		break;
	case 12:
		// ORE REFINERY
		pScene = TGCGameLayerSceneCreator< CMLOreRefinery >::CreateScene();
		break;
	case 13:
		// SKY LAB LANDING BAY
		pScene = TGCGameLayerSceneCreator< CMLSkyLabLandingBay >::CreateScene();
		break;
	case 14:
		// THE BANK
		pScene = TGCGameLayerSceneCreator< CMLTheBank >::CreateScene();
		break;
	case 15:
		// THE SIXTEENTH CAVERN
		pScene = TGCGameLayerSceneCreator< CMLTheSixteenthCavern >::CreateScene();
		break;
	case 16:
		// THE WAREHOUSE
		pScene = TGCGameLayerSceneCreator< CMLTheWarehouse >::CreateScene();
		break;
	case 17:
		// AMOEBATRONS REVENGE
		pScene = TGCGameLayerSceneCreator< CMLAmoebatronsRevenge >::CreateScene();
		break;
	case 18:
		// SOLAR POWER GENERATOR
		pScene = TGCGameLayerSceneCreator< CMLSolarPowerGenerator >::CreateScene();
		break;
	case 19:
		// THE FINAL BARRIER
		pScene = TGCGameLayerSceneCreator< CMLTheFinalBarrier >::CreateScene();
		break;

		
	default:
		// Should not run
		CC_ASSERT( false );
		break;
	}

	// If pScene is not nullptr, means a CManicLayer was created
	if( pScene != nullptr )
	{
		// Get the new CManicLayer ( child of pScene, tag = 0 )
		auto newManicLayer = static_cast< CManicLayer* >( pScene->getChildByTag( 0 ) );

		// Assign its pointer to this CLevelManager
		newManicLayer->SetLevelManager( *this );

		// Assign Game Manager pointer to instantiated Game Manager on this class
		newManicLayer->SetGameManager( *m_pcCGameManager );

		// We initialize the values here
		newManicLayer->Init();

		
		// Begin transition
		cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionPageTurn::create( 1.0f, pScene, false ) );
	}

	// Increment Level Identifier Index
	m_iCurrentLevelIndex++;
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	EnterCavern																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CLevelManager::EnterCavern()
{
	// Make sure current level is Main Menu
	if( m_iCurrentLevelIndex == 0 )
	{
		// Resets the values, and will be set again by the next level.
		// Has to be called first otherwise Level values get overwritten and therefore set to 0.
		m_pcCGameManager->ResetValues();
		
		// If so, proceed to next level ( which will be level 1  / Central Cavern )
		GoToNextLevel();
	}
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	GetCurrentManicLayer																				//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	CManicLayer&																						//
//					reference to the currently active CManicLayer														//
// -------------------------------------------------------------------------------------------------------------------- //
CManicLayer& CLevelManager::GetCurrentManicLayer() const
{
	return *static_cast< CManicLayer* >( cocos2d::Director::getInstance()->getRunningScene()->getChildByTag( 0 ) );
}