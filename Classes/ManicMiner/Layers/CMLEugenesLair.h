// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	CMLCentralCavern is a child of CManicLayer, the first level of Manic Miner							//
//					We needs these children classes to define the layout of each level									//
//					Positioning enemies, hazards, player, platforms, etc												//
//																														//
// See also		:	CManicLayer & CLevelManager																			//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _CMLEUGENESLAIR_H_
#define _CMLEUGENESLAIR_H_

#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/Enemy/GCObjEugene.h"

// ----------------------------------------- Fwd declares ------------------------------------------------------------- //
class CAirManager;																										//
class CHUD;																												//
// -------------------------------------------------------------------------------------------------------------------- //

class CMLEugenesLair : public CManicLayer
{
public:
	// Constructor -------------------------------------------------------------------------------------------------------- //
	CMLEugenesLair();
	// Destructor --------------------------------------------------------------------------------------------------------- //
	virtual ~CMLEugenesLair();
	// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
	// VOnCreate will define this level's specific layout ----------------------------------------------------------------- //
	virtual	void VOnCreate( void ) override;																			//
// VOnDestroy needs to cleanup the unique elements to this level ------------------------------------------------------ //
	virtual	void VOnDestroy( void ) override;																			//
// -------------------------------------------------------------------------------------------------------------------- //

public:

private:

	// --- HUD ------------------------------------------------------------------------------------------------------------ //
	CHUD* m_pCHUD;

	// --- Air Manager ---------------------------------------------------------------------------------------------------- //
	CAirManager* m_pcAirManager;

	virtual void VInitParams() override;
};
#endif //#ifndef _CMLEUGENESLAIR_H_