#include "AppDelegate.h"
#include <vector>
#include <string>

#include "AppMacros.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCObject/GCObjGroupDefault.h"
#include "SimpleAudioEngine.h"

#include "ManicMiner/LevelManager/CLevelManager.h"


USING_NS_CC;
using namespace std;

//////////////////////////////////////////////////////////////////////////
// GamerCamp Edit
// static 
CGCKeyboardManager*		AppDelegate::sm_pcKeyboardManager	= nullptr;
CGCControllerManager*	AppDelegate::sm_pcControllerManager = nullptr;
// GamerCamp Edit
//////////////////////////////////////////////////////////////////////////

AppDelegate::AppDelegate()
	:	m_pcLevelManager ( nullptr )
{
}


AppDelegate::~AppDelegate() 
{
//////////////////////////////////////////////////////////////////////////
// GamerCamp Edit
	// clean up win32 input 
	delete sm_pcKeyboardManager;
	sm_pcKeyboardManager = nullptr;

	delete sm_pcControllerManager;
	sm_pcControllerManager = nullptr;
// GamerCamp Edit
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Henrique Edit

	delete m_pcLevelManager;
	m_pcLevelManager = nullptr;

	// clean up singletons

///////////////////////////////////////////////////////////////////////////
// SimpleAudioEngine
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
	CocosDenshion::SimpleAudioEngine::end();
////////////////////////////////////////////////////////////////////////////
}


bool AppDelegate::applicationDidFinishLaunching() 
{
   // initialize director
	auto pDirector = Director::getInstance();
	auto glview = pDirector->getOpenGLView();
	
	if( !glview ) 
	{
		glview = GLViewImpl::create("Manic Miner Remake");
		pDirector->setOpenGLView(glview);
	}

	pDirector->setOpenGLView(glview);

    // Set the design resolution
	glview->setDesignResolutionSize( SM_CXSCREEN, SM_CYSCREEN, ResolutionPolicy::NO_BORDER );

	Size frameSize = glview->getFrameSize();
    
    vector<string> searchPath;

	//////////////////////////////////////////////////////////////////////////
	// GamerCamp Edit - add windows
	#if defined WIN32
		   searchPath.push_back( WindowsResource.directory );

		   pDirector->setContentScaleFactor( MIN( WindowsResource.size.height/designResolutionSize.height, WindowsResource.size.width/designResolutionSize.width ) );
	#else
	// GamerCamp Edit - add windows
	//////////////////////////////////////////////////////////////////////////

		// In this demo, we select resource according to the frame's height.
		// If the resource size is different from design resolution size, you need to set contentScaleFactor.
		// We use the ratio of resource's height to the height of design resolution,
		// this can make sure that the resource's height could fit for the height of design resolution.

		// if the frame's height is larger than the height of medium resource size, select large resource.
		if (frameSize.height > mediumResource.size.height)
		{
			searchPath.push_back(largeResource.directory);

			pDirector->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
		}
		// if the frame's height is larger than the height of small resource size, select medium resource.
		else if (frameSize.height > smallResource.size.height)
		{
			searchPath.push_back(mediumResource.directory);
        
			pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
		}
		// if the frame's height is smaller than the height of medium resource size, select small resource.
		else
		{
			searchPath.push_back(smallResource.directory);

			pDirector->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
		}

	//////////////////////////////////////////////////////////////////////////
	// GamerCamp Edit - add windows
	#endif //#if defined WIN32
	// GamerCamp Edit - add windows
	//////////////////////////////////////////////////////////////////////////
    
    // set searching path
    CCFileUtils::getInstance()->setSearchPaths( searchPath );
	
    // turn on display FPS
    //pDirector->setDisplayStats( true );

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0f / 60.0f);

	//////////////////////////////////////////////////////////////////////////
	// GamerCamp Edit

 		// create & init input
		sm_pcKeyboardManager	= new CGCKeyboardManager();

		sm_pcControllerManager	= new CGCControllerManager();
		InitialiseControllerManager();


	// GamerCamp Edit
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Henrique edit

		// Create Level Manager ( which will create the Main Menu Scene )
		m_pcLevelManager = new CLevelManager( *pDirector );
	
	// Henrique Edit
	//////////////////////////////////////////////////////////////////////////

    return true;
}


// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() 
{
    Director::getInstance()->stopAnimation();

	// Pause Audio
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}


// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() 
{
    Director::getInstance()->startAnimation();

    // Resume Audio
     CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
