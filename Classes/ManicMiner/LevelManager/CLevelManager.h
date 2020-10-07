#ifndef _CLEVELMANAGER_H_
#define _CLEVELMANAGER_H_

#include "Classes/ManicMiner/TSingleton/TSingleton.h"

class IGCGameLayer;
class CManicLayer;

class CLevelManager : public TSingleton< CLevelManager >
{
public:
	CLevelManager();
	virtual ~CLevelManager();

	void Init();

	void GoToMainMenu();
	void GoToNextLevel();

	CManicLayer& GetCurrentLevelLayer();
private:
	int m_iCurrentLevelIndex;			// Indicates level
	//CManicLayer* m_pArrManicLayers[3];	// Array to hold layer info

	void UpdateLevelInfo();
};

#endif //#ifndef _CLEVELMANAGER_H_