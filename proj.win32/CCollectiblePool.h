#ifndef	_CCOLLECTIBLEPOOL_H_
#define	_CCOLLECTIBLEPOOL_H_

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
class CCollectible;


//////////////////////////////////////////////////////////////////////////
// responsible for newing, managing, & deleting the player's projectiles
//
// This shows how an object group can be used as an allocation pool.
//
//////////////////////////////////////////////////////////////////////////
class CCollectiblePool
	: public CGCObjectGroup
{
public:
	// Max number of collectibles to generate
	static const u32 k_uNumberOfCollectibles = 5;

	CCollectiblePool();
	virtual ~CCollectiblePool()  override;

	// called from CGCGameLayerSpaceInvaders::VOnCreate
	void				CreateCollectible();
	void				DestroyCollectible();

	void				SpawnCollectible(cocos2d::Vec2 v2Position);

	//////////////////////////////////////////////////////////////////////////
	// overrides for CGCObjectGroup public interface

		// handles GCObjInvader
	virtual bool		VHandlesThisTypeId(GCTypeID idQueryType) override;

	// must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId() override;

	virtual void		VOnGroupResourceAcquire() override;
	virtual void		VOnGroupResourceRelease() override;

	// CGCObjectGroup public interface
	//////////////////////////////////////////////////////////////////////////
};

#endif
