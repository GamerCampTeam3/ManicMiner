// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
//	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose	:		CGameInstance was a templated singleton in which we would handle most game logic					//	
//					such as player death, item collected, going to next level, game states, etc;						//	
//					But after the first tutorial with Zaf, it was decided this was not the best approach 				//
//					and all this logic has since been moved to the CManicLayer class									//
//																														//
//					Completely commented out and excluded from the build in project settings							//
//					Only exists still for submission purposes															//
//																														//
// See Also	:		CManicLayer.h																						//
// -------------------------------------------------------------------------------------------------------------------- //



//#ifndef _CGAMEINSTANCE_H_
//#define _CGAMEINSTANCE_H_
//
//#include "ManicMiner/TSingleton/TSingleton.h"
//#include "ManicMiner/GameState/EGameState.h"
//#include "GamerCamp/Core/GCTypes.h"
//
//class CCollectibleManager;
//class CPlayer;
//class CGCObjItem;
//class IGCGameLayer;
//class CLevelManager;
//class CAirManager;
//class CManicLayer;
//
//
//class CGameInstance : public TSingleton<CGameInstance>
//{
//private:
//	CPlayer* m_pPlayer;
//	CLevelManager* m_pLevelManager;
//	CAirManager* m_pAirManager;
//	CCollectibleManager* m_pCollectibleManager;
//	
//	EGameState m_eGameState;
//
//	void OutOfLives();
//
//public:
//	CGameInstance();
//
//	void Init();
//
//	EGameState GetGameState();
//	void SetGameState( EGameState gameState );
//
//	void SetPlayer( CPlayer& rPlayer );
//
//	void OnFinishedLooting();
//	void OnPlayerDeath( CPlayer& rPlayer );
//	void OnEscaped();
//	void OnItemCollected( CGCObjItem& rItem );
//
//	void EnterCavern();
//
//
//	// Runs before level transition begins
//	void PlayerLeavingLevel( CManicLayer& rNewManicLayer );
//
//	void PlayerEnteredNewLevel( CManicLayer& rNewManicLayer, CPlayer& rPlayer );
//
//	void OutOfAir();
//
//	void Update( f32 fTimeStep );
//
//private:
//	////////////////////////////////////////////////////////////////////////// 
//	// reset handling
//	bool m_bResetWasRequested;
//
//	void RequestReset()
//	{
//		m_bResetWasRequested = true;
//	}
//
//	void ResetRequestWasHandled()
//	{
//		m_bResetWasRequested = false;
//	}
//
//	bool ResetWasRequested()
//	{
//		return m_bResetWasRequested;
//	}
//
//	void ResetLevel();
//};
//
//
//#endif //#ifdef	_CGAMEINSTANCE_H_