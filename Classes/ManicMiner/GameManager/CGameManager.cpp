#include "CGameManager.h"

#include <fstream>

#include "ManicMiner/Helpers/Helpers.h"

#include "ManicMiner/AirManager/AirManager.h"
#include "ManicMiner/HUD/CHUD.h"
#include "ManicMiner/LevelManager/CLevelManager.h"
#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/Player/CPlayer.h"



CGameManager::CGameManager( CLevelManager& rcLevelManager )
	: m_iCurrentScore			( 0 )
	, m_iHighScore				( 0 )
	, m_iCurrentLives			( m_kiStartingLives )
	, m_iCurrentCollectibles	( 0 )
	, m_iRequiredCollectibles	( 0 )
	, m_iCurrentSwitches		( 0 )
	, m_iRequiredSwitches		( 0 )
	, m_bDrainToScore			( false )
	, m_pcAirManager			( nullptr )
	, m_pcCHUD					( nullptr )
	, m_pcLevelManager			( &rcLevelManager )
	, m_pcCPlayer				( nullptr )
	, m_sLevelValues			( ECollectibleRequirements::Collectible, 0, 0 )
{
	ReadHighScore();
}


#pragma region Score_And_Life
// Increases the score
// Checks if score exceeds the high score and if so, update the high score
// Finally tell the CHUD to update it's text
void  CGameManager::IncreaseScore()
{
	m_iCurrentScore += m_kiScoreIncrements;
	
	CheckHighScore();
	UpdateScore();
	ExtraLifeCheck();
}

void CGameManager::CheckHighScore()
{
	if (CheckScore())
	{
		m_iHighScore = m_iCurrentScore;
		m_pcCHUD->UpdateHighScore( m_iHighScore );
		WriteHighScore();
	}
}


// Keeps tracks if the player has accrued enough score to get a new life
// If it has, the player will be given an extra life and then the counter will be reset.
void CGameManager::ExtraLifeCheck() const
{
	static int counter;
	counter += m_kiScoreIncrements;

	if ( counter >= m_kiExtraLifeThreshold )
	{
		int extraLife = m_pcCPlayer->GetLives();
		m_pcCPlayer->SetLives( ++extraLife );
		counter = 0;
	}
}

// Checks if score exceeds high score.
bool  CGameManager::CheckScore() const
{
	return m_iCurrentScore > m_iHighScore;
}

// Upon interacting, checks if enough has been reached.
const bool CGameManager::CheckIfEnoughReached() const
{
	bool enoughReached = false;

	switch (m_sLevelValues.collectibleRequirements)
	{
		case ECollectibleRequirements::Collectible:
			enoughReached = (m_iCurrentCollectibles == m_iRequiredCollectibles);
			break;

		case ECollectibleRequirements::Collectible_And_Switches:
			enoughReached = ((m_iCurrentCollectibles == m_iRequiredCollectibles) && (m_iCurrentSwitches == m_iRequiredSwitches));
			break;
	}

	return enoughReached;
}
#pragma endregion Score_And_Life

#pragma region W/R_Highscore_Value

// This function is called when the score exceeds the current high score.
// It then opens the file, and stores the value.
// To avoid consumers from cheating and simply adding whatever high score they want,
// I have "encrypted" it by bit shifting the value and them multiplying it.
// Another value is created with a different bit shift to be used later.
void  CGameManager::WriteHighScore()
{
	// TODO: Look into creating binary file instead
	std::ofstream highScoreFile;
	highScoreFile.open( "Highscore.bin" );
	if (!highScoreFile.is_open())
	{
		m_iHighScore = 0;
		// TODO: Add a prompt saying file not found, therefore reset.
	}
	else
	{

		int tempScore = m_iHighScore;						// Sec the temporary int to be the current high score
		int comparisonScore = tempScore;					// We then also store it in another variable

		tempScore = tempScore << 8;							// We shift the bits here by 8
		tempScore = tempScore * 4;							// then we multiply it by 4, this is done so the high score isn't edited easily.

		comparisonScore = comparisonScore << 1;				// The second variable is shifted with a different number (so they aren't both the same)


		highScoreFile << tempScore;							// We then write the first value to the file
		highScoreFile << "\r\n";							// Next line
		highScoreFile << comparisonScore;					// Store the comparison value into the following line
	}
}


