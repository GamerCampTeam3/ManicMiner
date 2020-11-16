// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLTheEndorianForest.h"

#include "ManicMiner/AirManager/AirManager.h"
#include "ManicMiner/Helpers/Helpers.h"
#include "ManicMiner/HUD/CHUD.h"
#include "ManicMiner/GameManager/CGameManager.h"



// Constructor -------------------------------------------------------------------------------------------------------- //
CMLTheEndorianForest::CMLTheEndorianForest()
	: CManicLayer()
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLTheEndorianForest::~CMLTheEndorianForest()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLTheEndorianForest::VOnCreate( void )
{
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues = SLevelValues( ECollectibleRequirements::Collectible, 5 );
	m_sLevelCreationParameters.pszLevelBackground = static_cast<char*>("TexturePacker/Backgrounds/Placeholder/TemporaryBackground.plist");
	m_sLevelCreationParameters.szLevelPath = "OgmoEditor/9_TheEndorianForest.oel";
	m_sLevelCreationParameters.szLevelName = "The Endorian Forest";

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();

	// Step 3:	New the CHUD and CAirManager.
	m_pCHUD = new CHUD( *this );
	m_pCHUD->UpdateLevelName( m_sLevelCreationParameters.szLevelName );

	m_pcAirManager = new CAirManager( m_pointOrigin, m_sizeVisible );
	m_pcAirManager->Init( *this );

	//auto emitter = cocos2d::ParticleExplosion::create();
	//emitter->setTotalParticles( 100 );
	//addChild( emitter, 10 );
}

void CMLTheEndorianForest::InitParams()
{
	// Step 4:  Set the references for the GameManager, this will be called by the LevelManager.
	m_pcGameManager->SetCHUD( m_pCHUD );
	m_pcGameManager->SetCPlayer( &GetPlayer() );
	m_pcGameManager->SetCAirManager( m_pcAirManager );
	m_pcAirManager->SetGameManager( m_pcGameManager );
	m_pcGameManager->SetLevelRequirements( m_sLevelCreationParameters.sLevelValues );
}


// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLTheEndorianForest::VOnDestroy( void )
{
	safeDelete( m_pCHUD );

	// Call base class last
	CManicLayer::VOnDestroy();
}