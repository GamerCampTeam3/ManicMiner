#include "ManicMiner/AirManager/AirManager.h"
#include "ManicMiner/AudioHelper/ManicAudio.h"
#include "ManicMiner/Doors/CDoor.h"
#include "ManicMiner/Enums/ELifeUpdateType.h"
#include "ManicMiner/Helpers/Helpers.h"
#include "ManicMiner/HUD/CHUD.h"
#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/LevelManager/CLevelManager.h"
#include "ManicMiner/Player/CPlayer.h"

#include "CGameManager.h"
#include <fstream>


CGameManager::CGameManager( CLevelManager& rcLevelManager )
	: m_iCurrentScore			( 0 )
	, m_iHighScore				( 0 )
	, m_iCurrentLives			( m_kiStartingLives )
	, m_iCurrentCollectibles	( 0 )
	, m_iCurrentSwitches		( 0 )
	, m_iInteractionIndex		( 0 )
	, m_bDrainToScore			( false )
    , m_bDoOnce					( true )
	, m_pcAirManager			( nullptr )
	, m_pcCHUD					( nullptr )
	, m_pcLevelManager			( &rcLevelManager )
	, m_pcCPlayer				( nullptr )
	, m_pcMovingDoor			( nullptr )
	, m_sLevelValues			( ECollectibleRequirements::Collectible, 0, 0 )
	, m_ESpecialInteractionType ( ESpecialInteraction::Default )
	, m_eCurrentBackgroundMusic ( EBackgroundMusicName::CrystalCoralReef1 )
{
	ReadHighScore();
}


#pragma region Score_And_Life
// Increases the score
// Checks if score exceeds the high score and if so, update the high score
// Finally tell the CHUD to update it's text
void  CGameManager::IncreaseScore()
{
	m_iCurrentScore += m_kIScoreIncrement;	
	CheckHighScoreForUpdate();
	UpdateScore();
	ExtraLifeCheck( m_kIScoreIncrement );
}

void CGameManager::CheckHighScoreForUpdate()
{
	if (IsScoreGreaterThanHighscore() )
	{
		m_iHighScore = m_iCurrentScore;
		m_pcCHUD->UpdateHighScore( m_iHighScore );
		WriteHighScore();
	}
}

// Keeps tracks if the player has accrued enough score to get a new life
// If it has, the player will be given an extra life and then the counter will be reset.
void CGameManager::ExtraLifeCheck(int iScore)
{
	static int counter;
	counter += iScore;

	if ( counter >= m_kiExtraLifeThreshold )
	{
		UpdateLives( ELifeUpdateType::Plus );
		counter = counter - m_kiExtraLifeThreshold;
	}
}

// Checks if score exceeds high score.
bool  CGameManager::IsScoreGreaterThanHighscore() const
{
	return m_iCurrentScore >= m_iHighScore;
}

// Upon interacting, checks if enough has been reached.
bool CGameManager::CheckIfLevelRequirementsAreMet()
{
	bool bEnoughReached = false;

	switch ( m_sLevelValues.eCollectibleRequirements )
	{
		case ECollectibleRequirements::Collectible:
			bEnoughReached = (m_iCurrentCollectibles >= m_sLevelValues.iNumberofCollectibles);
			m_ESpecialInteractionType = ESpecialInteraction::Boss;
			break;

		case ECollectibleRequirements::Collectible_And_Switches:
			bEnoughReached =
					(m_iCurrentCollectibles >= m_sLevelValues.iNumberofCollectibles	) 
				&&	(m_iCurrentSwitches		>= m_sLevelValues.iNumberOfSwitches		) ;
			break;
	}
	
	return bEnoughReached;
}
#pragma endregion Score_And_Life

void CGameManager::OpenDoor()
{
	CDoor* pcDoor;
	CGCObject* pcBaseObject;
	
	pcBaseObject = CGCObjectManager::FindObject( "Door", GetGCTypeIDOf( CDoor ) );
	pcDoor = static_cast<CDoor*>(pcBaseObject);

	if (pcDoor != nullptr)
	{
		pcDoor->DoorOpen();
	}
}

#pragma region W/R_Highscore_Value

