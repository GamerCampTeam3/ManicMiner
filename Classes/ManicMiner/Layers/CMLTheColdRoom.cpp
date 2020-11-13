// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
#include "CMLTheColdRoom.h"

#include "ManicMiner/Hazards/CentralCavern/GCObjGroupHazardCentralCavern.h"
#include "ManicMiner/Platforms/CentralCavern/CObjCCGroupPlatform.h"
#include "ManicMiner/Player/CPlayer.h"
#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/HUD/CHUD.h"
#include "ManicMiner/AirManager/AirManager.h"

// Constructor -------------------------------------------------------------------------------------------------------- //
CMLTheColdRoom::CMLTheColdRoom()
	: CManicLayer()
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLTheColdRoom::~CMLTheColdRoom()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLTheColdRoom::VOnCreate( void )
{
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible, 5 );
	m_sLevelCreationParameters.pszLevelBackground	= static_cast<char*>("TexturePacker/Backgrounds/Placeholder/TemporaryBackground.plist");
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/TheColdRoom.oel";

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();

	// Step 3:	New the CHUD and CAirManager.
	m_pCHUD = new CHUD( *this );
	m_pcAirManager = new CAirManager( m_pointOrigin, m_sizeVisible );
	m_pcAirManager->Init( *this );
}



void CMLTheColdRoom::InitParams()
{
	// Sets the references required by the player
	m_pcGameManager	->SetCHUD				( m_pCHUD			);
	m_pcGameManager	->SetCPlayer			( &GetPlayer()		);
	m_pcGameManager	->SetCAirManager		( m_pcAirManager	);
	m_pcAirManager	->SetGameManager		( m_pcGameManager	);
	m_pcGameManager	->ResetValues();
	m_pcGameManager	->SetLevelRequirements	( m_sLevelCreationParameters.sLevelValues );
}


// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLTheColdRoom::VOnDestroy( void )
{
	// Call base class last
	CManicLayer::VOnDestroy();
}