#include "CGameManager.h"

#include <fstream>

#include "ManicMiner/Helpers/Helpers.h"
#include "ManicMiner/HUD/CHUD.h"
#include "ManicMiner/LevelManager/CLevelManager.h"
#include "ManicMiner/Layers/CManicLayer.h"


CGameManager::CGameManager( CLevelManager& LevelManager )
	: m_iCurrentScore			( 0 )
	, m_iHighScore				( 0 )
	, m_iScoreIncrement			( 0 ) 
	, m_iCurrentLives			( 0 )
	, m_iCurrentCollectibles	( 0 )
	, m_iRequiredCollectibles	( 0 )
	, m_iCurrentSwitches		( 0 )
	, m_iRequiredSwitches		( 0 )
	, m_pcCHUD					( nullptr )
	, m_pcLevelManager			( &LevelManager )
	, m_eTypeRequired			( ECollectibleTypeRequired::Collectible )
{
	ReadHighScore();
}


void  CGameManager::IncreaseScore()
{
	if (CheckScore())
	{
		m_iHighScore = m_iCurrentScore;
		m_pcCHUD->UpdateHighScore( m_iHighScore );
	}
	
	m_iCurrentScore += m_iScoreIncrement;
	m_pcCHUD->UpdateScore( m_iCurrentScore );
}

int  CGameManager::GetCurrentLives()
{
	return m_iCurrentLives;
}

void  CGameManager::WriteHighScore()
{
	std::ofstream highScoreFile;
	highScoreFile.open( "Highscore.bin" );
	if (!highScoreFile.is_open())
	{
		// Error if unable to open
	}

	// Writes the highscore to the file, to be loaded in on opening.
	highScoreFile << m_iHighScore;
}

void CGameManager::UpdateLives()
{
	m_pcCHUD->UpdateLives( m_iCurrentLives );
}

void CGameManager::SetLives(int lives)
{
	m_iCurrentLives = lives;
}

void CGameManager::ResetValues()
{
	m_iCurrentCollectibles	= 0;
	m_iCurrentSwitches		= 0;
	m_iRequiredCollectibles = 0;
	m_iRequiredSwitches		= 0;
}

void  CGameManager::ReadHighScore()
{
	std::ifstream highScoreFile;
	highScoreFile.open( "Highscore.bin" );
	if (!highScoreFile.is_open())
	{
		// Error if unable to open
	}

	// Read the highscore file and stores it in m_iHighScore;
	highScoreFile >> m_iHighScore;
}

int  CGameManager::GetHighScore()
{
	return m_iHighScore;
}

bool  CGameManager::CheckScore()
{
	return m_iCurrentScore > m_iHighScore;
}

int CGameManager::GetScore()
{
	return m_iCurrentScore;
}

void CGameManager::SetCHUD(CHUD* cCHUD)
{
	if (cCHUD != nullptr)
	{
		m_pcCHUD = cCHUD;
	}
}


void CGameManager::CCollectibleInteractEvent()
{
	IncreaseScore();
	m_iCurrentCollectibles++;
	if (CheckIfEnoughReached() )
	{
		m_pcLevelManager->GetCurrentManicLayer().SetGameState( EGameState::Escaping );
	}
}

void CGameManager::CSwitchInteractEvent()
{
	m_iCurrentSwitches++;
	if (CheckIfEnoughReached())
	{
		m_pcLevelManager->GetCurrentManicLayer().SetGameState( EGameState::Escaping );
	}
}


bool CGameManager::CheckIfEnoughReached()
{
	switch (m_eTypeRequired)
	{
		case ECollectibleTypeRequired::Collectible:
			return m_iCurrentCollectibles == m_iRequiredCollectibles;
			break;

		case ECollectibleTypeRequired::Switch:
			return (true);
			break;

		case ECollectibleTypeRequired::Both:
			return ((m_iCurrentCollectibles == m_iRequiredCollectibles) && (m_iCurrentSwitches == m_iRequiredSwitches));
			break;
	}
}

void CGameManager::InitLevelEndRequirements(ECollectibleTypeRequired eType, int collectibles, int switches)
{
	m_eTypeRequired = eType;

	switch (m_eTypeRequired)
	{
		case ECollectibleTypeRequired::Collectible:
			m_iRequiredCollectibles = collectibles;
			break;

		case ECollectibleTypeRequired::Switch:
			break;

		case ECollectibleTypeRequired::Both:
			m_iRequiredCollectibles = collectibles;
			m_iRequiredSwitches = switches;
			break;
	}
}


CGameManager::~CGameManager()
{
	safeDelete( m_pcCHUD );
}