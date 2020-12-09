// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLSolarPowerGenerator.h"

#include "ManicMiner/AudioHelper/EBackgroundMusicNames.h"
#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/SolarLight/SolarLight.h"

// Constructor -------------------------------------------------------------------------------------------------------- //
CMLSolarPowerGenerator::CMLSolarPowerGenerator()
	: CManicLayer()
	, m_pcSolarLight ( nullptr )
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLSolarPowerGenerator::~CMLSolarPowerGenerator()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLSolarPowerGenerator::VOnCreate( void )
{
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible, 3 );
	m_sLevelCreationParameters.eParallaxTheme		= EParallaxTheme::Magma;
	m_sLevelCreationParameters.eBackgroundMusic		= EBackgroundMusicName::UnderwaterVolcano2;
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/18_SolarPowerGenerator.oel";
	m_sLevelCreationParameters.szLevelName			= "Solar Power Generator";
	m_sLevelCreationParameters.v2PlayerStartPos		= CC_V2( 840.0f + 30.0f, 300.0f );
	m_sLevelCreationParameters.bShouldFaceRight		= true;

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();
}

// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLSolarPowerGenerator::VOnDestroy( void )
{
	// Call base class last
	CManicLayer::VOnDestroy();

	if( m_pcSolarLight )
	{
		delete m_pcSolarLight;
		m_pcSolarLight = nullptr;
	}
}

void CMLSolarPowerGenerator::VOnUpdate( f32 fTimeStep )
{
	CManicLayer::VOnUpdate( fTimeStep );

	m_pcSolarLight->Update();
}

void CMLSolarPowerGenerator::Init()
{
	CManicLayer::Init();
	m_pcSolarLight = new CSolarLight( *this );
}
