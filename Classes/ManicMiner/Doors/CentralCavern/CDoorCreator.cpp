#include "CDoorCreator.h"

void CDoorCreator::CreateDoor()
{
	
}

GCTypeID CDoorCreator::VGetTypeId()
{
	return GetGCTypeIDOf( CDoor );
}

bool CDoorCreator::VHandlesThisTypeId(GCTypeID idQueryType)
{
	return(GetGCTypeIDOf( CDoor ) == idQueryType);
}

void CDoorCreator::VOnGroupResourceAcquire()
{
	CGCObjectGroup::VOnGroupResourceAcquire();
	CreateDoor();
}

void CDoorCreator::VOnGroupResourceAcquire_PostObject()
{
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();
}

void CDoorCreator::VOnGroupResourceRelease()
{
	CGCObjectGroup::VOnGroupResourceRelease();
	DestroyItems();
	
}


void CDoorCreator::DestroyItems()
{
	DestroyObjectsReverseOrder( [&]( CGCObject* pObject )
	{
		// do nothing - DestroyObjectsReverseOrder calls delete!
		GCASSERT( GetGCTypeIDOf( CDoor ) == pObject->GetGCTypeID(), "wrong type!" );
	} );
}







// static CGCFactoryCreationParams s_cCreationParamsKey( "Key", "TexturePacker/Sprites/Key/Key.plist", "Key", b2_staticBody, true );