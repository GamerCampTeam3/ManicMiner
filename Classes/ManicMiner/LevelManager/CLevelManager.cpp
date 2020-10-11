#include "CLevelManager.h"

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/Layers/CTestLayer.h"

#include "MenuScene.h"
#include "../Layers/CMLCentralCavern.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "../GameInstance/CGameInstance.h"

USING_NS_CC;


CLevelManager::CLevelManager()
	: TSingleton()
	, m_iCurrentLevelIndex	( -1 )
	, m_pGameInstance		( nullptr )
	//,	m_pArrManicLayers	()
{}

CLevelManager::~CLevelManager()
{}

void CLevelManager::Init()
{
	CGameInstance* pGameInstance = CGameInstance::getInstance();
	if( pGameInstance != nullptr )
	{
		m_pGameInstance = pGameInstance;
	}
}

void CLevelManager::GoToMainMenu()
{
	m_pGameInstance->PlayerLeavingLevel( GetCurrentLevelLayer() );
	m_iCurrentLevelIndex = -1;
	Scene* pScene = CMenuLayer::scene();

	cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionRotoZoom::create( 1.0f, pScene ) );
}

void CLevelManager::GoToNextLevel()
{
	// If Not Coming Out Of MainMenu
	// Run PlayerLeavingLevel()
	if( m_iCurrentLevelIndex != -1 )
	{
		m_pGameInstance->PlayerLeavingLevel( GetCurrentLevelLayer() );
	}

	// Select next level
	switch( m_iCurrentLevelIndex )
	{
	case -1:
		cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionRotoZoom::create( 1.0f, TGCGameLayerSceneCreator< CMLCentralCavern >::CreateScene() ) );
		break;
	case 0:
		cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionRotoZoom::create( 1.0f, TGCGameLayerSceneCreator< CManicLayer >::CreateScene() ) );
		break;
	case 1:
		GoToMainMenu();
		break;
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

CManicLayer& CLevelManager::GetCurrentLevelLayer()
{
	// RIGHT APPROACH
	//int z = 0;
	//auto a1 = cocos2d::Director::getInstance();
	//auto a2 = a1->getRunningScene();
	//auto a3 = a2->getChildren();
	//auto *dynamic_cast< CManicLayer* >( cocos2d::Director::getInstance()->getRunningScene()->getChildByTag( 0 ) )
	//for( auto node : nodeVector )
	//{
	//	z++;
	//}
	//z = 99999;
	return *static_cast< CManicLayer* >( cocos2d::Director::getInstance()->getRunningScene()->getChildByTag( 0 ) );
}

void CLevelManager::UpdateLevelInfo()
{
	m_pGameInstance->SetPlayer( GetCurrentLevelLayer().GetPlayer() );
}