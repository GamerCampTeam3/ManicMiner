#ifndef _CMLABANDONEDURANIUMWORKINS_H_
#define _CMLABANDONEDURANIUMWORKINS_H_

#include "ManicMiner/Layers/CManicLayer.h"

class CMLAbandonedUraniumWorkings : public CManicLayer
{
public:
	// Constructor -------------------------------------------------------------------------------------------------------- //
	CMLAbandonedUraniumWorkings();
	// Destructor --------------------------------------------------------------------------------------------------------- //
	virtual ~CMLAbandonedUraniumWorkings();

	// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
	// VOnCreate will define this level's specific layout ----------------------------------------------------------------- //
	virtual	void VOnCreate( void ) override;																			//
// VOnDestroy needs to cleanup the unique elements to this level ------------------------------------------------------ //
	virtual	void VOnDestroy( void ) override;																			//
// -------------------------------------------------------------------------------------------------------------------- //
};
#endif //#ifndef _CMLABANDONEDURANIUMWORKINS_H_