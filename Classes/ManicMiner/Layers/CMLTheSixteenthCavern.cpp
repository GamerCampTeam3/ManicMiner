// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLTheSixteenthCavern.h"

#include "ManicMiner/GameManager/CGameManager.h"


// Constructor -------------------------------------------------------------------------------------------------------- //
CMLTheSixteenthCavern::CMLTheSixteenthCavern()
	: CManicLayer()
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLTheSixteenthCavern::~CMLTheSixteenthCavern()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLTheSixteenthCavern::VOnCreate( void )
{
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible, 3 );
	m_sLevelCreationParameters.eParallaxTheme		= EParallaxTheme::Shipwreck;
	m_sLevelCreationParameters.eBackgroundMusic		= EBackgroundMusicName::CrystalCoralReef1;
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/15_TheSixteenthCavern.oel";
	m_sLevelCreationParameters.szLevelName			= "The Sixteenth Cavern";
	m_sLevelCreationParameters.v2PlayerStartPos		= CC_V2( 120.0f + 30.0f, 120.0f );
	m_sLevelCreationParameters.bShouldFaceRight		= true;

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();
}



// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLTheSixteenthCavern::VOnDestroy( void )
{
	// Call base class last
	CManicLayer::VOnDestroy();
}