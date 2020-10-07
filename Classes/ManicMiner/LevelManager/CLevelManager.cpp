#include "CLevelManager.h"

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/Layers/CTestLayer.h"

#include "MenuScene.h"

USING_NS_CC;


CLevelManager::CLevelManager()
	:	TSingleton	(   )
	,	m_iCurrentLevelIndex		( 0 )
	,	m_pArrManicLayers			( )
{
	m_pArrManicLayers[0] = new CManicLayer;
}

CLevelManager::~CLevelManager()
{
}

void CLevelManager::Init()
{
	Scene* pScene = CMenuLayer::scene();
	// run
	Director::getInstance()->runWithScene( pScene );
}

void CLevelManager::GoToMainMenu()
{
	cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionRotoZoom::create( 1.0f, TGCGameLayerSceneCreator< CManicLayer >::CreateScene() ) );

	//cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionRotoZoom::create( 1.0f, TGCGameLayerSceneCreator< CMenuLayer >::CreateScene() ) );
	//m_pArrManicLayers[m_iCurrentLevelIndex].ReplaceScene( cocos2d::TransitionRotoZoom::create( 1.0f, CMenuLayer::scene() ) );

}

void CLevelManager::GoToNextLevel()
{
	cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionRotoZoom::create( 1.0f, TGCGameLayerSceneCreator< CManicLayer >::CreateScene() ) );
}

CManicLayer& CLevelManager::GetCurrentLevelLayer()
{
	return *m_pArrManicLayers[ m_iCurrentLevelIndex ];
}
