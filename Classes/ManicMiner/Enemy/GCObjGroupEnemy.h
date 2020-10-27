////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPENEMY_H_
#define	_GCOBJECTGROUPENEMY_H_

#ifndef	_GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif

//////////////////////////////////////////////////////////////////////////
//  This abstract class is a sub class of CGCObjGroup.
//  Its purpose is to provide a base class from which each of the Manic Miner
//  levels can be sub clased from, due to there being some common functionality
//  between the levels (destroying enemies and the function VHandlesThisTypeId).
//  This is an abstract class and cannot be instantiated.  Derived sub classes
//  must implement pure virtual functions VOnGroupResourceAcquire and 
//  VOnGroupResourceAcquire_PostObject
//////////////////////////////////////////////////////////////////////////

class CGCObjGroupEnemy
: public CGCObjectGroup
{
private:
	void DestroyEnemies	();

public:
	CGCObjGroupEnemy								();		
	virtual ~CGCObjGroupEnemy						() override;
	virtual void VOnGroupResourceAcquire_PostObject	() = 0;
	virtual void VOnGroupResourceAcquire			() = 0;
	virtual bool VHandlesThisTypeId					( GCTypeID idQueryType ) override;
	virtual void VOnGroupResourceRelease			() override;
};
#endif