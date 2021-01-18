// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
#include "SolarLightB2AABBQueryCB.h"

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"
#include "ManicMiner/Platforms/CPlatform.h"
#include "ManicMiner/Player/CPlayer.h"
#include "ManicMiner/Enemy/GCObjEnemy.h"

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ReportFixture																						//
// -------------------------------------------------------------------------------------------------------------------- //
bool CSolarLightB2AABBQueryCB::ReportFixture( b2Fixture* fixture )
{
// Get Body from detected Fixture
	const b2Body* pB2Body = fixture->GetBody();

	CGCObjSpritePhysics* pGcSprPhys = ( CGCObjSpritePhysics* )pB2Body->GetUserData();
// If this is not a GC Object
	if( pGcSprPhys == nullptr )
	{
	// Return with fail
		return true;
	}
// If it is a GC Object, check if it is a CPlatform
	if( pGcSprPhys->GetGCTypeID() == GetGCTypeIDOf( CPlatform ) && !fixture->IsSensor() )
	{
	// If it is a CPlatform
	// Set hit flag to platform
		m_eQueryResult = ESolarLightQueryResult::Platform;

	// Return with success
		return true;
	}
	// Else, check if it is an enemy
	else if( pGcSprPhys->GetGCTypeID() == GetGCTypeIDOf( CGCObjEnemy ) )
	{
	// If it is an Enemy
	// Set hit flag to enemy
		m_eQueryResult = ESolarLightQueryResult::Enemy;

	// Return with success
		return true;
	}
	// Else, check if it is the player
	else if( pGcSprPhys->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) )
	{
	// If it is the Player
	// Set hit flag to player
		m_eQueryResult = ESolarLightQueryResult::Player;

	// Return with success
		return true;
	}
	else
	{
	// Return with fail
		return true;
	}
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	Reset																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CSolarLightB2AABBQueryCB::Reset()
{
	m_eQueryResult = ESolarLightQueryResult::None;
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	GetQueryResult																						//
// -------------------------------------------------------------------------------------------------------------------- //
ESolarLightQueryResult CSolarLightB2AABBQueryCB::GetQueryResult() const
{
	return m_eQueryResult;
}