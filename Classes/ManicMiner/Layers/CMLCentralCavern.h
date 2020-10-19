#ifndef _CMLCENTRALCAVERN_H_
#define _CMLCENTRALCAVERN_H_

#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/Enemy/CentralCavern/GCObjGroupEnemyCentralCavern.h"

class CObjCCGroupPlatform;

class CMLCentralCavern: public CManicLayer
{
public:
	CMLCentralCavern();
	virtual ~CMLCentralCavern();

	virtual	void VOnCreate( void )			override;
	virtual	void VOnDestroy(void)			override;

	// object groups
	CGCObjGroupEnemyCentralCavern* m_pcGCGroupEnemyCentralCavern;
	CObjCCGroupPlatform* m_pcGroupPlatformCentralCavern;
};
#endif //#ifndef _CMANICLAYER_H_