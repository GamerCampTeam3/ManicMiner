#include "CCollectibleManager.h"
#include "CCollectible.h"
#include "CSwitch.h"

CCollectibleManager::CCollectibleManager()
	: m_iCollected(0)
	, m_iCollectiblesNeeded(0)
{
  // Initialize the arrays of struct
  for (int i = 0; i < m_kiMaxCollectiblesToGenerate; i++)
  {
      m_asCollectibles[i].p_cCollectible = new CCollectible();
      m_asCollectibles[i].p_cCollectible->SetIndex(i);
      m_asCollectibles[i].b_IsActive = false;
  }
  for (int i = 0; i < m_kiMaxSwitchesToGenerate; i++)
  {
      m_asSwitches[i].p_cSwitch = new CSwitch();
      m_asSwitches[i].p_cSwitch->SetIndex(i);
      m_asSwitches[i].b_IsActive = false;
  }
}

void CCollectibleManager::CollectiblesToSpawn(int numToSpawn, cocos2d::Vec2 spawnPos[])
{
	for (int i = 0; i < numToSpawn; i++)
	{
        m_asCollectibles[i].p_cCollectible->SetResetPosition(spawnPos[i]);
        m_asCollectibles[i].b_IsActive = true;
	}
}


void CCollectibleManager::SwitchesToSpawn(int numToSpawn, cocos2d::Vec2 spawnPos[])
{
    for (int i = 0; i < numToSpawn; i++)
    {
        m_asSwitches[i].p_cSwitch->SetResetPosition(spawnPos[i]);
        m_asSwitches[i].b_IsActive = true;
    }
}


void CCollectibleManager::ResetCollectibles()
{
    for (CCollectibleStruct m_as_collectible : m_asCollectibles)
    {
        m_as_collectible.p_cCollectible->SetResetPosition(cocos2d::Vec2(-99999.f, -99999.f));
        m_as_collectible.p_cCollectible->SetVisible(false);
        m_as_collectible.p_cCollectible->SetIndex(0);
        m_as_collectible.b_IsActive = false;
    }
	
    for (CSwitchStruct m_as_switch : m_asSwitches)
    {
        m_as_switch.p_cSwitch->SetResetPosition(cocos2d::Vec2(-99999.f, -99999.f));
        m_as_switch.p_cSwitch->SetVisible(false);
        m_as_switch.p_cSwitch->SetIndex(0);
        m_as_switch.b_IsActive = false;
    }

    m_iCollected = 0;
}

void CCollectibleManager::SetNeededNumOfCollectibles(int amount)
{
    m_iCollectiblesNeeded = amount;
}





CCollectibleManager::~CCollectibleManager()
{
  //Set all pointers to null
    for (CCollectibleStruct m_as_collectible : m_asCollectibles)
    {
        m_as_collectible.p_cCollectible = nullptr;
    }

    for (CSwitchStruct m_as_switch : m_asSwitches)
    {
        m_as_switch.p_cSwitch = nullptr;
    }
}



