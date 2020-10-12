#ifndef _CCOLLECTIBLEMANAGER_H_
#define _CCOLLECTIBLEMANAGER_H_
#include <array>

namespace cocos2d
{
	class Vec2;
}

class CSwitch;
class CCollectible;

struct CCollectibleStruct
{
	CCollectible* p_cCollectible;
	bool	      b_IsActive;
};

struct CSwitchStruct
{
	CSwitch*      p_cSwitch;
	bool	      b_IsActive;
};


class CCollectibleManager
{
public:
	static const int m_kiMaxCollectiblesToGenerate = 10;
	static const int m_kiMaxSwitchesToGenerate = 10;


private:
	CCollectibleStruct m_asCollectibles[m_kiMaxCollectiblesToGenerate];
	CSwitchStruct	   m_asSwitches[m_kiMaxSwitchesToGenerate];
	int m_iCollected;
	int m_iCollectiblesNeeded;

public:
	CCollectibleManager();
	~CCollectibleManager();

	void CollectiblesToSpawn ( int numToSpawn, cocos2d::Vec2 spawnPos[] );
	void SwitchesToSpawn( int numToSpawn, cocos2d::Vec2 spawnPos[]);
	void ArrTest(cocos2d::Vec2 spawnPos[], int numToSpawn);
	void ResetCollectibles();
	void SetNeededNumOfCollectibles(int amount);
private:
};

#endif // #ifndef _CCOLLECTIBLEMANAGER_H_