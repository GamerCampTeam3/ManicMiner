#ifndef _CDCREATORCENTRALCAVERN_H_
#define _CDCREATORCENTRALCAVERN_H_


#include "CDoorCreator.h"

class CDCreatorCentralCavern
	: public CDoorCreator
{
public:
	CDCreatorCentralCavern(CManicLayer& cLayer)
		: CDoorCreator(cLayer)
		, m_cCreationParams( "Door", "TexturePacker/Sprites/Door/Door.plist", "Door", b2_staticBody, true )
		, m_rManicLayer( cLayer )
	{
	}


private:

	void CreateDoor() override;
	void DeleteDoor();
	void VOnGroupResourceRelease() override;

	CGCFactoryCreationParams m_cCreationParams;
	CManicLayer& m_rManicLayer;
};



#endif // #ifndef _CDCREATORCENTRALCAVERN_H_



