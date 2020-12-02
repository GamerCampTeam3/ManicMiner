#include "SolarLightB2RayCastCallBack.h"

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"
#include "ManicMiner/Platforms/CPlatform.h"
#include "ManicMiner/Enemy/GCObjEnemy.h"

float32 CSolarLightB2RayCastCallBack::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction )
{
	const b2Body* pB2Body = fixture->GetBody();

	CGCObjSpritePhysics* pGcSprPhys = ( CGCObjSpritePhysics* )pB2Body->GetUserData();
// If this is not a GC Object
	if( pGcSprPhys == nullptr )
	{
		// Return with failed info
		return -1.0f;
	}
// If it is a GC Object, check if it is a CPlatform
	if( pGcSprPhys->GetGCTypeID() == GetGCTypeIDOf( CPlatform ) )
	{
	// If it is a CPlatform
		CPlatform* pPlatform = static_cast< CPlatform* >( pGcSprPhys );
		if( pPlatform != nullptr )
		{
			// Set hit flag to true, platform
			m_eHitResult = ESolarLightRayCastHitResult::Platform;

			// Return with success info
			return 1.0f;
		}
	}
// Else, check if it is an enemy
	else if( pGcSprPhys->GetGCTypeID() == GetGCTypeIDOf( CGCObjEnemy ) )
	{
		CGCObjEnemy* pEnemy = static_cast< CGCObjEnemy* >( pGcSprPhys );
		if( pEnemy != nullptr )
		{
			// Set hit flag to true, enemy
			m_eHitResult = ESolarLightRayCastHitResult::Enemy;

			// Return with success info
			return 1.0f;
		}
	}
	else
	{
		// Return with failed info
		return -1.0f;
	}
}

void CSolarLightB2RayCastCallBack::ResetFlag()
{
	m_eHitResult = ESolarLightRayCastHitResult::None;
}

ESolarLightRayCastHitResult CSolarLightB2RayCastCallBack::GetRaycastHitResult() const
{
	return m_eHitResult;
}