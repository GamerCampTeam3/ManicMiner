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
#include "ManicMiner/Layers/CMLCentralCavern.h"
#include "ManicMiner/Layers/CMLTheColdRoom.h"

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
	if( m_iCurrentLevelIndex == 2 )
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
		pScene = TGCGameLayerSceneCreator< CMLCentralCavern >::CreateScene();

		break;
	case 1:
		// THE COLD ROOM
		pScene = TGCGameLayerSceneCreator< CMLTheColdRoom >::CreateScene();
		break;
	default:
		// Should not run
		int z = 0;	// BREAKPOINT something went wrong!
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

		// Initialize any info the game manager needs
		newManicLayer->InitParams();

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
