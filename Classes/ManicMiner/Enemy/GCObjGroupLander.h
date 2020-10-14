////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPLANDER_H_
#define	_GCOBJECTGROUPLANDER_H_

//#ifndef _GCTYPES_H_
//	#include "../../Core/GCTypes.h"
//#endif

//#ifndef __cocos2d_libs__CCValue__
	//#include "base/CCValue.h"
//#endif

//////////////////////////////////////////////////////////////////////////
// forward declare

#ifndef MATH_VEC2_H
	#include "cocos2d/cocos/math/Vec2.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif

#include "ManicMiner/Enums/EEnemyTypes.h"
#include <vector>


//////////////////////////////////////////////////////////////////////////
// forward declare
class CGCObjSprite;
class CGCObjLander;

//////////////////////////////////////////////////////////////////////////
// responsible for newing, managing, & deleting the invaders
//
// This shows how an object group can be used as an allocation pool.
//
//////////////////////////////////////////////////////////////////////////
class CGCObjGroupLander
: public CGCObjectGroup
{
private:
	cocos2d::Vec2	m_v2FormationOrigin; // origin of the formation

	void	CreateLanders	();
	void	DestroyLanders	();
		
	void AllocateRandomLander(int iNumberToAllocate);
	void KillLandersAccordingToLanderActivators();

	bool bFirstPassDone;

	const int k_iMaxLanderAnchorPoints = 10;
	const int k_iMaxActiveLanders = 5;
	const int k_iAnchorPointStepDistance = 180;
	const int k_iAnchorPointYPosition = 1000;
	
	std::vector<bool> vLanderActivators;
	std::vector<cocos2d::Vec2> vLanderAnchorPoints;

public:
	
	CGCObjGroupLander();		
	virtual ~CGCObjGroupLander() override;

	void SetFormationOrigin( cocos2d::Vec2 m_v2FormationOrigin );

//////////////////////////////////////////////////////////////////////////
// overrides for CGCObjectGroup public interface

	// handles GCObjLander
	virtual bool		VHandlesThisTypeId					( GCTypeID idQueryType ) override;

	// must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId							() override;

	virtual void		VOnGroupResourceAcquire				() override;
	virtual void		VOnGroupResourceAcquire_PostObject	() override;
	virtual void		VOnGroupResourceRelease				() override;
	virtual void		VOnGroupUpdate						(f32 fTimeStep) override;
	virtual void		VOnGroupReset()						override;

// CGCObjectGroup public interface
//////////////////////////////////////////////////////////////////////////

};

#endif