#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"

#include "CCollectible.h"
#include "CCollectibleManager.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// GetGCTypeIDOf uses the template in GCTypeID to generate a unique ID for 
// this type - need this to construct our base type
// N.B. this is super important!
//
//////////////////////////////////////////////////////////////////////////
CCollectible::CCollectible()
	: CGCObjSpritePhysics(GetGCTypeIDOf(CCollectible))
	, m_iIndex(0)
{

}

CCollectible::CCollectible(CCollectibleManager& collectibleManager)
	: CGCObjSpritePhysics(GetGCTypeIDOf(CCollectible))
	, m_iIndex(0)
	, m_rcCollectibleManager(nullptr)
{
	m_rcCollectibleManager = &collectibleManager;
}


CCollectible::CCollectible(cocos2d::Vec2 startPos)
	: CGCObjSpritePhysics(GetGCTypeIDOf(CCollectible))
	, m_iIndex(0)
{
	SetResetPosition(startPos);

}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CGCObject interface
//////////////////////////////////////////////////////////////////////////
IN_CPP_CREATION_PARAMS_DECLARE(CCollectible, "TexturePacker/Sprites/Egg/Egg.plist", "egg", b2_dynamicBody, true);
//virtual 
void CCollectible::VOnResourceAcquire()
{
	IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE(CCollectible);
	CGCObjSpritePhysics::VOnResourceAcquire();
}

//////////////////////////////////////////////////////////////////////////
//        virtual 
//////////////////////////////////////////////////////////////////////////
void CCollectible::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
}

void CCollectible::VOnUpdate(f32 fTimeStep)
{
	GetPhysicsBody()->SetGravityScale(0);
}

//////////////////////////////////////////////////////////////////////////
// Wrapper function to set the transform as PhysicsBody cannot be accessed
//////////////////////////////////////////////////////////////////////////
void CCollectible::SetPosition(const b2Vec2 pos)
{
	GetPhysicsBody()->SetTransform(pos, 0);
}

//////////////////////////////////////////////////////////////////////////
// Fires off on Collision and calls appropriate event on Manager
//////////////////////////////////////////////////////////////////////////
void CCollectible::InteractEvent()
{
	m_rcCollectibleManager->IncrementCollectible();
	m_rcCollectibleManager->RemoveCollectible(*this);
}
