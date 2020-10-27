////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPENEMYCENTRALCAVERN_H_
#define	_GCOBJECTGROUPENEMYCENTRALCAVERN_H_

#include "ManicMiner/Enemy/GCObjGroupEnemy.h"

//////////////////////////////////////////////////////////////////////////
//  This class is a sub class of the abstract base CGCObjGroupEnemy.  
//  Its purpose is to: 
//         - instantiate all enemies required for level 1 (Central Cavern).
//         - create and attach an animation loop to each enemy via operation
//           VOnGroupResourceAcquire_PostObject()
//         - Instantiate the static class s_cCreationParams_CGCObj_EDuck  for
//           each required enemy.  A reference to this parameter structure
//           is passed into the enemy on construction.
//  
//////////////////////////////////////////////////////////////////////////
class CGCObjGroupEnemyCentralCavern
: public CGCObjGroupEnemy
{
public:
	CGCObjGroupEnemyCentralCavern();		
	virtual ~CGCObjGroupEnemyCentralCavern			();
	virtual void VOnGroupResourceAcquire_PostObject	() override;
	virtual void VOnGroupResourceAcquire			() override;
	virtual GCTypeID VGetTypeId						() override;
};
#endif