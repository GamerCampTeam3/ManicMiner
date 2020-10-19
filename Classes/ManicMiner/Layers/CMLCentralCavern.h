#ifndef _CMLCENTRALCAVERN_H_
#define _CMLCENTRALCAVERN_H_

#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/Enemy/CentralCavern/GCObjGroupEnemyCentralCavern.h"
#include "ManicMiner/Hazards/CentralCavern/GCObjGroupHazardCentralCavern.h"

class CObjCCGroupPlatform;

class CMLCentralCavern: public CManicLayer
{
public:
	CMLCentralCavern();
	virtual ~CMLCentralCavern();

	virtual	void VOnCreate( void )			override;
	virtual	void VOnDestroy(void)			override;

private:
	// object groups
	CGCObjGroupEnemyCentralCavern* m_pcGCGroupEnemyCentralCavern;
	CGCObjGroupHazardCentralCavern* m_pcGCGroupHazardCentralCavern;
	CObjCCGroupPlatform* m_pcGroupPlatformCentralCavern;
	CCollectiblesGroup* m_pcCollectibleGroup;
};
#endif //#ifndef _CMANICLAYER_H_