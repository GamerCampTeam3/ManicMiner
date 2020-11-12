// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
#include "CMLTheColdRoom.h"

#include "ManicMiner/Hazards/CentralCavern/GCObjGroupHazardCentralCavern.h"
#include "ManicMiner/Platforms/CentralCavern/CObjCCGroupPlatform.h"
#include "ManicMiner/Player/CPlayer.h"
#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/HUD/CHUD.h"
#include "ManicMiner/AirManager/AirManager.h"

// Constructor -------------------------------------------------------------------------------------------------------- //
CMLTheColdRoom::CMLTheColdRoom()
	: CManicLayer()
{}

// Destructor --------------------------------------------------------------------------------------------------------- //
CMLTheColdRoom::~CMLTheColdRoom()
{}

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate - Define unique layout ----------------------------------------------------------------------------------- //
void CMLTheColdRoom::VOnCreate( void )
{

	// Set the inherited structs, it will be fed into the game manager
	m_sLevelValues = SLevelValues( ECollectibleRequirements::Collectible, 1 );

	// We set the background here, if m_pczBackGround is a nullptr, the background will be black.
	// It's important to call this before the parent's VOnCreate, otherwise the texture will never be loaded in.
	m_pczBackGround = static_cast<char*>("TexturePacker/Backgrounds/Placeholder/TemporaryBackground.plist");

	CManicLayer::VOnCreate();


	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	cocos2d::Vec2	v2NewStart( ( origin.x + ( visibleSize.width * 0.5f ) ), ( origin.y + ( visibleSize.height * 0.33f ) ) );


	// Edit Custom Layout
	GetPlayer().SetResetPosition( v2NewStart );
	
	m_pCHUD = new CHUD( *this );


	m_pcAirManager = new CAirManager( origin, visibleSize );
	m_pcAirManager->Init( *this );
	
}

// VOnDestroy - Cleanup unique layout --------------------------------------------------------------------------------- //
void CMLTheColdRoom::VOnDestroy( void )
{

	// Call base class last
	CManicLayer::VOnDestroy();
}

void CMLTheColdRoom::InitParams()
{
	// Sets the references required by the player
	m_pcGameManager->SetCHUD( m_pCHUD );
	m_pcGameManager->SetCPlayer( &GetPlayer() );
	m_pcGameManager->SetCAirManager( m_pcAirManager );
	m_pcAirManager->SetGameManager( m_pcGameManager );
	m_pcGameManager->ResetValues();
	m_pcGameManager->SetLevelRequirements( m_sLevelValues );
}
