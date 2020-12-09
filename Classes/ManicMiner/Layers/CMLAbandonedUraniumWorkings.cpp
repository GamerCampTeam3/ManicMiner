#include "CMLAbandonedUraniumWorkings.h"

#include "ManicMiner/AudioHelper/EBackgroundMusicNames.h"

// Constructor -------------------------------------------------------------------------------------------------------- //
CMLAbandonedUraniumWorkings::CMLAbandonedUraniumWorkings()
	: CManicLayer()
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLAbandonedUraniumWorkings::~CMLAbandonedUraniumWorkings()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLAbandonedUraniumWorkings::VOnCreate( void )
{
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible, 5 );
	m_sLevelCreationParameters.eParallaxTheme		= EParallaxTheme::Shipwreck;
	m_sLevelCreationParameters.eBackgroundMusic		= EBackgroundMusicName::CrystalCoralReef4;
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/3_AbandonedUraniumWorkings.oel";
	m_sLevelCreationParameters.szLevelName			= "Abandoned Uranium Workings";
	m_sLevelCreationParameters.v2PlayerStartPos		= CC_V2 ( 1920.0f - 90.0f, 120.0f );
	m_sLevelCreationParameters.bShouldFaceRight		= false;

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();
}



// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLAbandonedUraniumWorkings::VOnDestroy( void )
{	
	// Call base class last
	CManicLayer::VOnDestroy();
}