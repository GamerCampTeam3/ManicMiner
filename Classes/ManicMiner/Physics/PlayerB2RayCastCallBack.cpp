#include "PlayerB2RayCastCallBack.h"

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"
//#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "ManicMiner/Platforms/CPlatform.h"

float32 CPlayerB2RayCastCallBack::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction )
{
	const b2Body* pB2Body = fixture->GetBody();
	CGCObjSpritePhysics* pGcSprPhys = ( CGCObjSpritePhysics* )pB2Body->GetUserData();

// If this is not a GC Object
	if ( pGcSprPhys == nullptr)
	{
	// Return with failed info
		return -1.0f;
	}

// If is a GC Object, try to downcast into a CPlatform
	CPlatform* pPlatform = static_cast< CPlatform* >( pGcSprPhys );

// If it is a CPlatform, and if it is of Brick Type
	if( pPlatform != nullptr && pPlatform->GetPlatformType() == EPlatformType::Brick )
	{
	// Set hit flag to true
		m_bDidRayHit = true;

	// Return with success info
		return 1.0f;
	}
	else
	{
	// Return with failed info
		return -1.0f;
	}
}

void CPlayerB2RayCastCallBack::ResetFlag()
{
	m_bDidRayHit = false;
}

bool CPlayerB2RayCastCallBack::GetDidRayHit() const
{
	return m_bDidRayHit;
}
