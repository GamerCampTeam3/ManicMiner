// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLWillyMeetsKong.h"

#include "ManicMiner/AirManager/AirManager.h"
#include "ManicMiner/Helpers/Helpers.h"
#include "ManicMiner/HUD/CHUD.h"
#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/Enemy/GCObjEnemy.h"
#include "ManicMiner/Enemy/GCObjKong.h"


static CGCFactoryCreationParams s_cCreationParams_CGCObj_EKong("CGCObjEnemy_EDuck", "TexturePacker/Sprites/Duck/Duck.plist", "cc_enemy_duck", b2_dynamicBody, true);


// Constructor -------------------------------------------------------------------------------------------------------- //
CMLWillyMeetsKong::CMLWillyMeetsKong()
	: CManicLayer()
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLWillyMeetsKong::~CMLWillyMeetsKong()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLWillyMeetsKong::VOnCreate( void )
{
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible_And_Switches, 2, 5 );
	m_sLevelCreationParameters.eParallaxTheme		= EParallaxTheme::Cavern;
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/7_MinerWillyMeetsTheKongBeast.oel";
	m_sLevelCreationParameters.szLevelName			= " Miner Willy meets the Kong Beast";
	m_sLevelCreationParameters.v2PlayerStartPos		= CC_V2( 120.0f + 30.0f, 120.0f );
	m_sLevelCreationParameters.bShouldFaceRight		= true;

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();

	// Step 3:	New the CHUD and CAirManager.
	m_pCHUD = new CHUD(*this, m_pointOrigin, m_sizeVisible );
	//m_pCHUD->UpdateLevelName( m_sLevelCreationParameters.szLevelName );

	m_pcAirManager = new CAirManager( m_pointOrigin, m_sizeVisible );
	m_pcAirManager->Init( *this );

	//auto emitter = cocos2d::ParticleExplosion::create();
	//emitter->setTotalParticles( 100 );
	//addChild( emitter, 10 );


	   	  


    //  Add Kong boss character.

	CGCObjKong* CKong = new CGCObjKong(cocos2d::Vec2(960.0, 900.0), cocos2d::Vec2(100.0, 360.0), 2.0f, s_cCreationParams_CGCObj_EKong);


	/////////////////////////////////////////////
    // Find Extendable enemy in the object list. 
	CGCObjEnemy* pcEnemy;
	CGCObject* pcBaseObject;
	pcBaseObject = CGCObjectManager::FindObject("KongExtend", GetGCTypeIDOf(CGCObjEnemy));
	pcEnemy = static_cast<CGCObjEnemy*>(pcBaseObject);


	// This operation can be called when the enemy movement is required to be extended (wall removed), and the correct enemy will have its movement window extended.
	//pcEnemy->ModifyEnemyDestinationPoint(cocos2d::Vec2(1150.0f,240.0f));


	CKong->TriggerKongToFall();
	


}

void CMLWillyMeetsKong::VInitParams()
{
	// Step 4:  Set the references for the GameManager, this will be called by the LevelManager.
	m_pcGameManager->SetCHUD( m_pCHUD );
	m_pcGameManager->SetCPlayer( &GetPlayer() );
	m_pcGameManager->SetCAirManager( m_pcAirManager );
	m_pcAirManager->SetGameManager( m_pcGameManager );
	m_pcGameManager->SetLevelRequirements( m_sLevelCreationParameters.sLevelValues );
	m_pcGameManager->InitCHUD( m_sLevelCreationParameters.szLevelName );
}







// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLWillyMeetsKong::VOnDestroy( void )
{
	safeDelete( m_pCHUD );

	// Call base class last
	CManicLayer::VOnDestroy();
}