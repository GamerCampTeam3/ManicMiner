////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPENEMY_H_
#define	_GCOBJECTGROUPENEMY_H_

#ifndef	_GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif

//////////////////////////////////////////////////////////////////////////
//  This class is a sub class of CGCObjGroup.
//  Its purpose is to provide a base class from which each of the Manic Miner
//  levels can be sub clased from, due to there being some common functionality
//  between the levels (destroying enemies and the function VHandlesThisTypeId)
//////////////////////////////////////////////////////////////////////////

class CGCObjGroupEnemy
: public CGCObjectGroup
{
private:
	
	void DestroyEnemies	();

public:
	
	CGCObjGroupEnemy			();		
	virtual ~CGCObjGroupEnemy	() override;

	// handles GCObjEnemy
	virtual bool VHandlesThisTypeId ( GCTypeID idQueryType ) override;
	
	virtual void VOnGroupResourceAcquire			() override;
	virtual void VOnGroupResourceAcquire_PostObject	() override;
	virtual void VOnGroupResourceRelease			() override;

};
#endif