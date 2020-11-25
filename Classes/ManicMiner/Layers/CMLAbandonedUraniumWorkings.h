#ifndef _CMLABANDONEDURANIUMWORKINS_H_
#define _CMLABANDONEDURANIUMWORKINS_H_

#include "ManicMiner/Layers/CManicLayer.h"

class CHUD;
class CAirManager;

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

private:
	// --- HUD ------------------------------------------------------------------------------------------------------------ //
	CHUD* m_pCHUD;

	// --- Air Manager ---------------------------------------------------------------------------------------------------- //
	CAirManager* m_pcAirManager;

	virtual void VInitParams() override;
};
#endif //#ifndef _CMLABANDONEDURANIUMWORKINS_H_