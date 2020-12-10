// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLOreRefinery.h"

#include "ManicMiner/GameManager/CGameManager.h"



// Constructor -------------------------------------------------------------------------------------------------------- //
CMLOreRefinery::CMLOreRefinery()
	: CManicLayer()
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLOreRefinery::~CMLOreRefinery()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLOreRefinery::VOnCreate( void )
{
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible, 5 );
	m_sLevelCreationParameters.eParallaxTheme		= EParallaxTheme::Magma;
	m_sLevelCreationParameters.eBackgroundMusic		= EBackgroundMusicName::UnderwaterVolcano2;
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/12_OreRefinery.oel";
	m_sLevelCreationParameters.szLevelName			= "Ore Refinery";
	m_sLevelCreationParameters.v2PlayerStartPos		= CC_V2( 1820.0f, 120.0f );
	m_sLevelCreationParameters.bShouldFaceRight		= true;

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();
}


// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLOreRefinery::VOnDestroy( void )
{
	// Call base class last
	CManicLayer::VOnDestroy();
}