// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	CMLCentralCavern is a child of CManicLayer, the first level of Manic Miner							//
//					We needs these children classes to define the layout of each level									//
//					Positioning enemies, hazards, player, platforms, etc												//
//																														//
// See also		:	CManicLayer & CLevelManager																			//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _CMLWILLYMEETSKONG_H_
#define _CMLWILLYMEETSKONG_H_

#include "ManicMiner/Layers/CManicLayer.h"

class CGCObjKong;
class CMovingDoor;

class CMLWillyMeetsKong : public CManicLayer
{
public:
	// Constructor -------------------------------------------------------------------------------------------------------- //
	CMLWillyMeetsKong();
	// Destructor --------------------------------------------------------------------------------------------------------- //
	virtual ~CMLWillyMeetsKong();

	// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
	// VOnCreate will define this level's specific layout ----------------------------------------------------------------- //
	virtual	void VOnCreate( void ) override;																			//
// VOnDestroy needs to cleanup the unique elements to this level ------------------------------------------------------ //
	virtual	void VOnDestroy( void ) override;																			//
// -------------------------------------------------------------------------------------------------------------------- //
	void VLevelSpecificInteraction() override;

private:
	CMovingDoor* m_pcMovingDoor;
	CGCObjKong* m_pcKong;
};
#endif //#ifndef _CMLWILLYMEETSKONG_H_