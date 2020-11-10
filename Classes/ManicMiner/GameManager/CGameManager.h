//////////////////
/// Bib
//////////////////
#ifndef _CGAMEMANAGER_H_
#define _CGAMEMANAGER_H_

#include "ManicMiner/Structs/SLevelValues.h"

class CAirManager;
class CPlayer;
class CHUD;
class CLevelManager;




class CGameManager
{
public:
	CGameManager(CLevelManager& rcLevelManager );
	~CGameManager();
	
	void	IncreaseScore();				// Increases the score by a flat amount which is 100 (just like the original Manic Miner)
	void	WriteHighScore();				// To be called upon Hard Death (all lives out), stores the score to text.
	
	void	CCollectibleInteractEvent();	// Updates the elements upon collecting with a collectible.
	void	CSwitchInteractEvent();			// Updates the elements upon interacting with a cswitch.

	// This function is called by each individual level and tells the manager what is the requirement for that level
	void	SetLevelRequirements( const SLevelValues& levelValues );


	void	ResetValues();					// Resets the values required for level management.

	// Kind of wrapper functions for telling the CHUD to update.
	void	UpdateHighScore() const;		// Tells the CHUD to update the high score with the current high score.
	void	UpdateLives()	  const;		// Tells the CHUD to update the lives with the current lives.
	void	UpdateScore()	  const;		// Tells the CHUD to update the score.

	// References, they will then call the function required to update some of the elements.
	void	SetCPlayer( CPlayer* pcPlayer );	// Sets the pointer to the current CPlayer.
	void	SetCHUD( CHUD* pcCHUD );			// Sets the pointer to the current CHUD.
	void	SetCAirManager( CAirManager* pcAirManager );
	void	EndLevel() ;
	void	DrainToScore();
	bool	GetCanDrainToScore() { return m_bDrainToScore; }
private:
	int		m_iCurrentScore;
	int		m_iHighScore;
	int		m_iCurrentLives;
	int		m_iCurrentCollectibles;
	int		m_iRequiredCollectibles;
	int		m_iCurrentSwitches;
	int		m_iRequiredSwitches;
	bool	m_bDrainToScore;

	CAirManager*				m_pcAirManager;
	CHUD*						m_pcCHUD;
	CLevelManager*				m_pcLevelManager;
	CPlayer*					m_pcCPlayer;
	SLevelValues				m_sLevelValues;


	// Constant values
	static const int m_kiStartingLives		= 3;
	static const int m_kiScoreIncrements	= 100;
	static const int m_kiExtraLifeThreshold = 10000;


	void 		ReadHighScore();
	bool 		CheckScore() const;
	const bool	CheckIfEnoughReached() const;
	void		ExtraLifeCheck() const;
	void		DrainAirForScore() ;
	void		CheckHighScore();
	
};

#endif // #ifndef _CGAMEMANAGER_H_