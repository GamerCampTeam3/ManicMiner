// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLProcessingPlant.h"

#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/AudioHelper/EBackgroundMusicNames.h"


// Constructor -------------------------------------------------------------------------------------------------------- //
CMLProcessingPlant::CMLProcessingPlant()
	: CManicLayer()
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLProcessingPlant::~CMLProcessingPlant()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLProcessingPlant::VOnCreate( void )
{
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible, 5 );
	m_sLevelCreationParameters.eParallaxTheme		= EParallaxTheme::Cavern;
	m_sLevelCreationParameters.eBackgroundMusic		= EBackgroundMusicName::CrystalCoralReef;
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/5_ProcessingPlant.oel";
	m_sLevelCreationParameters.szLevelName			= "Processing Plant";
	m_sLevelCreationParameters.v2PlayerStartPos		= CC_V2( 990.0f, 720.f);
	m_sLevelCreationParameters.bShouldFaceRight		= false;

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();
}



// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLProcessingPlant::VOnDestroy( void )
{
	// Call base class last
	CManicLayer::VOnDestroy();
}