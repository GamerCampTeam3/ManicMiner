////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MenuScene.h"

#include "ManicMiner/AudioHelper/ManicAudio.h"
#include "ManicMiner/LevelManager/CLevelManager.h"


USING_NS_CC;


//////////////////////////////////////////////////////////////////////////
//
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


void CMenuLayer::SetLevelManager( CLevelManager& rcLevelManager )
{
	m_pcLevelManager = &rcLevelManager;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
bool CMenuLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	// Quick little change to help me change between res
    const bool fullScreenSwitch = true;

    if ( fullScreenSwitch )
    {
        // Stretch to fullscreen
        static_cast< GLViewImpl* >( cocos2d::Director::getInstance()->getOpenGLView() )->setFullscreen();

        // Set resolution
        Director::getInstance()->getOpenGLView()->setFrameSize( 1920, 1080 );
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize( 1920, 1080, ResolutionPolicy::EXACT_FIT );
    }
    else
    {
		// Set resolution
		Director::getInstance()->getOpenGLView()->setFrameSize( 1920, 1080 );
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize( 1920, 1080, ResolutionPolicy::EXACT_FIT );
    }
	
    Size visibleSize	= Director::getInstance()->getVisibleSize();
    Vec2 origin			= Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    MenuItemImage* pItemStartGame = MenuItemImage::create(
                                        "Loose/CloseNormal.png",
                                        "Loose/CloseSelected.png",
                                        CC_CALLBACK_1( CMenuLayer::CB_OnGameStartButton, this ) );
        
    pItemStartGame->setPosition( Vec2(	origin.x + (visibleSize.width * 0.5f ),
										origin.y + (visibleSize.height * 0.5f ) ) );

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

    /////////////////////////////
    // 3. add your codes below...

    // add "HelloWorld" splash screen"
    Sprite* pSprite = Sprite::create("Loose/HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition( Vec2( visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y ) );

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);

    return true;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CMenuLayer::CB_OnGameStartButton( Ref* pSender)
{
	//PreloadSoundEffect( ESoundName::ESN_KeyCollected );
	//PlaySoundEffect( ESoundName::ESN_KeyCollected );
	m_pcLevelManager->EnterCavern();
}

void CMenuLayer::CB_OnGameExitButton(Ref* pSender)
{
    // add code to release anything that needs to be released before exiting the game

    exit(0);
}