// This function is called when the score exceeds the current high score.
// It then opens the file, and stores the value.
// To avoid consumers from cheating and simply adding whatever high score they want,
// I have "encrypted" it by bit shifting the value and them multiplying it.
// Another value is created with a different bit shift to be used later.
void  CGameManager::WriteHighScore() const
{
	if (!USEBINARYMETHOD)
	{
		std::ofstream highScoreFile;
		highScoreFile.open( "Highscore.bin" );
		//ASSERT_CHECK( highScoreFile.is_open() );
		
		unsigned int tempScore = m_iHighScore;				// Sec the temporary int to be the current high score
		unsigned int comparisonScore = tempScore;			// We then also store it in another variable
		
		tempScore = tempScore << 16;						// We shift the bits here by 8
		comparisonScore = comparisonScore << 5;				// The second variable is shifted with a different number (so they aren't both the same)
		
		highScoreFile << tempScore;							// We then write the first value to the file
		highScoreFile << "\r\n";							// Next line
		highScoreFile << comparisonScore;					// Store the comparison value into the following line
	}

	else
	{
		std::ofstream file( "Data.bin", std::ios::binary );
		file.write( reinterpret_cast<const char*>(&m_iHighScore), sizeof( m_iHighScore ) );
		file.close();
	}
}


