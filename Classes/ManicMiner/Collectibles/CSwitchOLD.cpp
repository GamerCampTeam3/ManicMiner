#include "CSwitchOLD.h"

#include <memory.h>

#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"

#include "CCollectibleManager.h"



USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// GetGCTypeIDOf uses the template in GCTypeID to generate a unique ID for 
// this type - need this to construct our base type
// N.B. this is super important!
//
//////////////////////////////////////////////////////////////////////////
CSwitchOLD::CSwitchOLD()
	: CGCObjSpritePhysics(GetGCTypeIDOf(CSwitch))
	, m_iIndex(0)
	, m_rcCollectibleManager(nullptr)
{

}

CSwitchOLD::CSwitchOLD(cocos2d::Vec2 startPos)
	: CGCObjSpritePhysics(GetGCTypeIDOf(CSwitch))
	, m_iIndex(0)
	, m_rcCollectibleManager(nullptr)
{
	SetResetPosition(startPos);

}

CSwitchOLD::CSwitchOLD(CCollectibleManager& collectibleManager)
	: m_iIndex(0)
	, m_rcCollectibleManager(nullptr)
{
	m_rcCollectibleManager = &collectibleManager;
}

void CSwitchOLD::InteractEvent()
{
	m_rcCollectibleManager->IncrementSwitches();
	//m_rcCollectibleManager->FlipSwitch(*this);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGCObject interface
IN_CPP_CREATION_PARAMS_DECLARE(CCollectible, "TexturePacker/Sprites/Egg/Egg.plist", "egg", b2_dynamicBody, true);
//virtual 
void CSwitchOLD::VOnResourceAcquire()
{
	IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE(CCollectible);
	CGCObjSpritePhysics::VOnResourceAcquire();
}
//////////////////////////////////////////////////////////////////////////
//  virtual
//////////////////////////////////////////////////////////////////////////
void CSwitchOLD::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
}
void CSwitchOLD::VOnUpdate(f32 fTimeStep)
{
	GetPhysicsBody()->SetGravityScale(0.0f);
}
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

