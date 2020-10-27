////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPHAZARDCENTRALCAVERN_H_
#define	_GCOBJECTGROUPHAZARDCENTRALCAVERN_H_

#include "ManicMiner/Hazards/GCObjGroupHazard.h"

//////////////////////////////////////////////////////////////////////////
//  This class is a sub class of the abstract base CGCObjGroupHazard.  
//  Its purpose is to: 
//         - instantiate all hazards required for level 1 (Central Cavern).
//         - Instantiate the static class s_cCreationParams_CGCObj_EBush and 
//           s_cCreationParams_CGCObj_EDownwardSpike for each required
//           hazard.  A reference to this parameter structure is passed into 
//           the hazard on construction.
//////////////////////////////////////////////////////////////////////////
class CGCObjGroupHazardCentralCavern
: public CGCObjGroupHazard
{

public:
	CGCObjGroupHazardCentralCavern			();		
	virtual ~CGCObjGroupHazardCentralCavern	();
	virtual void VOnGroupResourceAcquire	() override;
	virtual GCTypeID VGetTypeId				() override;

};
#endif