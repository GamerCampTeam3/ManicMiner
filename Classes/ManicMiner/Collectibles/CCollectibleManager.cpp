#include "CCollectibleManager.h"
#include "CCollectibleOLD.h"
#include "CSwitch.h"

#include "GamerCamp/Core/GCTypes.h"
#include "GamerCamp/GCObject/GCObject.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

#include "ManicMiner/Structs/SCollectibles.h"
#include "ManicMiner/Structs/SSwitches.h"

CCollectibleManager::CCollectibleManager()
    : m_iCollected(0)
    , m_iCollectiblesNeeded(4)
    , m_iSwitchesFlipped(0)
    , m_iSwitchedFlippedNeeded(0)
{
    // Initialize the arrays of struct
    for (int i = 0; i < m_kiMaxCollectiblesToGenerate; i++)
    {
        m_asCollectibles[i].p_cCollectible = new CCollectibleOLD(*this);
        m_asCollectibles[i].p_cCollectible->SetIndex(i);
        //m_asCollectibles[i].p_cCollectible->SetSpritePosition(cocos2d::Vec2 (500.f, 300.f ));
        m_asCollectibles[i].b_IsActive = false;
    }
    for (int i = 0; i < m_kiMaxSwitchesToGenerate; i++)
    {
        m_asSwitches[i].p_cSwitch = new CSwitch();
        m_asSwitches[i].p_cSwitch->SetIndex(i);
        m_asSwitches[i].b_IsActive = false;
    }
}


void CCollectibleManager::VOnGroupResourceAcquire_PostObject()
{
		// we do this here because the alternative is to do it for each invader as they're created and it's

	// parent class version
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();

	// set up animations for all items
	const char* pszPlist	= "TexturePacker/Sprites/Egg/Egg.plist";

	// make an animation
		cocos2d::ValueMap&	rdictPList = GCCocosHelpers::CreateDictionaryFromPlist( pszPlist );

	ForEachObject( [&] ( CGCObject* pObject ) -> bool
	{
		CCAssert( ( GetGCTypeIDOf( CCollectibleOLD ) == pObject->GetGCTypeID() ),
			"CGCObject derived type mismatch!" );

		CGCObjSprite* pItemSprite = (CGCObjSprite*)pObject;
		return true;
	} );
}

// Overloaded Ctor that takes in initial values (for first level?)
CCollectibleManager::CCollectibleManager(cocos2d::Vec2 spawnPosition[], int numToSpawn, int collectiblesNeeded)
    : m_iCollected(0)
    , m_iCollectiblesNeeded(collectiblesNeeded)
    , m_iSwitchesFlipped(0)
    , m_iSwitchedFlippedNeeded(0)
{
    // Initialize the arrays of struct
    // Takes the values given to the Ctor for the first level
    for (int i = 0; i < numToSpawn; i++)
    {
        m_asCollectibles[i].p_cCollectible = new CCollectibleOLD(*this);
        m_asCollectibles[i].p_cCollectible->SetResetPosition(spawnPosition[i]);
        m_asCollectibles[i].p_cCollectible->SetIndex(i);
        m_asCollectibles[i].b_IsActive = false;
    }

    // Simply creates the switches to be used at later levels
    for (int i = 0; i < m_kiMaxSwitchesToGenerate; i++)
    {
        m_asSwitches[i].p_cSwitch = new CSwitch();
        m_asSwitches[i].p_cSwitch->SetIndex(i);
        m_asSwitches[i].b_IsActive = false;
    }
}

void CCollectibleManager::IncrementCollectible()
{
    m_iCollected++;
    CheckCollectiblesNeeded();
}

void CCollectibleManager::IncrementSwitches()
{
    m_iSwitchesFlipped++;
    CheckSwitchNeeded();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Called by the levels to place the collectibles and switches as well as amount required to be collected by player
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCollectibleManager::GenerateNewCCollectibles(int numToSpawn, cocos2d::Vec2 spawnPos[], int collectiblesNeeded)
{
    for (int i = 0; i < numToSpawn; i++)
    {
        m_asCollectibles[i].p_cCollectible->SetSpritePosition(spawnPos[i]);
        m_asCollectibles[i].p_cCollectible->SetVisible(true);
        m_asCollectibles[i].b_IsActive = true;
    }
}

void CCollectibleManager::GenerateNewCSwitches(int numToSpawn, cocos2d::Vec2 spawnPos[], int switchesNeeded)
{
    m_iCollected = 0;
    SetNeededNumofSwitchesFlipped(switchesNeeded);
    for (int i = 0; i < numToSpawn; i++)
    {
        m_asSwitches[i].p_cSwitch->SetSpritePosition(spawnPos[i]);
        m_asSwitches[i].p_cSwitch->SetVisible(true);
        m_asSwitches[i].b_IsActive = true;
    }
}

void CCollectibleManager::SetNeededNumOfCollectibles(int amount)
{
    m_iCollectiblesNeeded = amount;
}


void CCollectibleManager::SetNeededNumofSwitchesFlipped(int amount)
{
    m_iSwitchedFlippedNeeded = amount;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCollectibleManager::ResetCurrentCollectibles()
{
    m_iCollected = 0;
}


// Removes the collectibles from the screen
// This is a hacky way to remove them and should be changed
// TODO: Investigate the ::ObjectKill method

void CCollectibleManager::RemoveCollectible( CCollectibleOLD& collectible)
{
    int i = collectible.GetIndex();
    m_asCollectibles[i].b_IsActive = false;
    m_asCollectibles[i].p_cCollectible->SetVisible(false);
    m_asCollectibles[i].p_cCollectible->SetSpritePosition(cocos2d::Vec2(-99999.f, -99999.f));
}


void CCollectibleManager::FlipSwitch(CSwitch& cswitch)
{
    cswitch.SetFlippedX(true);
}

// Upon level end, reset every objects, ready to be called by next level
void CCollectibleManager::ResetCollectibles()
{
	if (m_asCollectibles != nullptr)
    for (SCollectibles m_as_collectible : m_asCollectibles)
    {
        m_as_collectible.p_cCollectible->SetVisible(false);
        m_as_collectible.p_cCollectible->SetSpritePosition(cocos2d::Vec2(-99999.f, -99999.f));
        m_as_collectible.p_cCollectible->SetIndex(0);
        m_as_collectible.b_IsActive = false;
    }

    for (SSwitches m_as_switch : m_asSwitches)
    {
        m_as_switch.p_cSwitch->SetVisible(false);
        m_as_switch.p_cSwitch->SetSpritePosition(cocos2d::Vec2(-99999.f, -99999.f));
        m_as_switch.p_cSwitch->SetIndex(0);
        m_as_switch.b_IsActive = false;
    }

    m_iCollected = 0;
}

// Returns True or False if so
bool CCollectibleManager::CheckCollectiblesNeeded()
{
    return (m_iCollected == m_iCollectiblesNeeded);
}

bool CCollectibleManager::CheckSwitchNeeded()
{
    return (m_iSwitchesFlipped == m_iSwitchedFlippedNeeded);
}


CCollectibleManager::~CCollectibleManager()
{
    //Set all pointers to null
    for (SCollectibles m_as_collectible : m_asCollectibles)
    {
        m_as_collectible.p_cCollectible = nullptr;
    }

    for (SSwitches m_as_switch : m_asSwitches)
    {
        m_as_switch.p_cSwitch = nullptr;
    }
}