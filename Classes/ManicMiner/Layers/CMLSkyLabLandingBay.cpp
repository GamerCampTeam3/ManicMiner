// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLSkyLabLandingBay.h"

#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/Enemy/SkylabLandingBay/GCObjLander.h"


static CGCFactoryCreationParams s_cCreationParams_CGCObj_ELander("CGCObjEnemy_EDuck", "TexturePacker/Sprites/Duck/Duck.plist", "cc_enemy_duck", b2_dynamicBody, true);

// Constructor -------------------------------------------------------------------------------------------------------- //
CMLSkyLabLandingBay::CMLSkyLabLandingBay()
	: CManicLayer()
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLSkyLabLandingBay::~CMLSkyLabLandingBay()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLSkyLabLandingBay::VOnCreate( void )
{
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible, 4 );
	m_sLevelCreationParameters.eParallaxTheme		= EParallaxTheme::Cavern;
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/13_SkyLabLandingBay.oel";
	m_sLevelCreationParameters.szLevelName			= "Sky Lab Landing Bay";
	m_sLevelCreationParameters.v2PlayerStartPos		= CC_V2( 1820.0f, 120.0f );
	m_sLevelCreationParameters.bShouldFaceRight		= false;

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();

	const float fPTM = 60.0f;

	// All data replicated from the following clip:
	// https://youtu.be/BgUzteADsRI?t=890
	// There are only 3 enemies in this level at any given frame
	// Once a lander explodes, it will redeploy on its next defined position
	// A single lander will always explode on a certain Y coordinate, it just redeploys in different X coordinates
	// Conveniently this level starts on 14:50
	// The White and Yellow Landers land for the first time on 14:52,
	// Whereas the Cyan Lander lands on 14:53
	// Therefore the time they take for their first collision is 2s ( White & Yellow ) and 3s ( Cyan )
	// This also means that White and Yellow always land at the same time, no matter their current spawn

	// Time for Collision / Start Delay / Redeploy Delay
	CGCObjLander* CLanderWhite = new CGCObjLander( { 2, 10, 18, 26 }, 4, 2.0f, s_cCreationParams_CGCObj_ELander );

	CGCObjLander* CLanderCyan = new CGCObjLander( { 12, 20, 28, 4 }, 9, 3.0f, s_cCreationParams_CGCObj_ELander );

	CGCObjLander* CLanderYellow = new CGCObjLander( { 22, 30, 6, 14 }, 6, 2.0f, s_cCreationParams_CGCObj_ELander );

}


// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLSkyLabLandingBay::VOnDestroy( void )
{
	// Call base class last
	CManicLayer::VOnDestroy();
}