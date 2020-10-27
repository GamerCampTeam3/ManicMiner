//////////////////
/// Bib
//////////////////
/// DEPRECATED CODE

#ifndef _CCOLLECTIBLEMANAGER_H_
#define _CCOLLECTIBLEMANAGER_H_
#include <array>


#include "GamerCamp/GCObject/GCObjectGroup.h"
#include "ManicMiner/Structs/SCollectibles.h"
#include "ManicMiner/Structs/SSwitches.h"

class CCollectibleOLD;
///////////////////////////////////////////
/// Namespaces and Class declaration
namespace cocos2d
{
	class Vec2;
}

class CCollectibleOLD;
class CSwitch;
///////////////////////////////////////////

class CCollectibleManager : public CGCObjectGroup
{
public:
	// Those values dictates how much total Switch/Collectibles to generate
	// This should never be above the highest maximum from any level
	static const int m_kiMaxCollectiblesToGenerate = 2;
	static const int m_kiMaxSwitchesToGenerate = 2;


private:
	// TODO: clean up the structs
	SCollectibles	m_asCollectibles[m_kiMaxCollectiblesToGenerate];
	SSwitches		m_asSwitches[m_kiMaxSwitchesToGenerate];

	int			    m_iCollected;
	int				m_iCollectiblesNeeded;

	int				m_iSwitchesFlipped;
	int				m_iSwitchedFlippedNeeded;

public:
	// Ctor + Overloaded to take in array of Vector2s with an amount to spawn as well as required collectibles for first level
	CCollectibleManager();
	CCollectibleManager( cocos2d::Vec2 spawnPosition[], int numToSpawn, int collectiblesNeeded );
	// Dtor, clears the arrays
	~CCollectibleManager();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Functions used for level generation and clearing
	// Generates CCollectibles and Switches depending on the levels need
	void GenerateNewCCollectibles(int numToSpawn, cocos2d::Vec2 spawnPos[], int collectiblesNeeded);
	void GenerateNewCSwitches(int numToSpawn, cocos2d::Vec2 spawnPos[], int switchesNeeded);

	// Feeds into the above, used to check when level can end
	void SetNeededNumOfCollectibles(int amount);
	void SetNeededNumofSwitchesFlipped(int amount);

	// Resets all collectibles to be used for next level
	void ResetCollectibles();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void	VOnGroupResourceAcquire_PostObject	() override;

	// Collision things for Collectibles/Switches
	void IncrementCollectible();
	void RemoveCollectible(CCollectibleOLD& collectible);
	void IncrementSwitches();
	void FlipSwitch(CSwitch& cswitch);
	void ResetCurrentCollectibles();

	// Checks if level can be completed
	bool CheckCollectiblesNeeded();
	bool CheckSwitchNeeded();


private:
};

#endif // #ifndef _CCOLLECTIBLEMANAGER_H_