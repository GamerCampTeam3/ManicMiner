#ifndef _CMLTHECOLDROOM_H_
#define _CMLTHECOLDROOM_H_

#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/Enemy/CentralCavern/GCObjGroupEnemyCentralCavern.h"

class CObjCCGroupPlatform;
class CGCObjGroupHazardCentralCavern;

class CMLTheColdRoom: public CManicLayer
{
public:
	CMLTheColdRoom();
	virtual ~CMLTheColdRoom();

	virtual	void VOnCreate( void )			override;
	virtual	void VOnDestroy(void)			override;

private:
	// object groups
	//CGCObjGroupEnemyCentralCavern* m_pcGCGroupEnemyCentralCavern;
	CObjCCGroupPlatform* m_pcGroupPlatformCentralCavern;
	CGCObjGroupHazardCentralCavern* m_pcGCGroupHazardCentralCavern;

};
#endif //#ifndef _CMLTHECOLDROOM_H_