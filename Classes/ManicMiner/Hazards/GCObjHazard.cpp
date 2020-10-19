////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjHazard.h"

USING_NS_CC;

CGCObjHazard::CGCObjHazard(const cocos2d::Vec2& AnchorPoint, const HazardTypes::EHazardId HazardIdentifierInput, CGCFactoryCreationParams& ParamsInput)
	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjHazard))
	, rFactoryCreationParams(ParamsInput)
	, m_cAnchorPoint(AnchorPoint)
	, eHazardIdentifier(HazardIdentifierInput)
{
}

//////////////////////////////////////////////////////////////////////////
// This function initialises an enemies direction of velocity, 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjHazard::VOnResourceAcquire( void )
{

	//IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjHazard );    
	VHandleFactoryParams(rFactoryCreationParams, GetResetPosition());

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
