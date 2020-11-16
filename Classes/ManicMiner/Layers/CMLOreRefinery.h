// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	CMLCentralCavern is a child of CManicLayer, the first level of Manic Miner							//
//					We needs these children classes to define the layout of each level									//
//					Positioning enemies, hazards, player, platforms, etc												//
//																														//
// See also		:	CManicLayer & CLevelManager																			//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _CMLOREREFINERY_H_
#define _CMLOREREFINERY_H_

#include "ManicMiner/Layers/CManicLayer.h"
// ----------------------------------------- Fwd declares ------------------------------------------------------------- //
class CAirManager;																										//
class CHUD;																												//
// -------------------------------------------------------------------------------------------------------------------- //

class CMLOreRefinery : public CManicLayer
{
public:
	// Constructor -------------------------------------------------------------------------------------------------------- //
	CMLOreRefinery();
	// Destructor --------------------------------------------------------------------------------------------------------- //
	virtual ~CMLOreRefinery();

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

	virtual void InitParams() override;
};
#endif //#ifndef _CMLOREREFINERY_H_