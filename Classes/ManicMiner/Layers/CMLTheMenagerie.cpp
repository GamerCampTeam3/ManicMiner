#include "CMLTheMenagerie.h"

#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/HUD/CHUD.h"
#include "ManicMiner/AirManager/AirManager.h"

// Constructor -------------------------------------------------------------------------------------------------------- //
CMLTheMenagerie::CMLTheMenagerie()
	: CManicLayer()
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLTheMenagerie::~CMLTheMenagerie()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLTheMenagerie::VOnCreate( void )
{

	// Set the inherited structs, it will be fed into the game manager
	m_sLevelValues = SLevelValues( ECollectibleRequirements::Collectible, 5 );

	// We set the background here, if m_pczBackGround is a nullptr, the background will be black.
	// It's important to call this before the parent's VOnCreate, otherwise the texture will never be loaded in.
	m_pczBackGround = static_cast<char*>("TexturePacker/Backgrounds/Placeholder/TemporaryBackground.plist");

	// Set the level path name here
	m_sLevelPath = "OgmoEditor/TheMenagerie.oel";

	// Run parent VOnCreate() which will handle all the rest of the setting up.
	CManicLayer::VOnCreate();

	// new the CHUD and AirManager (only classes that required to be  newed on the level)
	m_pCHUD = new CHUD( *this );
	m_pcAirManager = new CAirManager( m_pointOrigin, m_sizeVisible );
	m_pcAirManager->Init( *this );
}



void CMLTheMenagerie::InitParams()
{
	// Sets the references required by the player
	m_pcGameManager->SetCHUD( m_pCHUD );
	m_pcGameManager->SetCPlayer( &GetPlayer() );
	m_pcGameManager->SetCAirManager( m_pcAirManager );
	m_pcAirManager->SetGameManager( m_pcGameManager );
	m_pcGameManager->ResetValues();
	m_pcGameManager->SetLevelRequirements( m_sLevelValues );
}


// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLTheMenagerie::VOnDestroy( void )
{
	// Call base class last
	CManicLayer::VOnDestroy();
}