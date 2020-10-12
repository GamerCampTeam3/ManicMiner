////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPENEMY_H_
#define	_GCOBJECTGROUPENEMY_H_

//#ifndef _GCTYPES_H_
//	#include "../../Core/GCTypes.h"
//#endif

//#ifndef __cocos2d_libs__CCValue__
	//#include "base/CCValue.h"
//#endif

//////////////////////////////////////////////////////////////////////////
// forward declare

namespace cocos2d
{
	class Sprite;
	class Animation;
	class ActionInterval;
}

#ifndef MATH_VEC2_H
	#include "cocos2d/cocos/math/Vec2.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif

//////////////////////////////////////////////////////////////////////////
// forward declare
class CGCObjSprite;
class CGCObjEnemy;
class CGCEnemyDataStore;

//////////////////////////////////////////////////////////////////////////
// responsible for newing, managing, & deleting the invaders
//
// This shows how an object group can be used as an allocation pool.
//
//////////////////////////////////////////////////////////////////////////
class CGCObjGroupEnemy
: public CGCObjectGroup
{
private:
	cocos2d::Vec2	m_v2FormationOrigin; // origin of the formation

	void	CreateEnemys	();
	void	DestroyEnemys	();

public:
	// number of invaders
	static const u32 k_uNumProjectiles = 1;

	CGCEnemyDataStore* c_pcGCEnemyDataStore;
	
	CGCObjGroupEnemy();		
	virtual ~CGCObjGroupEnemy() override;

	void SetFormationOrigin( cocos2d::Vec2 m_v2FormationOrigin );

//////////////////////////////////////////////////////////////////////////
// overrides for CGCObjectGroup public interface

	// handles GCObjEnemy
	virtual bool		VHandlesThisTypeId					( GCTypeID idQueryType ) override;

	// must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId							() override;

	virtual void		VOnGroupResourceAcquire				() override;
	virtual void		VOnGroupResourceAcquire_PostObject	() override;
	virtual void		VOnGroupResourceRelease				() override;

// CGCObjectGroup public interface
//////////////////////////////////////////////////////////////////////////

public:
	
	cocos2d::Animation* pAnimation1;
	cocos2d::Animation* pAnimation2;
	
};

#endif
