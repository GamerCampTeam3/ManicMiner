// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLEugenesLair.h"

#include "ManicMiner/AirManager/AirManager.h"
#include "ManicMiner/Helpers/Helpers.h"
#include "ManicMiner/HUD/CHUD.h"
#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/Enemy/GCObjEugene.h"


// Constructor -------------------------------------------------------------------------------------------------------- //
CMLEugenesLair::CMLEugenesLair()
	: CManicLayer()

{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLEugenesLair::~CMLEugenesLair()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLEugenesLair::VOnCreate( void )
{
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible, 5 );
	m_sLevelCreationParameters.eParallaxTheme		= EParallaxTheme::Cavern;
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/4_Eugene'sLair.oel";
	m_sLevelCreationParameters.szLevelName			= "Eugene's Lair";
	m_sLevelCreationParameters.v2PlayerStartPos		= CC_V2( 60.0f + 30.0f, 720.0f );
	m_sLevelCreationParameters.bShouldFaceRight		= true;

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();
}

void CMLEugenesLair::VLevelSpecificInteraction()
{
	switch (m_pcGameManager->GetCanProceed())
	{
		case ESpecialInteraction::Default:
			break;

		case ESpecialInteraction::Door:
			break;

		case ESpecialInteraction::Boss:
			///////////////////////////////////////////////
			// Find Eugene in the object list 
			CGCObjEugene* pcEugene;
			CGCObject* pcBaseObject;
			pcBaseObject = CGCObjectManager::FindObject( "Eugene", GetGCTypeIDOf( CGCObjEugene ) );
			pcEugene = static_cast<CGCObjEugene*>(pcBaseObject);

			// This operation can now be call when required, eg. all collectibles have been collected on this level.
			pcEugene->TriggerEugenesAlternativeAnimation();
			m_pcGameManager->SetCanProceed( ESpecialInteraction::Default );
			break;
	}
}

// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLEugenesLair::VOnDestroy( void )
{

	// Call base class last
	CManicLayer::VOnDestroy();
}