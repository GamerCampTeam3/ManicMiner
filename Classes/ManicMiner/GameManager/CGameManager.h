//////////////////
/// Bib
//////////////////
#ifndef _CGAMEMANAGER_H_
#define _CGAMEMANAGER_H_

#include "ManicMiner/Enums/ECollectibleTypeRequired.h"


class CHUD;
class CLevelManager;


class CGameManager
{
public:
	CGameManager();
	CGameManager(CLevelManager& LevelManager);
	~CGameManager();

	void	IncreaseScore();			// Increases the score by a flat amount which is 100 (just like the original Manic Miner)
	int	GetCurrentLives();			// Returns the current player lives
	int	GetHighScore();				// Returns the high-score
	int	GetScore();				// Returns the current score
	void	WriteHighScore();			// To be called upon Hard Death (all lives out), stores the score to text.
	void	SetCHUD( CHUD* cCHUD );			// Sets the pointer to the current CHUD
	void	CCollectibleInteractEvent();
	void	CSwitchInteractEvent();
	void	InitLevelEndRequirements( ECollectibleTypeRequired eType, int collectibles = 0, int switches = 0 );
	void	UpdateLives();
	void	SetLives(int lives);
	void	ResetValues();

private:
	int		m_iCurrentScore;
	int		m_iHighScore;
	int		m_iScoreIncrement;
	int		m_iCurrentLives;
	int		m_iCurrentCollectibles;
	int		m_iRequiredCollectibles;
	int		m_iCurrentSwitches;
	int		m_iRequiredSwitches;
	
	CHUD*	m_pcCHUD;
	CLevelManager* m_pcLevelManager;
	ECollectibleTypeRequired	m_eTypeRequired;

	void 	ReadHighScore();
	bool 	CheckScore();
	bool	CheckIfEnoughReached();
	
};

#endif // #ifndef _CGAMEMANAGER_H_