////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPHAZARDCENTRALCAVERN_H_
#define	_GCOBJECTGROUPHAZARDCENTRALCAVERN_H_

#include "ManicMiner/Hazards/GCObjGroupHazard.h"

//////////////////////////////////////////////////////////////////////////
//  This class is a sub class of the base CGCObjGroupHazard.  
//  
//////////////////////////////////////////////////////////////////////////
class CGCObjGroupHazardCentralCavern
: public CGCObjGroupHazard
{

public:
	CGCObjGroupHazardCentralCavern();		
	virtual ~CGCObjGroupHazardCentralCavern();

	virtual void VOnGroupResourceAcquire_PostObject() override;
	virtual void VOnGroupResourceAcquire() override;

	virtual GCTypeID VGetTypeId() override;


};
#endif