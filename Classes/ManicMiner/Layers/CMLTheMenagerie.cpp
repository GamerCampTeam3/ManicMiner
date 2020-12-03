#include "CMLTheMenagerie.h"

#include "ManicMiner/GameManager/CGameManager.h"

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
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible, 5 );
	m_sLevelCreationParameters.eParallaxTheme		= EParallaxTheme::Toxic;
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/2_TheMenagerie.oel";
	m_sLevelCreationParameters.szLevelName			= "The Menagerie";
	m_sLevelCreationParameters.v2PlayerStartPos		= CC_V2( 120.0f + 30.0f, 120.0f );
	m_sLevelCreationParameters.bShouldFaceRight		= true;
	m_sLevelCreationParameters.pszPlatformBackground = "TexturePacker/Backgrounds/Toxic/ToxicPlatforms.plist";
	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();
}


// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLTheMenagerie::VOnDestroy( void )
{
	// Call base class last
	CManicLayer::VOnDestroy();
}