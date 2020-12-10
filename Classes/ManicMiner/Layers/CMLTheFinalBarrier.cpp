// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLTheFinalBarrier.h"

#include "ManicMiner/GameManager/CGameManager.h"

// Constructor -------------------------------------------------------------------------------------------------------- //
CMLTheFinalBarrier::CMLTheFinalBarrier()
	: CManicLayer()
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLTheFinalBarrier::~CMLTheFinalBarrier()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLTheFinalBarrier::VOnCreate( void )
{
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible, 5 );
	m_sLevelCreationParameters.eParallaxTheme		= EParallaxTheme::Shipwreck;
	m_sLevelCreationParameters.eBackgroundMusic		= EBackgroundMusicName::Shipwreck3;
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/19_TheFinalBarrier.oel";
	m_sLevelCreationParameters.szLevelName			= "The Final Barrier";
	m_sLevelCreationParameters.v2PlayerStartPos		= CC_V2( 1680.0f + 30.0f, 120.0f );
	m_sLevelCreationParameters.bShouldFaceRight		= false;

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();
}

// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLTheFinalBarrier::VOnDestroy( void )
{
	// Call base class last
	CManicLayer::VOnDestroy();
}