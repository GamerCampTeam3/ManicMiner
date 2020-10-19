#ifndef _CMLTHECOLDROOM_H_
#define _CMLTHECOLDROOM_H_

#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/Enemy/GCObjGroupEnemyCentralCavern.h"

class CObjCCGroupPlatform;

class CMLTheColdRoom: public CManicLayer
{
public:
	CMLTheColdRoom();
	virtual ~CMLTheColdRoom();

	virtual	void VOnCreate( void )			override;
	virtual	void VOnDestroy(void)			override;

private:
	// object groups
	CGCObjGroupEnemyCentralCavern* m_pcGCGroupEnemyCentralCavern;
	CObjCCGroupPlatform* m_pcGroupPlatformCentralCavern;
};
#endif //#ifndef _CMLTHECOLDROOM_H_