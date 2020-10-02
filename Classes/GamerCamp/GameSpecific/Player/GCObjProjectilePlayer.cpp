////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <memory.h>

#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GameSpecific/Player/GCObjProjectilePlayer.h"
#include "GamerCamp/GameSpecific/Player/GCObjGroupProjectilePlayer.h"

#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"

#include "GCObjProjectilePlayer.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// GetGCTypeIDOf uses the template in GCTypeID to generate a unique ID for 
// this type - need this to construct our base type
// N.B. this is super important!
//
//////////////////////////////////////////////////////////////////////////
CGCObjProjectilePlayer::CGCObjProjectilePlayer()
: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjProjectilePlayer ) )
{	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGCObject interface

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
IN_CPP_CREATION_PARAMS_DECLARE( CGCObjProjectilePlayer, "TexturePacker/Sprites/Egg/Egg.plist", "egg", b2_dynamicBody, true );
//virtual 
void CGCObjProjectilePlayer::VOnResourceAcquire()
{
	IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjProjectilePlayer );
	CGCObjSpritePhysics::VOnResourceAcquire();
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjProjectilePlayer::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjProjectilePlayer::VOnUpdate( f32 fTimeStep )
{
	// changes to desired velocity
	Vec2 v2DesiredVelocityDelta = ( m_v2VelocityDesired - GetVelocity() );

	// 1) f = m * a -> a = f/m
	// 2) v = a * t -> a = v/t
	// so f/m = v/t -> f = (v/t) * m
	Vec2 v2RequiredForce = GetPhysicsBody()->GetMass() * ( ( 1.0f / fTimeStep ) * v2DesiredVelocityDelta );
	ApplyForceToCenter( v2RequiredForce );

	// handle lifetime
	m_fRemainingLifetime -= fTimeStep;
	if( m_fRemainingLifetime < 0.0f )
	{
		CGCObjectManager::ObjectKill( this );
	}
}
