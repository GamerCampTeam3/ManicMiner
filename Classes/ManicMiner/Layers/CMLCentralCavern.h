#ifndef _CMLCENTRALCAVERN_H_
#define _CMLCENTRALCAVERN_H_

#include "ManicMiner/Layers/CManicLayer.h"

class CMLCentralCavern: public CManicLayer
{
public:
	CMLCentralCavern();
	virtual ~CMLCentralCavern();

	virtual	void VOnCreate( void )			override;

};
#endif //#ifndef _CMANICLAYER_H_