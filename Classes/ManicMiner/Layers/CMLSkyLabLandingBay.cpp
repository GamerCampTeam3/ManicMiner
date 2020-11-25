// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLSkyLabLandingBay.h"

#include "ManicMiner/AirManager/AirManager.h"
#include "ManicMiner/Helpers/Helpers.h"
#include "ManicMiner/HUD/CHUD.h"
#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/Enemy/SkylabLandingBay/GCObjLander.h"


static CGCFactoryCreationParams s_cCreationParams_CGCObj_ELander("CGCObjEnemy_EDuck", "TexturePacker/Sprites/Duck/Duck.plist", "cc_enemy_duck", b2_dynamicBody, true);

// Constructor -------------------------------------------------------------------------------------------------------- //
CMLSkyLabLandingBay::CMLSkyLabLandingBay()
	: CManicLayer()
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLSkyLabLandingBay::~CMLSkyLabLandingBay()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLSkyLabLandingBay::VOnCreate( void )
{
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible, 4 );
	m_sLevelCreationParameters.eParallaxTheme		= EParallaxTheme::Cavern;
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/13_SkyLabLandingBay.oel";
	m_sLevelCreationParameters.szLevelName			= "Sky Lab Landing Bay";
	m_sLevelCreationParameters.v2PlayerStartPos		= CC_V2( 1820.0f, 120.0f );
	m_sLevelCreationParameters.bShouldFaceRight		= false;

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();

	// Step 3:	New the CHUD and CAirManager.
	m_pCHUD = new CHUD(*this, m_pointOrigin, m_sizeVisible );
	//m_pCHUD->UpdateLevelName( m_sLevelCreationParameters.szLevelName );

	m_pcAirManager = new CAirManager( m_pointOrigin, m_sizeVisible );
	m_pcAirManager->Init( *this );

	//auto emitter = cocos2d::ParticleExplosion::create();
	//emitter->setTotalParticles( 100 );
	//addChild( emitter, 10 );


	CGCObjLander* CLander1 = new CGCObjLander(cocos2d::Vec2(100.0,900.0), cocos2d::Vec2(100.0, 360.0), 2.0f, 3.0f, 1.0f, s_cCreationParams_CGCObj_ELander);
	
	CGCObjLander* CLander2 = new CGCObjLander(cocos2d::Vec2(240.0, 900.0), cocos2d::Vec2(240.0, 660.0), 1.0f, 0.0f, 1.0f, s_cCreationParams_CGCObj_ELander);
	
	CGCObjLander* CLander3 = new CGCObjLander(cocos2d::Vec2(360.0, 900.0), cocos2d::Vec2(360.0, 480.0), 3.0f, 3.0f, 1.0f, s_cCreationParams_CGCObj_ELander);
	
	CGCObjLander* CLander5 = new CGCObjLander(cocos2d::Vec2(590.0, 900.0), cocos2d::Vec2(590.0, 360.0), 7.0f, 1.0f, 1.0f, s_cCreationParams_CGCObj_ELander);

	CGCObjLander* CLander6 = new CGCObjLander(cocos2d::Vec2(730.0, 900.0), cocos2d::Vec2(730.0, 650.0), 3.0f, 4.0f, 1.0f, s_cCreationParams_CGCObj_ELander);

	CGCObjLander* CLander7 = new CGCObjLander(cocos2d::Vec2(840.0, 900.0), cocos2d::Vec2(840.0, 480.0), 8.0f, 2.0f, 1.0f, s_cCreationParams_CGCObj_ELander);

	CGCObjLander* CLander9 = new CGCObjLander(cocos2d::Vec2(1090.0, 900.0), cocos2d::Vec2(1090.0, 360.0), 7.0f, 0.0f, 1.0f, s_cCreationParams_CGCObj_ELander);

	CGCObjLander* CLander10 = new CGCObjLander(cocos2d::Vec2(1190.0, 900.0), cocos2d::Vec2(1190.0, 650.0), 7.0f, 0.0f, 1.0f, s_cCreationParams_CGCObj_ELander);

	CGCObjLander* CLander11 = new CGCObjLander(cocos2d::Vec2(1320.0, 900.0), cocos2d::Vec2(1320.0, 480.0), 1.0f, 3.0f, 1.0f, s_cCreationParams_CGCObj_ELander);

	CGCObjLander* CLander13 = new CGCObjLander(cocos2d::Vec2(1560.0, 900.0), cocos2d::Vec2(1560.0, 360.0), 2.0f, 2.0f, 1.0f, s_cCreationParams_CGCObj_ELander);

	CGCObjLander* CLander14 = new CGCObjLander(cocos2d::Vec2(1670.0, 900.0), cocos2d::Vec2(1670.0, 650.0), 4.0f, 1.0f, 1.0f, s_cCreationParams_CGCObj_ELander);

	CGCObjLander* CLander15 = new CGCObjLander(cocos2d::Vec2(1810.0, 900.0), cocos2d::Vec2(1810.0, 480.0), 1.0f, 0.0f, 1.0f, s_cCreationParams_CGCObj_ELander);

}


void CMLSkyLabLandingBay::InitParams()
{
	// Step 4:  Set the references for the GameManager, this will be called by the LevelManager.
	m_pcGameManager->SetCHUD( m_pCHUD );
	m_pcGameManager->SetCPlayer( &GetPlayer() );
	m_pcGameManager->SetCAirManager( m_pcAirManager );
	m_pcAirManager->SetGameManager( m_pcGameManager );
	m_pcGameManager->SetLevelRequirements( m_sLevelCreationParameters.sLevelValues );
	m_pcGameManager->InitCHUD( m_sLevelCreationParameters.szLevelName );
}


// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLSkyLabLandingBay::VOnDestroy( void )
{
	safeDelete( m_pCHUD );

	// Call base class last
	CManicLayer::VOnDestroy();
}