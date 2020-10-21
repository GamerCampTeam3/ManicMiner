#ifndef _CDOORCREATOR_H_
#define _CDOORCREATOR_H_

#include "GamerCamp/GCObject/GCObjectGroup.h"
#include "ManicMiner/Doors/CDoor.h"

class CDoorCreator
	: public CGCObjectGroup
{
protected:
	CManicLayer* m_rManicLayer;

	virtual void CreateDoor();
	
public:
	CDoorCreator( CManicLayer& cLayer)
		: m_rManicLayer( nullptr )
	{		
	}

	virtual void DestroyItems( void );

	virtual bool		VHandlesThisTypeId( GCTypeID idQueryType );

	// must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId( void );
	virtual void		VOnGroupResourceAcquire_PostObject( void );
	virtual void		VOnGroupResourceAcquire() override;
	virtual void		VOnGroupResourceRelease( void );
};


#endif // #ifndef _CDOORCREATOR_H_
