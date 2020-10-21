#ifndef _CMLCENTRALCAVERN_H_
#define _CMLCENTRALCAVERN_H_

#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/Enemy/CentralCavern/GCObjGroupEnemyCentralCavern.h"
#include "ManicMiner/Hazards/CentralCavern/GCObjGroupHazardCentralCavern.h"

class CCGCentralCavern;
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
	CCGCentralCavern* m_pcCollectiblesGroupCentralCavern;
};
#endif //#ifndef _CMANICLAYER_H_