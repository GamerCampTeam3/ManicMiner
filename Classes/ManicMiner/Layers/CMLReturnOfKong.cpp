// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLReturnOfKong.h"

#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/Doors/CMovingDoor.h"
#include "ManicMiner/Enemy/GCObjKong.h"
#include "ManicMiner/Enemy/GCObjEnemy.h"
#include "GamerCamp/GCObject/GCObject.h"
#include "ManicMiner/Platforms/CTriggerPlatform.h"

static CGCFactoryCreationParams s_cCreationParams_CGCObj_EKong("CGCObjEnemy_EKong", "TexturePacker/Sprites/Kong/Kong.plist", "cc_enemy_duck", b2_dynamicBody, true);
static CGCFactoryCreationParams s_cCreationParams_CMovingDoors( "CPlatform_Regular_1x1", "TexturePacker/Sprites/Platform/AbandonedUraniumWorkings/Brick/auw_platform_brick_60x60.plist", "CN_Platform_Regular_1x1", b2_staticBody, true );

// Constructor -------------------------------------------------------------------------------------------------------- //
CMLReturnOfKong::CMLReturnOfKong()
	: CManicLayer()
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLReturnOfKong::~CMLReturnOfKong()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLReturnOfKong::VOnCreate( void )
{
	// Step 1:  Set the values of the inherited protected LevelCreationParamaters.

	// SLevelValues:		Tells the CGameManager what is required for the level to be completed.
	// pszLevelBackground:	Sets the background for the level.
	// szLevelPath:			Sets the path where the level will be found.
	// It is important you initialize the values BEFORE CManicLayer::VOnCreate() is called -
	// Otherwise, it will use a bunch of default data (I have added checks for that) and nothing will load.	
	m_sLevelCreationParameters.sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible_And_Switches, 2, 5 );
	m_sLevelCreationParameters.eParallaxTheme		= EParallaxTheme::Cavern;
	m_sLevelCreationParameters.eBackgroundMusic		= EBackgroundMusicName::UnderwaterVolcano1;
	m_sLevelCreationParameters.szLevelPath			= "OgmoEditor/11_ReturnOfTheAlienKongBeast.oel";
	m_sLevelCreationParameters.szLevelName			= "Return of the Alien Kong Beast";
	m_sLevelCreationParameters.v2PlayerStartPos		= CC_V2( 120.0f + 30.0f, 120.0f );
	m_sLevelCreationParameters.bShouldFaceRight		= true;

	// Step 2:  Call CManicLayer VOnCreate to create everything the level requires e.g collisions, physics.
	CManicLayer::VOnCreate();

	m_pcKong = new CGCObjKong( cocos2d::Vec2( 960.0, 900.0 ), cocos2d::Vec2( 100.0, 360.0 ), 2.0f, s_cCreationParams_CGCObj_EKong );
	
	static const CGCFactoryCreationParams skcCMovingDoorParams( "CPlatform_Regular_1x1", "TexturePacker/Sprites/Platform/AbandonedUraniumWorkings/Brick/auw_platform_brick_60x60.plist", "CN_Platform_Regular_1x1", b2_staticBody, true );
	m_pcMovingDoor = new CMovingDoor( *m_pcGameManager, skcCMovingDoorParams );
}

void CMLReturnOfKong::VLevelSpecificInteraction()
{
	switch (m_pcGameManager->GetInteractionStage())
	{
		case ESpecialInteraction::Default:
			break;

		case ESpecialInteraction::Door:
			CGCObjEnemy* pcEnemy;
			CGCObject* pcBaseObject;
			pcBaseObject = CGCObjectManager::FindObject( "KongExtend", GetGCTypeIDOf( CGCObjEnemy ) );
			pcEnemy = static_cast<CGCObjEnemy*>(pcBaseObject);
			pcEnemy->ModifyEnemyDestinationPoint( cocos2d::Vec2( 1370.0f, 240.0f ) );
			m_pcMovingDoor->MoveBlocksAside();
			break;

		case ESpecialInteraction::Boss:
			CTriggerPlatform* pcTriggerPlatform;
			CGCObject* pcBasePlatform;
			pcBasePlatform = CGCObjectManager::FindObject( "TriggerPlatform", GetGCTypeIDOf( CTriggerPlatform ) );
			pcTriggerPlatform = static_cast<CTriggerPlatform*>(pcBasePlatform);
			pcTriggerPlatform->TriggerCrumble();

			m_pcKong->TriggerKongToFall();
			break;
	}
}


// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLReturnOfKong::VOnDestroy( void )
{
	// Call base class last
	CManicLayer::VOnDestroy();
}