// This reads the high score stored in the "Highscore.bin"
// It will undo the bit shifts and multiplication to retrieve the original value
// It then compares the two decrypted value to check if any where altered
// And if so, punish the player by resetting it to 0
void  CGameManager::ReadHighScore()
{	
	if (!USEBINARYMETHOD)
	{
		std::ifstream highScoreFile;
		highScoreFile.open( "Highscore.bin" );
		//ASSERT_CHECK( highScoreFile.is_open());
		
		unsigned int tempScore = 0;
		unsigned int comparisonScore = 0;
		highScoreFile >> tempScore >> comparisonScore;			// We read the file and store the values
		
		// We then decrypt it with a division operation
		tempScore = tempScore >> 16;							// then shift 8 bits back
		comparisonScore = comparisonScore >> 5;					// Reverse the operation on comparison as well
		
		if ( tempScore != comparisonScore)						// Finally we compare the value to see if any where user touched.
		{
			m_iHighScore = 0;									// High score is reset to 0 if so
		}
		else
		{
			m_iHighScore = static_cast<int>(tempScore);							// Otherwise, it's all good!
		}
	}	
	else
	{
		std::ifstream file( "Data.bin", std::ios::in, std::ios::binary );
		file.read( reinterpret_cast<char*>(&m_iHighScore), sizeof( m_iHighScore ) );
		file.close();
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


void CGameManager::UpdateLives(ELifeUpdateType eLifeUpdateType)
{
		switch (eLifeUpdateType)
		{
			case ELifeUpdateType::Plus:
				++m_iCurrentLives;
				m_pcCPlayer->SetLives( m_iCurrentLives );
				m_pcCHUD->UpdateLives( ELifeUpdateType::Plus, m_iCurrentLives );
				break;

			case ELifeUpdateType::Minus:
				--m_iCurrentLives;
				m_pcCPlayer->SetLives( m_iCurrentLives );
				m_pcCHUD->UpdateLives( ELifeUpdateType::Minus, m_iCurrentLives );
				break;
		}
}


void CGameManager::ResetHUD()
{
	m_pcCHUD->FlushText();
}

void CGameManager::CheckShouldUpdateMusic( EBackgroundMusicName eBackgroundMusic )
{
	if ( m_eCurrentBackgroundMusic != eBackgroundMusic )
	{
		StopBackgroundMusic();
		m_eCurrentBackgroundMusic = eBackgroundMusic;
		PlayBackgroundMusic( m_eCurrentBackgroundMusic );
	}
}

#pragma endregion CHUD_Update_Calls

#pragma region Pointers_Setters

// Sets the current CHUD if valid
void CGameManager::SetCHUD(CHUD* pcCHUD)
{
	if (pcCHUD != nullptr)
	{
		m_pcCHUD = pcCHUD;
	}
}

void CGameManager::InitCHUD(std::string szLevelName)
{
	m_pcCHUD->Init( szLevelName, m_iCurrentLives, m_iCurrentScore, m_iHighScore );
}


// Sets the current CPlayer if valid
void CGameManager::SetCPlayer(CPlayer* pcPlayer)
{
	if (pcPlayer != nullptr)
	{
		m_pcCPlayer = pcPlayer;
		pcPlayer->SetLives( m_iCurrentLives );
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

// Sets the moving door
void CGameManager::SetMovingDoors( CMovingDoor* rcMovingDoor )
{
	m_pcMovingDoor = rcMovingDoor;
}

#pragma endregion Pointers_Setters

#pragma region Collectible/Switch Events
// Increases the score if a collectible, increments counter and checks if enough have been reached.

void CGameManager::CCollectibleInteractEvent()
{
	IncreaseScore();
	m_iCurrentCollectibles++;
	
	if (CheckIfLevelRequirementsAreMet() )
	{
		OpenDoor();
		m_pcLevelManager->GetCurrentManicLayer().SetGameState( EGameState::Escaping );
	}

	m_pcLevelManager->GetCurrentManicLayer().VLevelSpecificInteraction();
}

void CGameManager::CSwitchInteractEvent()
{
	m_iInteractionIndex++;	
	m_iCurrentSwitches++;
	
	if (CheckIfLevelRequirementsAreMet() )
	{
		m_pcLevelManager->GetCurrentManicLayer().SetGameState( EGameState::Escaping );
	}

	if (m_iInteractionIndex == 1)
	{
		m_ESpecialInteractionType = ESpecialInteraction::Door;
	}

	if (m_iInteractionIndex == 2)
	{
		m_ESpecialInteractionType = ESpecialInteraction::Boss;
		m_iInteractionIndex = 0;
	}
	
	m_pcLevelManager->GetCurrentManicLayer().VLevelSpecificInteraction();
}

#pragma endregion Collectible/Switch Events

#pragma region Level_Related_Calls
void CGameManager::SetLevelRequirements( const SLevelValues& rsLevelValues )
{
	m_sLevelValues = rsLevelValues;
}

void CGameManager::ResetEvent()
{
	m_iCurrentSwitches = 0;
	m_iCurrentCollectibles = 0;
	m_ESpecialInteractionType = ESpecialInteraction::Default;
	m_iInteractionIndex = 0;
}

void CGameManager::ResetValues()
{
	m_iCurrentCollectibles	= 0;
	m_iCurrentSwitches		= 0;
	m_iInteractionIndex		= 0;
	m_bDrainToScore			= false;
	m_sLevelValues			= SLevelValues( ECollectibleRequirements::Collectible, 0 );
}

void CGameManager::ResetLives()
{
	m_iCurrentLives = m_kiStartingLives;
	m_iCurrentScore = 0;
}

void CGameManager::EndLevel() 
{
	if ( m_pcLevelManager->GetCurrentManicLayer().GetGameState() == EGameState::Escaping )
	{
		DrainAirForScore();
		m_pcLevelManager->GetCurrentManicLayer().GetPlayer().OnEscape();
	}
}

void CGameManager::DrainAirForScore() 
{
	m_pcAirManager->DrainAir();
	m_pcCPlayer->SetVisible( false );
	m_pcCPlayer->SetCanBeControlled( false );
	m_pcCPlayer->ApplyDirectionChange( EPlayerDirection::Static );
	m_bDrainToScore = true;

}

void CGameManager::DrainToScore()
{
	if (m_pcAirManager->GetDrainComplete())
	{

		m_pcLevelManager->GetCurrentManicLayer().RequestNextLevel();
		m_pcCHUD->FlushText();

	}
	m_iCurrentScore += m_kiScorePerTimeLeft;
	UpdateScore();
	CheckHighScoreForUpdate();
	ExtraLifeCheck( m_kiScorePerTimeLeft );
}

#pragma endregion Level_Related_Calls

#pragma region Setters
void CGameManager::SetDoOnce(const bool bDoOnce)
{
	m_bDoOnce = bDoOnce;
}

// Sets the interaction type.
void CGameManager::SetInteractionStage( ESpecialInteraction eSpecialInteraction )
{
	m_ESpecialInteractionType = eSpecialInteraction;
}

#pragma endregion Setters


#pragma region Getters
// Gets the current interaction stage
ESpecialInteraction CGameManager::GetInteractionStage() const
{
	return m_ESpecialInteractionType;
}


bool CGameManager::CanLevelEnd()
{
	return CheckIfLevelRequirementsAreMet();
}

#pragma endregion Getters


CGameManager::~CGameManager()
{

}