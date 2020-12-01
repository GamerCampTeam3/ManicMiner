#include "CGameOverScene.h"


#include "ManicMiner/Helpers/Helpers.h"
#include "ManicMiner/LevelManager/CLevelManager.h"

#include <fstream>

USING_NS_CC;


//////////////////////////////////////////////////////////////////////////
// scene
//////////////////////////////////////////////////////////////////////////
Scene* CGameOverScene::scene( CLevelManager& rcLevelManager )
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    CGameOverScene* layer = CGameOverScene::create();

    // add layer as a child to scene
    scene->addChild( layer );

    // Set ptr to CLevelManager
    layer->SetLevelManager( rcLevelManager );

   
    // return the scene
    return scene;
}

//////////////////////////////////////////////////////////////////////////
// SetLevelManager
//////////////////////////////////////////////////////////////////////////
void CGameOverScene::SetLevelManager( CLevelManager& rcLevelManager )
{
    m_pcLevelManager = &rcLevelManager;
}

//////////////////////////////////////////////////////////////////////////
// init
//////////////////////////////////////////////////////////////////////////
bool CGameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Exit button --------------------------------------------------------------//
    MenuItemImage* pItemStartGame = MenuItemImage::create(
        "Loose/play_normal.png",
        "Loose/play_pressed.png",
        CC_CALLBACK_1( CGameOverScene::CB_OnGameStartButton, this ) );

    pItemStartGame->setPosition( Vec2( origin.x + (visibleSize.width * 0.61f),
        origin.y + (visibleSize.height * 0.52f) ) );

    Menu* pMenu = Menu::create( pItemStartGame, nullptr );
    pMenu->setPosition( Vec2::ZERO );
    this->addChild( pMenu, 1 );
    // --------------------------------------------------------------------------//

    // Background ---------------------------------------------------------------//
    Sprite* pBackgroundSprite = Sprite::create( "Loose/game_over.png" );

    // position the sprite on the center of the screen
    pBackgroundSprite->setPosition( Vec2( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );

    // add the sprite as a child to this layer
    this->addChild( pBackgroundSprite, 0 );
    // --------------------------------------------------------------------------//

    std::ifstream highScoreFile;
    highScoreFile.open( "Highscore.bin" );

    unsigned int tempScore = 0;
    highScoreFile >> tempScore;			

    // We then decrypt it with a division operation
    tempScore = tempScore >> 16;				

    int iHighscore = static_cast<int>(tempScore);


    Label* pHighscoreLabel = new cocos2d::Label();
    Label* pTextLabel = new cocos2d::Label();
	
    cocos2d::Vec2 v2HighscorePos = Vec2( origin.x + (visibleSize.width * 0.5f), origin.y + (visibleSize.height * 0.42f) );
    cocos2d::Vec2 v2TextPos = Vec2( origin.x + (visibleSize.width * 0.5f), origin.y + (visibleSize.height * 0.30f) );

    pHighscoreLabel->setTextColor( cocos2d::Color4B(255, 255, 255, 255) );	
    pHighscoreLabel->setTTFConfig( cocos2d::TTFConfig( "fonts/SMB2.ttf", 50.0f ) );	
    pHighscoreLabel->setGlobalZOrder( 3.f );
    pHighscoreLabel->setPosition( v2HighscorePos );
	    
    this->addChild( pHighscoreLabel, 2 );

    pHighscoreLabel->setString( "HIGHSCORE: " + std::to_string(iHighscore) );
    pHighscoreLabel->updateContent();

    pTextLabel->setTextColor( cocos2d::Color4B( 255, 255, 255, 255 ) );
    pTextLabel->setTTFConfig( cocos2d::TTFConfig( "fonts/SMB2.ttf", 30.0f ) );
    pTextLabel->setGlobalZOrder( 3.f );
    pTextLabel->setPosition( v2TextPos );

    this->addChild( pTextLabel, 2 );

    pTextLabel->setString( "Only " + std::to_string( iHighscore ) + "? Pathetic." );
    pTextLabel->updateContent();


	
    return true;
}


//////////////////////////////////////////////////////////////////////////
//	CB_OnGameStartButton
//////////////////////////////////////////////////////////////////////////
void CGameOverScene::CB_OnGameStartButton( Ref* pSender )
{
    m_pcLevelManager->GoToMainMenu();
}

//////////////////////////////////////////////////////////////////////////
//	CB_OnGameExitButton
//////////////////////////////////////////////////////////////////////////
void CGameOverScene::CB_OnGameExitButton( Ref* pSender )
{
    exit( 0 );
}