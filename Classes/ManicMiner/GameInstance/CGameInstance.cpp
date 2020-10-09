#include "CGameInstance.h"

#include "GamerCamp/GameSpecific/Items/GCObjItem.h"
#include "GamerCamp/GameSpecific/Player/GCObjPlayer.h"
#include "GamerCamp/GameSpecific/Platforms/GCObjPlatform.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/LevelManager/CLevelManager.h"
#include "../AirManager/AirManager.h"

CGameInstance::CGameInstance()
	: TSingleton			()
	, m_pPlayer				( nullptr )
	, m_pLevelManager		( nullptr )
	, m_pAirManager			( nullptr )
	, m_iMaxLives			( 3 )
	, m_iLivesLeft			( m_iMaxLives )
	, m_iCollected			( 0 )
	, m_eGameState			( EGameState::EGS_Looting )
	, m_bResetWasRequested	( false )
{
}


////////////////////////////////////////////////////////////////////////////////////////
// Init																				  //
////////////////////////////////////////////////////////////////////////////////////////
void CGameInstance::Init()
{
	// Create CLevelManager singleton, assign it to said pointer
	m_pLevelManager = CLevelManager::getInstance();
	
	// Run CLevelManager Init
	m_pLevelManager->Init();

	//m_pPlayer = &m_pLevelManager->GetCurrentLevelLayer().GetPlayer();
}

EGameState CGameInstance::GetGameState()
{
	return m_eGameState;
}

void CGameInstance::SetGameState( EGameState gameState )
{
	m_eGameState = gameState;
}

void CGameInstance::SetPlayer( CGCObjPlayer& rPlayer )
{
	m_pPlayer = &rPlayer;
}

void CGameInstance::ResetLevel()
{
	m_pLevelManager->GetCurrentLevelLayer().VOnReset();
	m_iCollected = 0;
	m_pAirManager->Reset();
	ResetRequestWasHandled();
}

void CGameInstance::PlayerEnteredNewLevel( CManicLayer& rNewManicLayer, CGCObjPlayer& rPlayer )
{
	if( m_pAirManager == nullptr )
	{
		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();
		m_pAirManager = new CAirManager(  origin, visibleSize );
	}

	m_iCollected = 0;
	m_pPlayer = &rPlayer;
	m_eGameState = EGameState::EGS_Looting;


	m_pAirManager->Init( rNewManicLayer );
}

void CGameInstance::OutOfAir()
{
	OnPlayerDeath( m_pLevelManager->GetCurrentLevelLayer().GetPlayer() );
}

void CGameInstance::OnFinishedLooting()
{
	SetGameState( EGameState::EGS_Escaping );
	m_pLevelManager->GetCurrentLevelLayer().GetCollisionManager().AddCollisionHandler( [&] ( CGCObjPlayer& rcPlayer, CGCObjPlatform& rPlatform, const b2Contact& rcContact ) -> void
		{
			if( m_eGameState == EGameState::EGS_Escaping )
			{
				OnEscaped();
			}
		} );
}

void CGameInstance::OnEscaped()
{
	SetGameState( EGameState::EGS_Victory );
	// Run Time animation and points and stuff
	// when that ends, call this line:
	m_pLevelManager->GoToNextLevel();
}

void CGameInstance::OnItemCollected( CGCObjItem& rItem )
{
	if( !rItem.GetHasBeenCollected() )
	{
		rItem.Collected();
		CGCObjectManager::ObjectKill( &rItem );
		m_iCollected++;

		if( m_eGameState == EGameState::EGS_Looting && m_iCollected > 3 )
		{
			OnFinishedLooting();
		}
	}
}

void CGameInstance::EnterCavern()
{
	m_iLivesLeft = m_iMaxLives;
	m_iCollected = 0;
	m_pLevelManager->EnterCavern();
}

void CGameInstance::PlayerLeavingLevel( CManicLayer& rNewManicLayer )
{
	m_pAirManager->LeavingLevel(rNewManicLayer);
}

void CGameInstance::Update( f32 fTimeStep )
{
	// AIR MANAGER UPDATE
	if( m_pAirManager )
	{
		m_pAirManager->Update( fTimeStep );
	}

	// RESET LEVEL REQUEST
	if( ResetWasRequested() )
	{
		ResetLevel();
	}
}

void CGameInstance::OnPlayerDeath( CGCObjPlayer& rPlayer )
{
	m_iLivesLeft--;
	if( m_iLivesLeft )
	{
		RequestReset();
	}
	else
	{
		OutOfLives();
	}
}

void CGameInstance::OutOfLives()
{
	// Death animation instead of the objkill

	// After death plays, pop up with retry UI
	m_pLevelManager->GoToMainMenu();
}