// This reads the high score stored in the "Highscore.bin"
// It will undo the bit shifts and multiplication to retrieve the original value
// It then compares the two decrypted value to check if any where altered
// And if so, punish the player by resetting it to 0
void  CGameManager::ReadHighScore()
{
	std::ifstream highScoreFile;
	highScoreFile.open( "Highscore.bin" );
	if (!highScoreFile.is_open())
	{
		m_iHighScore = 0;
		// TODO: Add a prompt saying file not found, therefore reset.
	}

	int tempScore = 0;					
	int comparisonScore = 0;
	highScoreFile >> tempScore >> comparisonScore;			// We read the file and store the values
	
	tempScore = tempScore / 4;								// We then decrypt it with a division operation
	tempScore = tempScore >> 8;								// then shift 8 bits back
	comparisonScore = comparisonScore >> 1;					// Reverse the operation on comparison as well

	if ( tempScore != comparisonScore)						// Finally we compare the value to see if any where user touched.
	{
		m_iHighScore = 0;									// High score is reset to 0 if so
	}
	else
	{
		m_iHighScore = tempScore;							// Otherwise, it's all good!
	}	
}

#pragma endregion W/R_Highscore_Value

#pragma region CHUD_Update_Calls
void CGameManager::UpdateScore() const
{
	m_pcCHUD->UpdateScore( m_iCurrentScore );
}


void CGameManager::UpdateHighScore() const
{
	m_pcCHUD->UpdateHighScore( m_iHighScore );
}


void CGameManager::UpdateLives() const
{
	m_pcCHUD->UpdateLives( m_pcCPlayer->GetLives() );
}
#pragma endregion CHUD_Update_Calls

#pragma region Pointers_Setters

// Sets the current CHUD if valid
void CGameManager::SetCHUD(CHUD* pcCHUD)
{
	if (pcCHUD != nullptr)
	{
		m_pcCHUD = pcCHUD;
		UpdateHighScore();
	}
}

// Sets the current CPlayer if valid
void CGameManager::SetCPlayer(CPlayer* pcPlayer)
{
	if (pcPlayer != nullptr)
	{
		m_pcCPlayer = pcPlayer;
		pcPlayer->SetLives( m_iCurrentLives );
		UpdateLives();
	}
}

// Sets the current CAirManager if valid
void CGameManager::SetCAirManager(CAirManager* pcAirManager)
{
	if (pcAirManager != nullptr )
	{
		m_pcAirManager = pcAirManager;
	}
}
#pragma endregion Pointers_Setters

#pragma region Collectible/Switch Events
// Increases the score if a collectible, increments counter and checks if enough have been reached.

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

#pragma endregion Collectible/Switch Events


void CGameManager::SetLevelRequirements( const SLevelValues& rsLevelValues )
{
	m_sLevelValues = rsLevelValues;

	switch (m_sLevelValues.collectibleRequirements)
	{
		case ECollectibleRequirements::Collectible:
			m_iRequiredCollectibles = m_sLevelValues.numCollectibles;
			break;
		
		case ECollectibleRequirements::Collectible_And_Switches:
			m_iRequiredSwitches = m_sLevelValues.numSwitches;
			break;
	}
}

void CGameManager::ResetValues()
{
	m_iCurrentCollectibles = 0;
	m_iCurrentSwitches = 0;
	m_iRequiredCollectibles = 0;
	m_iRequiredSwitches = 0;
	m_bDrainToScore = false;
}

void CGameManager::EndLevel() 
{
	if (m_pcLevelManager->GetCurrentManicLayer().GetGameState() == EGameState::Escaping)
	{
		DrainAirForScore();
	}
}

void CGameManager::DrainAirForScore() 
{
	m_pcAirManager->DrainAir();

	//float x = m_pcAirManager->fGetRemainingAirAmount() * 10.0f;
	//	
	//m_iCurrentScore = m_iCurrentScore * static_cast<int>(x);
							
																						
	
	if (m_pcAirManager->GetDrainComplete())
	{
		m_pcLevelManager->GetCurrentManicLayer().RequestNextLevel();
	}
	else
	{
		m_pcCPlayer->SetCanBeControlled( false );
		m_bDrainToScore = true;
	}
}

void CGameManager::DrainToScore()
{
	m_iCurrentScore += m_kiScoreIncrements;
	UpdateScore();
	CheckHighScore();
	ExtraLifeCheck();
	UpdateLives();
}


CGameManager::~CGameManager()
{

}