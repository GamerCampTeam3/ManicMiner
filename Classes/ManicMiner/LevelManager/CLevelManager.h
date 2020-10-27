#ifndef _CLEVELMANAGER_H_
#define _CLEVELMANAGER_H_

class IGCGameLayer;
class CManicLayer;
namespace cocos2d
{
	class Director;
}

class CLevelManager
{
public:
	CLevelManager( cocos2d::Director& rcDirector );
	virtual ~CLevelManager();

	void GoToMainMenu();
	void GoToNextLevel();

	// Gets called to start game (after menu)
	void EnterCavern();

	// Returns current Layer, casted as a CManicLayer
	CManicLayer& GetCurrentManicLayer();
private:
	int m_iCurrentLevelIndex;				// Indicates level ( 0 means Menu, 1 means Central Cavern and so on)
};
#endif //#ifndef _CLEVELMANAGER_H_