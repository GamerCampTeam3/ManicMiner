////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ManicMiner/Hazards/GCObjGroupHazard.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "ManicMiner/Hazards/GCObjHazard.h"

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CGCObjGroupHazard::CGCObjGroupHazard()
{
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjGroupHazard::~CGCObjGroupHazard()
{
}

//////////////////////////////////////////////////////////////////////////
// Fucntion to query the class with a GCTypeID, returning true/false depending
// on if the class handles the GCTypeID.
//////////////////////////////////////////////////////////////////////////
//virtual 
bool CGCObjGroupHazard::VHandlesThisTypeId( GCTypeID idQueryType )
{
	return( GetGCTypeIDOf( CGCObjHazard ) == idQueryType );
}

//////////////////////////////////////////////////////////////////////////
// Base class virtual function.
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupHazard::VOnGroupResourceAcquire()
{
	// call parent class version
	CGCObjectGroup::VOnGroupResourceAcquire();
}

//////////////////////////////////////////////////////////////////////////
// Base class virtual function.
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupHazard::VOnGroupResourceAcquire_PostObject()
{
	// call parent class version
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();
}

//////////////////////////////////////////////////////////////////////////
// Base class function to release the resource this group manages.
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupHazard::VOnGroupResourceRelease()
{
	// call parent class version
	CGCObjectGroup::VOnGroupResourceRelease();

	DestroyHazards();

}

//////////////////////////////////////////////////////////////////////////
// Function to iterate the array of registerd CGCObjects, calling the supplied
// function then deleting them.
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupHazard::DestroyHazards()
{
	DestroyObjectsReverseOrder([&](CGCObject* pObject)
		{
			// do nothing - DestroyObjectsReverseOrder calls delete!
			GCASSERT(GetGCTypeIDOf(CGCObjHazard) == pObject->GetGCTypeID(), "wrong type!");
		});
}