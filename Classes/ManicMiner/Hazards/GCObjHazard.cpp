////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjHazard.h"

#ifndef TINYXML2_INCLUDED
#include "external\tinyxml2\tinyxml2.h"
#endif

#ifndef _GCLEVELLOADER_OGMO_H_
#include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

USING_NS_CC;

GCFACTORY_IMPLEMENT_CREATEABLECLASS(CGCObjHazard);

//Module 1 
//CGCObjHazard::CGCObjHazard(const cocos2d::Vec2& rcAnchorPoint, const HazardTypes::EHazardId eHazardId, CGCFactoryCreationParams& rcFactoryCreationParamsInput)
//	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjHazard))
//	, m_rFactoryCreationParams(rcFactoryCreationParamsInput)
//	, m_cAnchorPoint(rcAnchorPoint)
//	, m_eHazardIdentifier(eHazardId)
//{
//}

CGCObjHazard::CGCObjHazard()
	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjHazard))
//	, m_pCustomCreationParams(nullptr)
{
}

//////////////////////////////////////////////////////////////////////////
// This function initialises an enemies direction of velocity, 
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjHazard::VOnResourceAcquire( void )
{
	// Removed maco call so the reference m_rFactorCreationParams could be passed 
	// into VHandleFactoryParms.  Pending module 2 framework his may be done differently.
	//IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjHazard );    
	//VHandleFactoryParams(m_rFactoryCreationParams, GetResetPosition());

	CGCObjSpritePhysics::VOnResourceAcquire();

	//SetResetPosition(m_cAnchorPoint);
}

//////////////////////////////////////////////////////////////////////////
// This function is called when an hazard is resurected from the dead-list to the 
// live list.
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjHazard::VOnResurrected( void )
{
	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetGravityScale( 0.0f );
}

//////////////////////////////////////////////////////////////////////////
// Function to provide the frame update of this object
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjHazard::VOnUpdate(float fTimeStep)
{
	// Call base class version first.
	CGCObject::VOnUpdate(fTimeStep);

	// D.O'DWYER - THIS FUNCTION IS NOT REQUIRED AND SHOULD BE REMOVED AT MODULE 2.
}
