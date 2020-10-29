////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ManicMiner/Enemy/GCObjGroupEnemy.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "ManicMiner/Enemy/GCObjEnemy.h"

//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
CGCObjGroupEnemy::CGCObjGroupEnemy()
{
}
//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjGroupEnemy::~CGCObjGroupEnemy()
{
}
//////////////////////////////////////////////////////////////////////////
// Fucntion to query the class with a GCTypeID, returning true/false depending
// on if the class handles the GCTypeID.
//////////////////////////////////////////////////////////////////////////
//virtual 
bool CGCObjGroupEnemy::VHandlesThisTypeId( GCTypeID idQueryType )
{
	return( GetGCTypeIDOf( CGCObjEnemy ) == idQueryType );
}
//////////////////////////////////////////////////////////////////////////
// Vunction to release the resource this group manages.
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupEnemy::VOnGroupResourceRelease()
{
	// call parent class version
	CGCObjectGroup::VOnGroupResourceRelease();
	DestroyEnemies();
}
//////////////////////////////////////////////////////////////////////////
// Function to iterate the array of registerd CGCObjects, calling the supplied
// function then deleting them.
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupEnemy::DestroyEnemies()
{
	DestroyObjectsReverseOrder([&](CGCObject* pObject)
		{
			// do nothing - DestroyObjectsReverseOrder calls delete!
			GCASSERT(GetGCTypeIDOf(CGCObjEnemy) == pObject->GetGCTypeID(), "wrong type!");
		});
}
