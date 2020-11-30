#ifndef _CMLTHEMENAGERIE_H_
#define _CMLTHEMENAGERIE_H_

#include "ManicMiner/Layers/CManicLayer.h"

class CMLTheMenagerie : public CManicLayer
{
public:
	// Constructor -------------------------------------------------------------------------------------------------------- //
	CMLTheMenagerie();
	// Destructor --------------------------------------------------------------------------------------------------------- //
	virtual ~CMLTheMenagerie();

	// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
	// VOnCreate will define this level's specific layout ----------------------------------------------------------------- //
	virtual	void VOnCreate( void ) override;																			//
// VOnDestroy needs to cleanup the unique elements to this level ------------------------------------------------------ //
	virtual	void VOnDestroy( void ) override;																			//
// -------------------------------------------------------------------------------------------------------------------- //
};
#endif //#ifndef _CMLTHEMENAGERIE_H_