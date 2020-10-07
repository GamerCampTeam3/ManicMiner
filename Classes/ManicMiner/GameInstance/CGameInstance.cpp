#include "CGameInstance.h"

#include "GamerCamp/GameSpecific/Items/GCObjItem.h"
#include "GamerCamp/GameSpecific/Player/GCObjPlayer.h"
#include "GamerCamp/GameSpecific/Platforms/GCObjPlatform.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/LevelManager/CLevelManager.h"

CGameInstance::CGameInstance()
	: TSingleton			()
	, m_pPlayer				( nullptr )
	, m_pLevelManager		( nullptr )
	, m_iLivesLeft			( 3 )
	, m_iCollected			( 0 )
	, m_eGameState			( EGameState::EGS_Looting )
	, m_bResetWasRequested	( false )
{}


////////////////////////////////////////////////////////////////////////////////////////
// Init																				  //
////////////////////////////////////////////////////////////////////////////////////////
void CGameInstance::Init()
{
	// Create CLevelManager singleton, assign it to said pointer
	m_pLevelManager = CLevelManager::getInstance();
	
	// Run CLevelManager Init
	m_pLevelManager->Init();


}

EGameState CGameInstance::GetGameState()
{
	return m_eGameState;
}

void CGameInstance::SetGameState( EGameState gameState )
{
	m_eGameState = gameState;
}

void CGameInstance::ResetLevel()
{
	m_pLevelManager->GetCurrentLevelLayer().VOnReset();
	ResetRequestWasHandled();
}

void CGameInstance::OnFinishedLooting()
{
	SetGameState( EGameState::EGS_Escaping );
	m_pLevelManager->GetCurrentLevelLayer().GetCollisionManager().AddCollisionHandler( [&] ( CGCObjPlayer& rcPlayer, CGCObjPlatform& rItem, const b2Contact& rcContact ) -> void
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
}

void CGameInstance::OnItemCollected( CGCObjItem& rItem )
{
	CGCObjectManager::ObjectKill( &rItem );
	m_iCollected++;

	if( m_eGameState == EGameState::EGS_Looting && m_iCollected > 3 )
	{
		OnFinishedLooting();
	}
}

void CGameInstance::Update( f32 fTimeStep )
{
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
	CGCObjectManager::ObjectKill( m_pPlayer );
	// After death plays, pop up with retry UI
}
