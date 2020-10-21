////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjHazard.h"

USING_NS_CC;

CGCObjHazard::CGCObjHazard(const cocos2d::Vec2& rcAnchorPoint, const HazardTypes::EHazardId eHazardId, CGCFactoryCreationParams& rcParamsInput)
	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjHazard))
	, m_rFactoryCreationParams(rcParamsInput)
	, m_cAnchorPoint(rcAnchorPoint)
	, m_eHazardIdentifier(eHazardId)
{
}

//////////////////////////////////////////////////////////////////////////
// This function initialises an enemies direction of velocity, 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjHazard::VOnResourceAcquire( void )
{

	//IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjHazard );    
	VHandleFactoryParams(m_rFactoryCreationParams, GetResetPosition());

	CGCObjSpritePhysics::VOnResourceAcquire();
	
	SetResetPosition(m_cAnchorPoint);

}

//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjHazard::VOnResurrected( void )
{
	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetGravityScale( 0.0f );
}

//////////////////////////////////////////////////////////////////////////
//virtual function

void CGCObjHazard::VOnUpdate(float fTimeStep)
{
	CGCObject::VOnUpdate(fTimeStep);




}
