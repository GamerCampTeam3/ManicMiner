#include "CDCreatorCentralCavern.h"
#include "CDoorCreator.h"
#include "ManicMiner/Doors/CDoor.h"

void CDCreatorCentralCavern::CreateDoor()
{
	CDoor* CentralCavernDoor = new CDoor( m_rManicLayer, m_cCreationParams, cocos2d::Vec2( 1800.f, 120.f ));

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


