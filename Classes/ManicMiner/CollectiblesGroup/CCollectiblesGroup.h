#ifndef _CCOLLECTIBLESGROUP_H_
#define _CCOLLECTIBLESGROUP_H_

#ifndef	_GCOBJECTGROUP_H_
#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif

#include "ManicMiner/Enums/ECollectibleTypeRequired.h"

class CCollectiblesGroup
: public CGCObjectGroup
{
	// destroy managed items
	void DestroyItems( void );
	
	int m_iCollectibles;
	int m_iSwitches;
	int m_iMaxCollectibles;
	int m_iMaxSwitches;

	ECollectibleTypeRequired m_eCollectibleTypeRequired;

public:

	CCollectiblesGroup();
	CCollectiblesGroup( ECollectibleTypeRequired typeCollectibles );
	CCollectiblesGroup( ECollectibleTypeRequired typeCollectibles, int numCollectibles );
	virtual ~CCollectiblesGroup();


	void CollectibleEvent();
	void SwitchEvent();
	bool CheckIfEnoughToOpenExit();

	void SetRequiredType( ECollectibleTypeRequired collectibleRequired ) { m_eCollectibleTypeRequired = collectibleRequired; };
	
	int GetCollectible() { return m_iCollectibles; };
	void SetCollectibles( int collectibles ) { m_iCollectibles = collectibles; };
	
	int GetSwitches() { return m_iSwitches; };
	void SetSwitches( int switches ) { m_iSwitches = switches; };
	
	//////////////////////////////////////////////////////////////////////////
	// overrides for CGCObjectGroup public interface
	// handles GCObjItem
	virtual bool		VHandlesThisTypeId( GCTypeID idQueryType );

	// must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId( void );
	virtual void		VOnGroupResourceAcquire_PostObject( void );
	virtual void		VOnGroupResourceRelease( void );
	// CGCObjectGroup public interface
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
};

#endif // #ifndef _CCOLLECTIBLESGROUP_H_