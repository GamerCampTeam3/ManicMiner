#include "CLevelSelectScene.h"

#include "ManicMiner/LevelManager/CLevelManager.h"

USING_NS_CC;


//////////////////////////////////////////////////////////////////////////
// scene
//////////////////////////////////////////////////////////////////////////
Scene* CLevelSelectScene::scene( CLevelManager& rcLevelManager )
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    CLevelSelectScene* layer = CLevelSelectScene::create();

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
void CLevelSelectScene::SetLevelManager( CLevelManager& rcLevelManager )
{
    m_pcLevelManager = &rcLevelManager;
}

//////////////////////////////////////////////////////////////////////////
// init
//////////////////////////////////////////////////////////////////////////
bool CLevelSelectScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }

    // Bib Edit
    // This is a little boolean that allows for quick swapping from fullscreen to windowed
    // This is required to avoid flickering and alt-tabbing for debugging on Higher resolution screens

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    MenuItemImage* pItemBackToMenu = MenuItemImage::create(
        "Loose/back_button_normal.png",
        "Loose/back_button_pressed.png",
        CC_CALLBACK_1( CLevelSelectScene::CB_OnReturnToMenu, this ) );

    pItemBackToMenu->setPosition( Vec2( origin.x + (visibleSize.width * 0.11f),
        origin.y + (visibleSize.height * 0.12f) ) );

    // create menu, it's an autorelease object
    m_pMenu = Menu::create( pItemBackToMenu, nullptr );
    m_pMenu->setPosition( Vec2::ZERO );
    this->addChild( m_pMenu, 1 );

    cocos2d::Vec2 ButtonsPos[]
    {
        Vec2( origin.x + (visibleSize.width * 0.32f), origin.y + (visibleSize.height * 0.68f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.41f), origin.y + (visibleSize.height * 0.68f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.50f), origin.y + (visibleSize.height * 0.68f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.59f), origin.y + (visibleSize.height * 0.68f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.68f), origin.y + (visibleSize.height * 0.68f) ) ,
    	
        Vec2( origin.x + (visibleSize.width * 0.32f), origin.y + (visibleSize.height * 0.55f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.41f), origin.y + (visibleSize.height * 0.55f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.50f), origin.y + (visibleSize.height * 0.55f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.59f), origin.y + (visibleSize.height * 0.55f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.68f), origin.y + (visibleSize.height * 0.55f) ) ,
    	
        Vec2( origin.x + (visibleSize.width * 0.32f), origin.y + (visibleSize.height * 0.42f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.41f), origin.y + (visibleSize.height * 0.42f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.50f), origin.y + (visibleSize.height * 0.42f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.59f), origin.y + (visibleSize.height * 0.42f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.68f), origin.y + (visibleSize.height * 0.42f) ) ,
    	
        Vec2( origin.x + (visibleSize.width * 0.32f), origin.y + (visibleSize.height * 0.29f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.41f), origin.y + (visibleSize.height * 0.29f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.50f), origin.y + (visibleSize.height * 0.29f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.59f), origin.y + (visibleSize.height * 0.29f) ) ,
        Vec2( origin.x + (visibleSize.width * 0.68f), origin.y + (visibleSize.height * 0.29f) )
    	
    };

	
	
    for (int i = 0; i< 20; i++)
    {
        //int iTemp = ++;
        std::string strIndex = std::to_string( i + 1 );
        std::string strPath = "Menu/Buttons/Level/" + strIndex + "_LVL_Pressed.png";
    	
        MenuItemImage* pItemBackToMenu = MenuItemImage::create(
            strPath,
            strPath,
            CC_CALLBACK_1( CLevelSelectScene::CB_LoadLevel, this, i) );

        pItemBackToMenu->setPosition( ButtonsPos[i] );
        m_pMenu = Menu::create( pItemBackToMenu, nullptr );
        m_pMenu->setPosition( Vec2::ZERO );
        this->addChild( m_pMenu, 1 );

    }





















	

    // add "HelloWorld" splash screen"
    Sprite* pSprite = Sprite::create( "Menu/Backgrounds/level_selection.png" );

    // position the sprite on the center of the screen
    pSprite->setPosition( Vec2( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );

    // add the sprite as a child to this layer
    this->addChild( pSprite, 0 );

    return true;
}

void CLevelSelectScene::CB_LoadLevel(Ref* pSender, int i)
{
    static int i_array[20];
    i_array[i] = i;

    m_pcLevelManager->SelectLevel( i_array[i] );	
}


//////////////////////////////////////////////////////////////////////////
//	CB_OnGameStartButton
//////////////////////////////////////////////////////////////////////////
void CLevelSelectScene::CB_OnReturnToMenu( Ref* pSender )
{
    m_pcLevelManager->GoToMainMenu();
}
