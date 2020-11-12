//////////////////
/// Bib
//////////////////

#ifndef _CDOORCREATOR_H_
#define _CDOORCREATOR_H_

#include "GamerCamp/GCObject/GCObjectGroup.h"
#include "ManicMiner/Doors/CDoor.h"


class CManicLayer;

class CDoorCreator
	: public CGCObjectGroup
{
protected:
	// The door to is spawned by the door creator.
	CManicLayer* m_rManicLayer;

	// Virtual function that is overriden by child of this class to spawn the door in.
	virtual void CreateDoor() {};
	
public:
	CDoorCreator( CManicLayer& cLayer)
		: m_rManicLayer( nullptr )
	{		
	}

	// Virtual functions
	virtual void		DestroyItems( void );
	virtual bool		VHandlesThisTypeId( GCTypeID idQueryType );
	virtual GCTypeID	VGetTypeId( void );
	virtual void		VOnGroupResourceAcquire_PostObject( void );
	virtual void		VOnGroupResourceAcquire() override;
	virtual void		VOnGroupResourceRelease( void );
};

#endif // #ifndef _CDOORCREATOR_H_