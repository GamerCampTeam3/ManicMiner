////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <memory.h>
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjLander.h"

USING_NS_CC;

CGCObjLander::CGCObjLander(const cocos2d::Vec2& rcAnchorPoint, const float fSpeed, CGCFactoryCreationParams& rcFactoryCreationParamsInput)
	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjLander))
	, m_rFactoryCreationParams(rcFactoryCreationParamsInput)
	, m_cAnchorPoint(rcAnchorPoint)
	, m_fSpeed(fSpeed)
{
}

//////////////////////////////////////////////////////////////////////////
// This function initialises the enemies velocity and reset position.
// NOTE NOT COMPLETE AT MODULE 1
//////////////////////////////////////////////////////////////////////////
// virtual function
void CGCObjLander::VOnResourceAcquire( void )
{

	// Removed maco call so the reference m_rFactorCreationParams could be passed 
    // into VHandleFactoryParms.  Pending module 2 framework his may be done differently.
	//IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjLander );    
	VHandleFactoryParams(m_rFactoryCreationParams, GetResetPosition());

	CGCObjSpritePhysics::VOnResourceAcquire();

	m_cTotalVelocity = Vec2::ZERO;

	m_cTotalVelocity.x = m_fSpeed;

	SetResetPosition(m_cAnchorPoint);
}

//////////////////////////////////////////////////////////////////////////
// This function is called when a lander is resurected from the dead-list to the 
// live list.
// NOTE NOT COMPLETE AT MODULE 1
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjLander::VOnResurrected( void )
{
	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetGravityScale( 0.0f );
}

//////////////////////////////////////////////////////////////////////////
//Function to provide the frame update of this object
// NOTE NOT COMPLETE AT MODULE 1
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjLander::VOnUpdate(float fTimeStep)
{
	CGCObject::VOnUpdate(fTimeStep);
	   
}
