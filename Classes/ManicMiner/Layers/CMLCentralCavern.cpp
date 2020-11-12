// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLCentralCavern.h"

#include "ManicMiner/AirManager/AirManager.h"
#include "ManicMiner/Helpers/Helpers.h"
#include "ManicMiner/HUD/CHUD.h"
#include "ManicMiner/GameManager/CGameManager.h"



// Constructor -------------------------------------------------------------------------------------------------------- //
CMLCentralCavern::CMLCentralCavern( )
	: CManicLayer( )
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLCentralCavern::~CMLCentralCavern()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLCentralCavern::VOnCreate( void )
{
	// Set the inherited structs, it will be fed into the game manager
	m_sLevelValues = SLevelValues( ECollectibleRequirements::Collectible, 5);
	
	// We set the background here, if m_pczBackGround is a nullptr, the background will be black.
	// It's important to call this before the parent's VOnCreate, otherwise the texture will never be loaded in.
	m_pczBackGround = static_cast<char*>("TexturePacker/Backgrounds/Placeholder/TemporaryBackground.plist");

	// Set the level path name here
	m_sLevelPath = "OgmoEditor/CentralCavern.oel";
	
	CManicLayer::VOnCreate();
	
	m_pCHUD = new CHUD(*this );	
	m_pcAirManager = new CAirManager(m_pointOrigin, m_sizeVisible);
	m_pcAirManager->Init(*this);	
}

void CMLCentralCavern::InitParams()
{
	// Sets the references required by the GameManager
	m_pcGameManager	->SetCHUD				( m_pCHUD		  );
	m_pcGameManager	->SetCPlayer			( &GetPlayer()	  );
	m_pcGameManager	->SetCAirManager		( m_pcAirManager  );
	m_pcAirManager	->SetGameManager		( m_pcGameManager );
	m_pcGameManager	->SetLevelRequirements	( m_sLevelValues  );
}


// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLCentralCavern::VOnDestroy(void)
{
	safeDelete( m_pCHUD );

	// Call base class last
	CManicLayer::VOnDestroy();
}