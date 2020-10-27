#include "CDoorCreator.h"

// Handles the CDoor objects

GCTypeID CDoorCreator::VGetTypeId()
{
	return GetGCTypeIDOf( CDoor );
}

bool CDoorCreator::VHandlesThisTypeId(GCTypeID idQueryType)
{
	return(GetGCTypeIDOf( CDoor ) == idQueryType);
}

// On Resource Acquire, create the door.
void CDoorCreator::VOnGroupResourceAcquire()
{
	CGCObjectGroup::VOnGroupResourceAcquire();
	CreateDoor();
}

void CDoorCreator::VOnGroupResourceAcquire_PostObject()
{
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();
}

// On Delete, destroy objects
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