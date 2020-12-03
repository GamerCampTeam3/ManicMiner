//////////////////
/// Bib
//////////////////
#ifndef _CGAMEMANAGER_H_
#define _CGAMEMANAGER_H_



#include "ManicMiner/Structs/SLevelValues.h"

#include <string>

	// To use binary file method, set this to true
	// Otherwise to use the manual bit shifting encryption, set false
	const bool bUseBinaryFile = true;

	#ifndef USEBINARYMETHOD
		#define USEBINARYMETHOD bUseBinaryFile
	#endif


	//-------------------------------------------------------- FWD CLS DECLARATION -------------------------------------------------------------------------------------//	
	class		CAirManager;																																					
	class		CPlayer;																																						
	class		CHUD;																																							
	class		CLevelManager;
	class		CMovingDoor;
	enum class	ELifeUpdateType;//
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
	
	// An enum that is used for the level that have special interactions in them, for instance when 1 switch is flipped, or both.
	enum class ESpecialInteraction
	{
		Default = 0,
		Door = 1,
		Boss
	};
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------//

class CGameManager
{
																	 public:
																		 
	//------------------------------------------------------------  CTOR/DTOR ------------------------------------------------------------------------------------------			
	CGameManager(CLevelManager& rcLevelManager );																																	
	~CGameManager();																																								
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------			
	//																																												
	//------------------------------------------------------------  FUNCTIONS ------------------------------------------------------------------------------------------			
	//-----------------------------------------------------------  On Interact  ----------------------------------------------------------------------------------------			
	void	CCollectibleInteractEvent();									// Updates the elements upon collecting with a collectible.												
	void	CSwitchInteractEvent();											// Updates the elements upon interacting with a cswitch.
	void	ResetInteractionCounter() { m_iInteractionIndex = 0; }		// Resets the interaction counter
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------			
	//																																												
	//-----------------------------------------------------------  Level Related  --------------------------------------------------------------------------------------			
	void	SetLevelRequirements( const SLevelValues& levelValues );		// Called by each individual level with its requirements.												
	void	ResetValues();													// Resets the values required for level management.														
	void	EndLevel();														// Checks if the level can end.																			
	void	DrainToScore();													// Called by the air manager, drains air to score. 														
	void	ResetHUD();														// Called by the CManicLayer when the level ends.														
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------			
	//																																												
	//-----------------------------------------------------------  Player Related  -------------------------------------------------------------------------------------												
	void	UpdateLives( ELifeUpdateType eLifeUpdateType )	  ;				// Tells the CHUD to update the lives with the current lives.
	void	ResetLives();													// Resets the lives.
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------			
	//																																												
	//--------------------------------------------------------------  Setters  -----------------------------------------------------------------------------------------			
	void	SetCPlayer			( CPlayer* pcPlayer );						// Sets the pointer to the current CPlayer.																
	void	SetCHUD				( CHUD* pcCHUD );							// Sets the pointer to the current CHUD.																
	void	SetCAirManager		( CAirManager* pcAirManager );				// Sets the pointer to the current AirManager.															
	void	InitCHUD			( std::string szLevelName );				// Sets the name of the level.
	void	SetDoOnce			( const bool bDoOnce );						// Sets bDoOnce.
	void	SetMovingDoors		( CMovingDoor* rcMovingDoor );				// Sets the reference to the CMovingDoor
	void	SetInteractionStage	( ESpecialInteraction eSpecialInteraction );// Sets the interaction enum.
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------			
	//																																												
	//--------------------------------------------------------------  Getters  -----------------------------------------------------------------------------------------			
	bool	GetCanDrainToScore() const	{ return m_bDrainToScore;	}		// Returns bDrainsToScore
	bool	GetDoOnce() const			{ return m_bDoOnce;			}		// Returns bDoOnce
	ESpecialInteraction GetInteractionStage() const;						// Returns the ESpecialInteraction
	bool	CanLevelEnd();
	int		GetScore() const { return m_iCurrentScore; }
	int		GetHighscore() const { return m_iHighScore; }
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------			
	void ResetEvent() { m_iCurrentSwitches = 0; m_iCurrentCollectibles = 0; m_ESpecialInteractionType = ESpecialInteraction::Default; m_iInteractionIndex = 0; }

	
																	 private:
	//------------------------------------------------------------  CONST VARS -----------------------------------------------------------------------------------------			
	static const int m_kiStartingLives		= 3;							// The lives the player start with.																		
	static const int m_kIScoreIncrement		= 100;							// The score that collectibles, 100 like original manic miner.											
	static const int m_kiExtraLifeThreshold = 10000;						// The threshold to reach before an extra life is given.												
	static const int m_kiScorePerTimeLeft	= 11;							// How much score each tick will give. (almost like the original manic miner							
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//																																									
	//------------------------------------------------------------  LOGIC VARS -----------------------------------------------------------------------------------------
	int				 m_iCurrentScore;										// The current score the player has.																
	int				 m_iHighScore;											// The highscore the player has achieved (is persistent)											
	int				 m_iCurrentLives;										// The current lives of the player.																	
	int				 m_iCurrentCollectibles;								// The amount of collectibles the player has currently collected.									
	int				 m_iCurrentSwitches;									// The amount of switches the player has currently flipped.
	int				 m_iInteractionIndex;									// Keeps track of the current interaction index
	bool			 m_bDrainToScore;										// Can the air manager update the score?															
	bool			 m_bDoOnce;												// Used by the CML to initiate something only once.													
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//																																									
	//------------------------------------------------------------  POINTERS -------------------------------------------------------------------------------------------
	CAirManager*	 m_pcAirManager;										// The air manager, used for the drain.																
	CHUD*			 m_pcCHUD;												// The CHUD, *this will tell it when to update.														
	CLevelManager*	 m_pcLevelManager;										// What instantiate this classes, through this, we get access to the current manic layer.			
	CPlayer*		 m_pcCPlayer;											// The player, needed to let it know how much lives it has on a new map.							
	CMovingDoor*	 m_pcMovingDoor;																																	
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//																																									
	//---------------------------------------------------------  STRUCTS/ENUMS -----------------------------------------------------------------------------------------
	SLevelValues		m_sLevelValues;										// The Struct that will be set by levels, has the data needed to finish levels.
	ESpecialInteraction m_ESpecialInteractionType;							// A struct to dictate which part of the special interaction to do.
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//																																									
	//------------------------------------------------------------  FUNCTIONS ------------------------------------------------------------------------------------------
	void 			 ReadHighScore();										// Called when program begins, reads the values of "Highscore.bin" and sets the Highscore to be that.	
	void			 WriteHighScore() const;								// Called when the score exceeds the highscore, writes the new highscore to "Highscore.bin".			
	void			 DrainAirForScore();									// Called by the AirManager (for now), drains remaining air into score									
	bool 			 IsScoreGreaterThanHighscore() const;					// Checks to see if score exceeds the highscore.														
	bool			 CheckIfLevelRequirementsAreMet() ;						// Checks if the level has collected everything set in the SLevelValues.								
	void			 ExtraLifeCheck( int iScore );							// Checks if player has acquired more than 10,000 score, then awards a life if so.						
	void			 CheckHighScoreForUpdate();								// Checks if Highscore should be updated.																
	void			 IncreaseScore();										// Increases the score by a flat amount which is 100 (just like the original Manic Miner)				
	void			 UpdateHighScore() const;								// Tells the CHUD to update the high score with the current high score.									
	void			 UpdateScore()	  const;								// Tells the CHUD to update the score.																	
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------/

};
#endif // #ifndef _CGAMEMANAGER_H_