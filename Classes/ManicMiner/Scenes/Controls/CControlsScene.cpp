#include "CControlsScene.h"

#include "ManicMiner/LevelManager/CLevelManager.h"

USING_NS_CC;


//////////////////////////////////////////////////////////////////////////
// scene
//////////////////////////////////////////////////////////////////////////
Scene* CControlsScene::scene( CLevelManager& rcLevelManager )
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    CControlsScene* layer = CControlsScene::create();

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
void CControlsScene::SetLevelManager( CLevelManager& rcLevelManager )
{
    m_pcLevelManager = &rcLevelManager;
}

//////////////////////////////////////////////////////////////////////////
// init
//////////////////////////////////////////////////////////////////////////
bool CControlsScene::init()
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

    m_bFullScreenSwitch = false;

    if (m_bFullScreenSwitch)
    {

        // Stretch to fullscreen
        static_cast<GLViewImpl*>(cocos2d::Director::getInstance()->getOpenGLView())->setFullscreen();
        // Set resolution
        Director::getInstance()->getOpenGLView()->setFrameSize( 1920, 1080 );
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize( 1920, 1080, ResolutionPolicy::EXACT_FIT );
        m_strPath = "Loose/to_windowed.png";
    }
    else
    {
        // Set resolution
        Director::getInstance()->getOpenGLView()->setFrameSize( 1920, 1080 );
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize( 1920, 1080, ResolutionPolicy::EXACT_FIT );
        m_strPath = "Loose/to_fullscreen.png";
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
	// Returns to main menu
	    MenuItemImage* pItemBackToMenu = MenuItemImage::create(
        "Loose/back_button_normal.png",
        "Loose/back_button_pressed.png",
        CC_CALLBACK_1( CControlsScene::CB_OnReturnToMenu, this ) );

    pItemBackToMenu->setPosition( Vec2( origin.x + (visibleSize.width * 0.21f),
        origin.y + (visibleSize.height * 0.22f) ) );

    // create menu, it's an autorelease object
    m_pMenu = Menu::create( pItemBackToMenu, nullptr );
    m_pMenu->setPosition( Vec2::ZERO );
    this->addChild( m_pMenu, 1 );


	
    CreateFullScreenButton();


    // add "HelloWorld" splash screen"
    Sprite* pSprite = Sprite::create( "Loose/help_menu.png" );

    // position the sprite on the center of the screen
    pSprite->setPosition( Vec2( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );

    // add the sprite as a child to this layer
    this->addChild( pSprite, 0 );

    return true;
}

void CControlsScene::CB_OnFullScreenButton( Ref* pSender )
{

    if (!m_bFullScreenSwitch)
    {
        dynamic_cast<GLViewImpl*>(cocos2d::Director::getInstance()->getOpenGLView())->setFullscreen();
        // Set resolution
        Director::getInstance()->getOpenGLView()->setFrameSize( 1920, 1080 );
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize( 1920, 1080, ResolutionPolicy::EXACT_FIT );
        m_bFullScreenSwitch = true;
        this->removeChild( this->getChildByName( "FSButton" ) );
        m_strPath = "Loose/to_windowed.png";
        CreateFullScreenButton();
    }

    else
    {
        // Set resolution
        dynamic_cast<GLViewImpl*>(cocos2d::Director::getInstance()->getOpenGLView())->setWindowed( 1920, 1080 );
        Director::getInstance()->getOpenGLView()->setFrameSize( 1920, 1080 );
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize( 1920, 1080, ResolutionPolicy::EXACT_FIT );
        m_bFullScreenSwitch = false;
        this->removeChild( this->getChildByName( "FSButton" ) );
        m_strPath = "Loose/to_fullscreen.png";
        CreateFullScreenButton();
    }
}


void CControlsScene::CreateFullScreenButton()
{
    MenuItemImage* pItemFullScreen = MenuItemImage::create(
        m_strPath,
        m_strPath,
        CC_CALLBACK_1( CControlsScene::CB_OnFullScreenButton, this ) );

    pItemFullScreen->setPosition( Vec2( 1820.f, 1060.f ) );
    m_pMenu = Menu::create( pItemFullScreen, nullptr );
    m_pMenu->setName( "FSButton" );
    m_pMenu->setPosition( Vec2::ZERO );
    this->addChild( m_pMenu, 1 );
}



//////////////////////////////////////////////////////////////////////////
//	CB_OnGameStartButton
//////////////////////////////////////////////////////////////////////////
void CControlsScene::CB_OnReturnToMenu( Ref* pSender )
{
    m_pcLevelManager->GoToMainMenu();
}
