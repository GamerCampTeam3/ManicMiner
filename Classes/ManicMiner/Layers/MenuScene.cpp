#include "ManicMiner/Layers/MenuScene.h"

#include "ManicMiner/LevelManager/CLevelManager.h"

#include "ManicMiner/AudioHelper/ManicAudio.h"

USING_NS_CC;


//////////////////////////////////////////////////////////////////////////
// scene
//////////////////////////////////////////////////////////////////////////
Scene* CMenuLayer::scene( CLevelManager& rcLevelManager )
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    CMenuLayer *layer = CMenuLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

	// Set ptr to CLevelManager
	layer->SetLevelManager( rcLevelManager );

    // return the scene
    return scene;
}

//////////////////////////////////////////////////////////////////////////
// SetLevelManager
//////////////////////////////////////////////////////////////////////////
void CMenuLayer::SetLevelManager( CLevelManager& rcLevelManager )
{
	m_pcLevelManager = &rcLevelManager;
}

//////////////////////////////////////////////////////////////////////////
// init
//////////////////////////////////////////////////////////////////////////
bool CMenuLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    // Bib Edit
	// This is a little boolean that allows for quick swapping from fullscreen to windowed
	// This is required to avoid flickering and alt-tabbing for debugging on Higher resolution screens
    
    m_bFullScreenSwitch = false;
    std::string strIconPath;
    if (m_bFullScreenSwitch)
    {
      
        // Stretch to fullscreen
        static_cast< GLViewImpl* >( cocos2d::Director::getInstance()->getOpenGLView() )->setFullscreen();
        // Set resolution
        Director::getInstance()->getOpenGLView()->setFrameSize( 1920, 1080 );
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize( 1920, 1080, ResolutionPolicy::EXACT_FIT );
        strIconPath = "Loose/to_windowed.png";
    }
    else
    {
		// Set resolution
	Director::getInstance()->getOpenGLView()->setFrameSize( 1920, 1080 );
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize( 1920, 1080, ResolutionPolicy::EXACT_FIT );
        strIconPath = "Loose/to_fullscreen.png";
    }
	////////////////////////////////////////////////////////////////////////////////////////////////////
	
    Size visibleSize	= Director::getInstance()->getVisibleSize();
    Vec2 origin			= Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    MenuItemImage* pItemStartGame = MenuItemImage::create(
                                        "Loose/play_normal.png",
                                        "Loose/play_pressed.png",
                                        CC_CALLBACK_1( CMenuLayer::CB_OnGameStartButton, this ) );
        
    pItemStartGame->setPosition( Vec2(	origin.x + (visibleSize.width * 0.61f ),
										origin.y + (visibleSize.height * 0.52f ) ) );

    // create menu, it's an autorelease object
    Menu* pMenu = Menu::create(pItemStartGame, nullptr);
    pMenu->setPosition( Vec2::ZERO );
    this->addChild(pMenu, 1);

    ///////////////////////////////////////////////////////////////////////////
	/// - Umeer Rama

    MenuItemImage* pItemExitGame = MenuItemImage::create(
        "Buttons/Exit/ExitButton_01.png",
        "Buttons/Exit/ExitButton_01.png",
        CC_CALLBACK_1(CMenuLayer::CB_OnGameExitButton, this));

    pItemExitGame->setPosition(Vec2(1900.f, 1060.f));

    pMenu = Menu::create(pItemExitGame, nullptr);
    pMenu->setPosition(Vec2::ZERO);
    this->addChild(pMenu, 1);
    ///
    ///////////////////////////////////////////////////////////////////////////
    MenuItemImage* pItemFullScreen = MenuItemImage::create(
        strIconPath,
        strIconPath,
        CC_CALLBACK_1( CMenuLayer::CB_OnFullScreenButton, this ) );

    pItemFullScreen->setPosition( Vec2( 1820.f, 1060.f ) );
    pMenu = Menu::create( pItemFullScreen, nullptr );
    //pMenu->getChildByName( "FSButton" );
    pMenu->setPosition( Vec2::ZERO );
    this->addChild( pMenu, 1 );
    //////////////////////////////////////////////////////////
    

    // add "HelloWorld" splash screen"
    Sprite* pSprite = Sprite::create("Loose/main_menu.png");

    // position the sprite on the center of the screen
    pSprite->setPosition( Vec2( visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y ) );

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);

    return true;
}

void CMenuLayer::CB_OnFullScreenButton(Ref *pSender)
{

    if ( !m_bFullScreenSwitch )
    {
        static_cast<GLViewImpl*>(cocos2d::Director::getInstance()->getOpenGLView())->setFullscreen();
        // Set resolution
        Director::getInstance()->getOpenGLView()->setFrameSize( 1920, 1080 );
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize( 1920, 1080, ResolutionPolicy::EXACT_FIT );
        m_bFullScreenSwitch = true;
    }

    else
    {
        static_cast<GLViewImpl*>(cocos2d::Director::getInstance()->getOpenGLView())->setWindowed( 1920, 1080 );
        // Set resolution
        Director::getInstance()->getOpenGLView()->setFrameSize( 1920, 1080 );
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize( 1920, 1080, ResolutionPolicy::EXACT_FIT );
        m_bFullScreenSwitch = false;
    }
}


//////////////////////////////////////////////////////////////////////////
//	CB_OnGameStartButton
//////////////////////////////////////////////////////////////////////////
void CMenuLayer::CB_OnGameStartButton( Ref* pSender)
{
	// ---------------- Henrique edit ---------------- //

	// Preload the collectible sound
	PreloadSoundEffect( ESoundName::KeyCollected );

	// Play intense music theme
	//PlaySoundEffect( ESoundName::BabyShark );

	// Load the first level
	m_pcLevelManager->EnterCavern();

	// ----------------------------------------------- //
}

//////////////////////////////////////////////////////////////////////////
//	CB_OnGameExitButton
//////////////////////////////////////////////////////////////////////////
void CMenuLayer::CB_OnGameExitButton( Ref* pSender )
{
    exit(0);
}
