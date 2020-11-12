// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CMLCentralCavern.h"

#include "ManicMiner/AirManager/AirManager.h"
#include "ManicMiner/Collectible/CCollectible.h"
#include "ManicMiner/CollectiblesGroup/CCollectiblesGroup.h"
#include "ManicMiner/CollectiblesGroup/CentralCavern/CCGCentralCavern.h"
#include "ManicMiner/Doors/CentralCavern/CDCreatorCentralCavern.h"
#include "ManicMiner/Enemy/CentralCavern/GCObjGroupEnemyCentralCavern.h"
#include "ManicMiner/Hazards/CentralCavern/GCObjGroupHazardCentralCavern.h"
#include "ManicMiner/Helpers/Helpers.h"
#include "ManicMiner/HUD/CHUD.h"
#include "ManicMiner/Platforms/CentralCavern/CObjCCGroupPlatform.h"
#include "ManicMiner/Player/CPlayer.h"
#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/LevelManager/CLevelManager.h"


// Constructor -------------------------------------------------------------------------------------------------------- //
CMLCentralCavern::CMLCentralCavern( )
	: CManicLayer( )
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLCentralCavern::~CMLCentralCavern()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLCentralCavern::VOnCreate( void )
{
	// Set the inherited structs, it will be fed into the game manager
	m_sLevelValues = SLevelValues( ECollectibleRequirements::Collectible, 1 );
	
	// We set the background here, if m_pczBackGround is a nullptr, the background will be black.
	// It's important to call this before the parent's VOnCreate, otherwise the texture will never be loaded in.
	m_pczBackGround = static_cast<char*>("TexturePacker/Backgrounds/Placeholder/TemporaryBackground.plist");
	
	CManicLayer::VOnCreate();


	cocos2d::Size  visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	cocos2d::Vec2  v2NewStart( ( origin.x + ( visibleSize.width * 0.08f ) ), ( origin.y + ( visibleSize.height * 0.11f ) ) );

	// Edit Custom Layout
	GetPlayer().SetResetPosition(cocos2d::Vec2( 120.0f + 30.0f, 120.0f ) );

	
	m_pCHUD = new CHUD(*this );
		

	m_pcAirManager = new CAirManager(origin, visibleSize);
	m_pcAirManager->Init(*this);	
}

void CMLCentralCavern::InitParams()
{
	// Sets the references required by the player
	m_pcGameManager	->SetCHUD				( m_pCHUD		  );
	m_pcGameManager	->SetCPlayer			( &GetPlayer()	  );
	m_pcGameManager	->SetCAirManager		( m_pcAirManager  );
	m_pcAirManager	->SetGameManager		( m_pcGameManager );
	m_pcGameManager	->SetLevelRequirements	( m_sLevelValues  );
}


// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLCentralCavern::VOnDestroy(void)
{
	safeDelete( m_pCHUD );

	// Call base class last
	CManicLayer::VOnDestroy();
}