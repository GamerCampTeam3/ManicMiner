// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	CMLCentralCavern is a child of CManicLayer, the first level of Manic Miner							//
//					We needs these children classes to define the layout of each level									//
//					Positioning enemies, hazards, player, platforms, etc												//
//																														//
// See also		:	CManicLayer & CLevelManager																			//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _CMLSOLARPOWERGENERATOR_H_
#define _CMLSOLARPOWERGENERATOR_H_

#include "ManicMiner/Layers/CManicLayer.h"

class CSolarLight;

class CMLSolarPowerGenerator : public CManicLayer
{
public:
	// Constructor -------------------------------------------------------------------------------------------------------- //
	CMLSolarPowerGenerator();
	// Destructor --------------------------------------------------------------------------------------------------------- //
	virtual ~CMLSolarPowerGenerator();

	// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
	// VOnCreate will define this level's specific layout ----------------------------------------------------------------- //
	virtual	void VOnCreate( void ) override;																			//
// VOnDestroy needs to cleanup the unique elements to this level ------------------------------------------------------ //
	virtual	void VOnDestroy( void ) override;																			//
// -------------------------------------------------------------------------------------------------------------------- //
	virtual void VOnUpdate( f32 fTimeStep )	override;
private:
	CSolarLight* m_pcSolarLight;
};
#endif //#ifndef _CMLSOLARPOWERGENERATOR_H_