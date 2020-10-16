////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPENEMYCENTRALCAVERN_H_
#define	_GCOBJECTGROUPENEMYCENTRALCAVERN_H_

//#ifndef _GCTYPES_H_
//	#include "../../Core/GCTypes.h"
//#endif

//#ifndef __cocos2d_libs__CCValue__
	//#include "base/CCValue.h"
//#endif

//////////////////////////////////////////////////////////////////////////
// forward declare

//namespace cocos2d
//{
	//class Sprite;
	//class Animation;
	//class ActionInterval;
//}

#ifndef MATH_VEC2_H
	#include "cocos2d/cocos/math/Vec2.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif

#include "ManicMiner/Enemy/GCObjGroupEnemy.h"
#include "ManicMiner/Enums/EEnemyTypes.h"
#include <unordered_map>

//////////////////////////////////////////////////////////////////////////
// forward declare
class CGCObjSprite;
//class CGCObjEnemy;
class CGCObjGroupEnemy;

//////////////////////////////////////////////////////////////////////////
// responsible for newing, managing, & deleting the invaders
//
// This shows how an object group can be used as an allocation pool.
//
//////////////////////////////////////////////////////////////////////////
class CGCObjGroupEnemyCentralCavern
: public CGCObjGroupEnemy
{
private:
    void CreateEnemies();

public:
	CGCObjGroupEnemyCentralCavern();		
	virtual ~CGCObjGroupEnemyCentralCavern();

	virtual void VOnGroupResourceAcquire_PostObject() override;
	virtual void VOnGroupResourceAcquire() override;

};

#endif