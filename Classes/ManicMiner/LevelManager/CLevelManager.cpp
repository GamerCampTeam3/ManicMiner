#include "CLevelManager.h"

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

#include "cocos2d/cocos/base/CCDirector.h"

// Include Menu Scene
#include "MenuScene.h"

// Include ManicLayers
// Super Class
#include "ManicMiner/Layers/CManicLayer.h"

// Actual levels
#include "ManicMiner/Layers/CMLCentralCavern.h"
#include "ManicMiner/Layers/CMLTheColdRoom.h"

USING_NS_CC;


CLevelManager::CLevelManager( cocos2d::Director& rcDirector )
	: m_iCurrentLevelIndex( 0 )
{
	// Create CMenuLayer, assign a pointer to this CLevelManager instance
	Scene* pScene = CMenuLayer::scene( *this );

	// Run CMenuLayer
	rcDirector.runWithScene( pScene );
}

CLevelManager::~CLevelManager()
{}

void CLevelManager::Init()
{

}

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
	// Runs the template function CreateScene(), and assigns scene it to pScene
	switch( m_iCurrentLevelIndex )
	{
	case 0:
		// CENTRAL CAVERN
		pScene = TGCGameLayerSceneCreator< CMLCentralCavern >::CreateScene();
		break;
	case 1:
		// SECOND LEVEL
		pScene = TGCGameLayerSceneCreator< CMLTheColdRoom >::CreateScene();
		break;
	default:
		// BEAT FINAL LEVEL, GO BACK TO FIRST LEVEL
		// CENTRAL CAVERN
		pScene = TGCGameLayerSceneCreator< CMLCentralCavern >::CreateScene();
		break;
	}

	// If pScene is not nullptr, means a CManicLayer was created / we are not going into main menu
	if( pScene != nullptr )
	{
		// Get the new CManicLayer ( child of pScene, tag = 0 )
		auto newManicLayer = static_cast< CManicLayer* >( pScene->getChildByTag( 0 ) );

		// Assign its pointer to this CLevelManager
		newManicLayer->SetLevelManager( *this );

		// Begin transition
		cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionPageTurn::create( 1.0f, pScene, false ) );
	}


	// Increment Level Identifier Index
	m_iCurrentLevelIndex++;
}

void CLevelManager::GoToMainMenu()
{
	m_iCurrentLevelIndex = 0;

	// Create and run CMenuLayer
	Scene* pScene = CMenuLayer::scene( *this );
	cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionCrossFade::create( 1.0f, pScene ) );
}


void CLevelManager::EnterCavern()
{
	if( m_iCurrentLevelIndex == 0 )
	{
		GoToNextLevel();
	}
}

CManicLayer& CLevelManager::GetCurrentManicLayer()
{
	return *static_cast< CManicLayer* >( cocos2d::Director::getInstance()->getRunningScene()->getChildByTag( 0 ) );
}