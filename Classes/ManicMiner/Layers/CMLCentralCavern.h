// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	CMLCentralCavern is a child of CManicLayer, the first level of Manic Miner							//
//					We needs these children classes to define the layout of each level									//
//					Positioning enemies, hazards, player, platforms, etc												//
//																														//
// See also		:	CManicLayer & CLevelManager																			//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _CMLCENTRALCAVERN_H_
#define _CMLCENTRALCAVERN_H_

#include "ManicMiner/Layers/CManicLayer.h"
// ----------------------------------------- Fwd declares ------------------------------------------------------------- //
class CAirManager;																										//
class CHUD;																												//
// -------------------------------------------------------------------------------------------------------------------- //

class CMLCentralCavern: public CManicLayer
{
public:
// Constructor -------------------------------------------------------------------------------------------------------- //
	CMLCentralCavern();
// Destructor --------------------------------------------------------------------------------------------------------- //
	virtual ~CMLCentralCavern();

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate will define this level's specific layout ----------------------------------------------------------------- //
	virtual	void VOnCreate	( void ) override;																			//
// VOnDestroy needs to cleanup the unique elements to this level ------------------------------------------------------ //
	virtual	void VOnDestroy	( void ) override;																			//
// -------------------------------------------------------------------------------------------------------------------- //

private:

// --- HUD ------------------------------------------------------------------------------------------------------------ //
	CHUD*							m_pCHUD;

// --- Air Manager ---------------------------------------------------------------------------------------------------- //
	CAirManager*					m_pcAirManager;

	virtual void InitParams() override;
};
#endif //#ifndef _CMANICLAYER_H_