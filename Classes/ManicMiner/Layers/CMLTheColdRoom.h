// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	CMLTheColdRoom is a simplified version of CMLCentralCavern											//
//					This level exists only to showcase the capability of loading a second level							//
//					Through the CLevelManager																			//
//																														//
// See also		:	CMLCentralCavern & CManicLayer & CLevelManager														//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _CMLTHECOLDROOM_H_
#define _CMLTHECOLDROOM_H_

#include "ManicMiner/Layers/CManicLayer.h"

class CHUD;
class CAirManager;

class CMLTheColdRoom: public CManicLayer
{
public:
// Constructor -------------------------------------------------------------------------------------------------------- //
	CMLTheColdRoom();
// Destructor --------------------------------------------------------------------------------------------------------- //
	virtual ~CMLTheColdRoom();

// CManicLayer Overrides ---------------------------------------------------------------------------------------------- //
// VOnCreate will define this level's specific layout ----------------------------------------------------------------- //
	virtual	void VOnCreate	( void ) override;																			//
// VOnDestroy needs to cleanup the unique elements to this level ------------------------------------------------------ //
	virtual	void VOnDestroy	( void ) override;																			//
// -------------------------------------------------------------------------------------------------------------------- //

private:
	// --- HUD ------------------------------------------------------------------------------------------------------------ //
	CHUD* m_pCHUD;

	// --- Air Manager ---------------------------------------------------------------------------------------------------- //
	CAirManager* m_pcAirManager;

	virtual void VInitParams() override;
};
#endif //#ifndef _CMLTHECOLDROOM_H_