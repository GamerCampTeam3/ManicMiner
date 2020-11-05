////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJGROUPPROJECTILEPLAYER_H_
#define	_GCOBJGROUPPROJECTILEPLAYER_H_

#ifndef MATH_VEC2_H
	#include "cocos2d/cocos/math/Vec2.h"
#endif

#ifndef BOX2D_H
	#include "Box2d/Box2D.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif


//////////////////////////////////////////////////////////////////////////
// forward declare
class CGCObjProjectilePlayer;


//////////////////////////////////////////////////////////////////////////
// responsible for newing, managing, & deleting the player's projectiles
//
// This shows how an object group can be used as an allocation pool.
//
//////////////////////////////////////////////////////////////////////////
class CGCObjGroupProjectilePlayer
: public CGCObjectGroup
{
public:
	// number of invaders
	static const u32 k_uNumInvaders = 16;

	CGCObjGroupProjectilePlayer			();		
	virtual ~CGCObjGroupProjectilePlayer()  override;

	// called from CGCGameLayerSpaceInvaders::VOnCreate
	void				CreateProjectiles	();
	void				DestroyProjectiles	();

	void				SpawnProjectile		( cocos2d::Vec2 v2Position, cocos2d::Vec2 v2Velocity, f32 fMaxLifeTime );

//////////////////////////////////////////////////////////////////////////
// overrides for CGCObjectGroup public interface

	// handles GCObjInvader
	virtual bool		VHandlesThisTypeId	( GCTypeID idQueryType ) override;

	// must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId				() override;

	virtual void		VOnGroupResourceAcquire	() override;	
	virtual void		VOnGroupResourceRelease	() override;	

// CGCObjectGroup public interface
//////////////////////////////////////////////////////////////////////////
};

#endif
