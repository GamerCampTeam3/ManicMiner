// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLAmoebatronsRevenge.h"

#include "ManicMiner/GameManager/CGameManager.h"


// Constructor -------------------------------------------------------------------------------------------------------- //
CMLAmoebatronsRevenge::CMLAmoebatronsRevenge()
	: CManicLayer()
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLAmoebatronsRevenge::~CMLAmoebatronsRevenge()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLAmoebatronsRevenge::VOnCreate( void )
{
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible, 1 );
	m_sLevelCreationParameters.eParallaxTheme		= EParallaxTheme::Cavern;
	m_sLevelCreationParameters.eBackgroundMusic = EBackgroundMusicName::Shipwreck1;
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/17_Amoebatrons'Revenge.oel";
	m_sLevelCreationParameters.szLevelName			= "Amoebatrons' Revenge";
	m_sLevelCreationParameters.v2PlayerStartPos		= CC_V2( 1830.0f, 120.0f );
	m_sLevelCreationParameters.bShouldFaceRight		= false;

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();
}

// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLAmoebatronsRevenge::VOnDestroy( void )
{
	// Call base class last
	CManicLayer::VOnDestroy();
}