// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	CMLCentralCavern is a child of CManicLayer, the first level of Manic Miner							//
//					We needs these children classes to define the layout of each level									//
//					Positioning enemies, hazards, player, platforms, etc												//
//																														//
// See also		:	CManicLayer & CLevelManager																			//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _CMLRETURNOFKONG_H_
#define _CMLRETURNOFKONG_H_

#include "ManicMiner/Layers/CManicLayer.h"

class CGCObjKong;

// ----------------------------------------- Fwd declares ------------------------------------------------------------- //
class CAirManager;																										//
class CHUD;																												//
// -------------------------------------------------------------------------------------------------------------------- //

class CMLReturnOfKong : public CManicLayer
{
public:
	// Constructor -------------------------------------------------------------------------------------------------------- //
	CMLReturnOfKong();
	// Destructor --------------------------------------------------------------------------------------------------------- //
	virtual ~CMLReturnOfKong();

	// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
	// VOnCreate will define this level's specific layout ----------------------------------------------------------------- //
	virtual	void VOnCreate( void ) override;																			//
// VOnDestroy needs to cleanup the unique elements to this level ------------------------------------------------------ //
	virtual	void VOnDestroy( void ) override;																			//
	void VLevelSpecificInteraction() override;
// -------------------------------------------------------------------------------------------------------------------- //
private:
	CGCObjKong* m_pcKong;
};
#endif //#ifndef _CMLRETURNOFKONG_H_