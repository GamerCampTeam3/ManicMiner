// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLTheEndorianForest.h"

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
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible, 5 );
	m_sLevelCreationParameters.eParallaxTheme		= EParallaxTheme::Toxic;
	m_sLevelCreationParameters.eBackgroundMusic		= EBackgroundMusicName::Shipwreck2;
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/9_TheEndorianForest.oel";
	m_sLevelCreationParameters.szLevelName			= "The Endorian Forest";
	m_sLevelCreationParameters.v2PlayerStartPos		= CC_V2( 90.0f, 660.0f );
	m_sLevelCreationParameters.bShouldFaceRight		= true;

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();
}

// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLTheEndorianForest::VOnDestroy( void )
{
	// Call base class last
	CManicLayer::VOnDestroy();
}