#include "CLevelManager.h"

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/Layers/CTestLayer.h"

#include "MenuScene.h"
#include "../Layers/CMLCentralCavern.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"

#include "cocos2d/cocos/base/CCDirector.h"
USING_NS_CC;


CLevelManager::CLevelManager( cocos2d::Director& rcDirector )
	: m_iCurrentLevelIndex	( -1 )
{
	// Create CMenuLayer, assign a pointer to this CLevelManager instance
	Scene* pScene = CMenuLayer::scene( *this );

	// Run
	rcDirector.runWithScene( pScene );
}

CLevelManager::~CLevelManager()
{}

void CLevelManager::Init()
{

}

void CLevelManager::GoToMainMenu()
{
	m_iCurrentLevelIndex = -1;
	Scene* pScene = CMenuLayer::scene( *this );

	cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionCrossFade::create( 1.0f, pScene ) );
}

void CLevelManager::GoToNextLevel()
{
	// Prepare Scene to be created
	cocos2d::Scene* pScene = nullptr;

	// Select next level
	// Runs the template function CreateScene(), and assigns scene it to pScene

	switch( m_iCurrentLevelIndex )
	{
	case -1:
		// CENTRAL CAVERN
		pScene = TGCGameLayerSceneCreator< CMLCentralCavern >::CreateScene();
		break;
	case 0:
		// SECOND LEVEL
		pScene = TGCGameLayerSceneCreator< CManicLayer >::CreateScene();
		break;
	case 1:
		// BEAT FINAL LEVEL, GO BACK TO FIRST LEVEL
		// but actually goes to menu
		GoToMainMenu();
		break;
	}

	// If pScene is not nullptr, means a CManicLayer was created / we are not going into main menu
	if ( pScene != nullptr )
	{
		// Get the new CManicLayer ( child of pScene, tag = 0 )
		auto newManicLayer = static_cast< CManicLayer* >( pScene->getChildByTag( 0 ) );
		
		// Assign its pointer to this CLevelManager
		newManicLayer->SetLevelManager( *this );
		
		// Begin transition
		cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionProgressOutIn::create( 1.0f, pScene ) );
	}


	// Increment Level Identifier Index
	m_iCurrentLevelIndex++;
}

void CLevelManager::EnterCavern()
{
	if ( m_iCurrentLevelIndex == -1 )
	{
		GoToNextLevel();
	}
}

CManicLayer& CLevelManager::GetCurrentManicLayer()
{
	return *static_cast< CManicLayer* >( cocos2d::Director::getInstance()->getRunningScene()->getChildByTag( 0 ) );
}