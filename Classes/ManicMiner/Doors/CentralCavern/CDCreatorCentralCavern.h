//////////////////
/// Bib
//////////////////

#ifndef _CDCREATORCENTRALCAVERN_H_
#define _CDCREATORCENTRALCAVERN_H_

#include "CDoorCreator.h"

class CDCreatorCentralCavern
	: public CDoorCreator
{
public:
	// Constructor that feeds in the references to the creator parent
	CDCreatorCentralCavern(CManicLayer& cLayer)
		: CDoorCreator		(cLayer)
		, m_cCreationParams	( "Door", "TexturePacker/Sprites/Door/Door.plist", "Door", b2_staticBody, true )
		, m_rManicLayer		( cLayer )
		, m_v2Position		( 1800.f, 120.f)
	{
	}

private:
	void CreateDoor() override;					// Overriden function to spawn the CDoor in.	
	void DeleteDoor();							// Called on Resource release, deletes
	void VOnGroupResourceRelease() override;	// Releases the resources

	CGCFactoryCreationParams m_cCreationParams;	// CreationParams that will set the CDoor
	CManicLayer& m_rManicLayer;					// and a reference to the manic layer that this creator is instantiated from
	cocos2d::Vec2 m_v2Position;					// the starting (and this reset) position of the CDoor.
};

#endif // #ifndef _CDCREATORCENTRALCAVERN_H_