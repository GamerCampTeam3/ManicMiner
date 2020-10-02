////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJPLAYERPROJECTILE_H_
#define _GCOBJPLAYERPROJECTILE_H_

#ifndef _GCOBJSPRITEPHYSICS_H_
	#include "../../GCCocosInterface/GCObjSpritePhysics.h"
#endif


//////////////////////////////////////////////////////////////////////////
// This is a sample class derived from CGCObject.
// 
// It demonstrates how you might do projectiles, it relies on 
// CGCObjGroupProjectilePlayer to manage the lifetimes of these.
//
//////////////////////////////////////////////////////////////////////////
class CGCObjProjectilePlayer 
: public CGCObjSpritePhysics
{
private:
	cocos2d::Vec2	m_v2VelocityDesired;	// velocity of the projectile
	f32				m_fRemainingLifetime;	// remaining lifetime

public:
	CGCObjProjectilePlayer();

	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjProjectilePlayer()
	{}

	// accessors for data members
	inline void SetVelocity( cocos2d::Vec2 v2Velocity );
	inline void SetLifeTime( f32 fLifeTime );

	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface

		virtual void VOnResourceAcquire	() override;

		virtual void VOnReset			() override;

		virtual void VOnUpdate			( f32 fTimeStep ) override;

	// overridden virtuals from the game object interface
	//////////////////////////////////////////////////////////////////////////
};



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline void CGCObjProjectilePlayer::SetVelocity( cocos2d::Vec2 v2Velocity )
{
	m_v2VelocityDesired = v2Velocity;
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline void CGCObjProjectilePlayer::SetLifeTime( f32 fLifeTime )
{
	m_fRemainingLifetime = fLifeTime;
}


#endif // #ifndef _GCOBJPLAYERPROJECTILE_H_