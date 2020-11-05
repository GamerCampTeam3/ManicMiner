////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPINVADER_H_
#define	_GCOBJECTGROUPINVADER_H_

#ifndef MATH_VEC2_H
	#include "cocos2d/cocos/math/Vec2.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif


//////////////////////////////////////////////////////////////////////////
// forward declare
class CGCObjSprite;
class CGCObjInvader;


//////////////////////////////////////////////////////////////////////////
// responsible for newing, managing, & deleting the invaders
//
// This shows how an object group can be used as an allocation pool.
//
//////////////////////////////////////////////////////////////////////////
class CGCObjGroupInvader
: public CGCObjectGroup
{
private:
	cocos2d::Vec2	m_v2FormationOrigin; // origin of the formation

	void	CreateInvaders	();
	void	DestroyInvaders	();

public:
	// number of invaders
	static const u32 k_uNumProjectiles = 16;

	CGCObjGroupInvader();		
	virtual ~CGCObjGroupInvader() override;

	void SetFormationOrigin( cocos2d::Vec2 m_v2FormationOrigin );

//////////////////////////////////////////////////////////////////////////
// overrides for CGCObjectGroup public interface

	// handles GCObjInvader
	virtual bool		VHandlesThisTypeId					( GCTypeID idQueryType ) override;

	// must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId							() override;

	virtual void		VOnGroupResourceAcquire_PostObject	() override;
	virtual void		VOnGroupResourceRelease				() override;

// CGCObjectGroup public interface
//////////////////////////////////////////////////////////////////////////
};

#endif
