#include "CDCreatorCentralCavern.h"

#include "ManicMiner/Doors/CDoor.h"
#include "CDoorCreator.h"


// News a door at wanted location
void CDCreatorCentralCavern::CreateDoor()
{
	// CDoor* CentralCavernDoor = new CDoor( m_rManicLayer, m_cCreationParams, m_v2Position);

}

void CDCreatorCentralCavern::DeleteDoor()
{
	DestroyObjectsReverseOrder( [&]( CGCObject* pObject )
	{
		// do nothing - DestroyObjectsReverseOrder calls delete!
		GCASSERT( GetGCTypeIDOf( CDoor ) == pObject->GetGCTypeID(), "wrong type!" );

	} );
}

void CDCreatorCentralCavern::VOnGroupResourceRelease()
{
	CGCObjectGroup::VOnGroupResourceRelease();
	DeleteDoor();
}