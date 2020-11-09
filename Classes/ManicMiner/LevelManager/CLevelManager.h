// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	CLevelManager handles the logic that initializes the game itself									//
//					Creates all the CManicLayer's as well as the CMenuLayer												//
//					Keeps track of the current level, and provides functionality to proceed								//
//					Either by advancing to next the level or going back to the Main Menu upon fatal death				//
// -------------------------------------------------------------------------------------------------------------------- //


#ifndef _CLEVELMANAGER_H_
#define _CLEVELMANAGER_H_

// ----------------------------------------- Fwd declares ------------------------------------------------------------- //
class CManicLayer;																										//
class CGameManager;																										//
namespace cocos2d																										//
{																														//
	class Director;																										//
}																														//
// -------------------------------------------------------------------------------------------------------------------- //

class CLevelManager
{
public:
// Constructor -------------------------------------------------------------------------------------------------------- //
	CLevelManager( cocos2d::Director& rcDirector );

// Destructor  -------------------------------------------------------------------------------------------------------- //
	virtual ~CLevelManager();

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	GoToMainMenu																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Transitions to the Main Menu scene and resets the current level index to 0							//
//					Called from the current CManicLayer ( current level ) when player dies fatally						//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
//																														//
// See also		:	CMenuLayer																							//
// -------------------------------------------------------------------------------------------------------------------- //
	void GoToMainMenu();

	CGameManager& AccessGameManager() const;

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	GoToNextLevel																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Transitions to the next level in the sequence and increments the current level index by 1			//
//					Called from the current CManicLayer ( current level ) when player wins								//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void GoToNextLevel();

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	EnterCavern																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Initialises game properties ( lives, score ), then transitions to CMLCentralCavern					//
//					Called from the Main Menu when the Start Button is pressed											//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void EnterCavern();

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	GetCurrentManicLayer																				//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Returns a reference to the currently active CManicLayer												//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	CManicLayer&																						//
//					reference to the currently active CManicLayer														//
// -------------------------------------------------------------------------------------------------------------------- //
	CManicLayer& GetCurrentManicLayer() const;

private:

	// Indicates current level through a number
	// 0 for Menu, 1 for CentralCaver, 2 for ColdRoom, and so on
	// Used in GoToNextLevel() for the switch case that selects which level should be loaded next
	int m_iCurrentLevelIndex;

	CGameManager* m_pcCGameManager;
};
#endif //#ifndef _CLEVELMANAGER_H_