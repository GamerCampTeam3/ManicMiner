////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <memory.h>

#include "GamerCamp/GCObject/GCObjectManager.h"

#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

#include "GCObjLander.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// GetGCTypeIDOf uses the template in GCTypeID to generate a unique ID for 
// this type - need this to construct our base type


CGCObjLander::CGCObjLander(const cocos2d::Vec2& rcAnchorPoint, const float fSpeed, CGCFactoryCreationParams& rcParamsInput)
	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjLander))
	, m_rFactoryCreationParams(rcParamsInput)
	, m_cAnchorPoint(rcAnchorPoint)
	, m_fSpeed(fSpeed)
{
}

//virtual 
void CGCObjLander::VOnResourceAcquire( void )
{

	//IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjLander );    
	VHandleFactoryParams(m_rFactoryCreationParams, GetResetPosition());

	CGCObjSpritePhysics::VOnResourceAcquire();

	m_cTotalVelocity = Vec2::ZERO;

	m_cTotalVelocity.x = m_fSpeed;

	SetResetPosition(m_cAnchorPoint);

}

//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjLander::VOnResurrected( void )
{
	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetGravityScale( 0.0f );
}


//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjLander::VOnUpdate(float fTimeStep)
{
	CGCObject::VOnUpdate(fTimeStep);

}
