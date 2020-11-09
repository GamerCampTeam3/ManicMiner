//////////////////
/// Bib
//////////////////
#ifndef _CCOLLECTIBLESGROUP_H_
#define _CCOLLECTIBLESGROUP_H_

#ifndef	_GCOBJECTGROUP_H_
#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif

#include "ManicMiner/Enums/ECollectibleTypeRequired.h"

// Forward class declaration
class CManicLayer;

class CCollectiblesGroup
: public CGCObjectGroup
{
private:
	// destroy managed items
	void DestroyItems( void );

	// Collectibles logic ints
	int m_iCollectibles;		// Current collectibles count
	int m_iSwitches;			// Current switches flipped
	int m_iMaxCollectibles;		// Maximum collectibles required
	int m_iMaxSwitches;			// Maximum switches flipped required
	int m_iScore;				// The Current score TODO: expand on this

	
	ECollectibleTypeRequired m_eCollectibleTypeRequired; // Depending on the level, set the required type of collectible to move to next level
	CManicLayer*			 m_pcManicLayer;			 // Pointer to the layer that instantiates this object

protected:
	virtual void GenerateCollectibles();	// this will be overriden by class that inherits from it.

public:
	// Constructor that takes in multiple params
	// CManicLayer: reference to the layer that instantiates this object
	// ECollectibleTypeRequired: Set when instantiating this object, dictates if it requires Switches, collectibles or both
	// numCollectibles: How much collectibles are required before moving in TODO: Add numSwitches for future levels
	CCollectiblesGroup( CManicLayer& cLayer, ECollectibleTypeRequired typeCollectibles, int numCollectibles );

	// Virtual Dtor
	virtual ~CCollectiblesGroup();


	void CollectibleEvent();				// Called by collectibles when colliding
	void SwitchEvent();						// Called by switches when colliding
	void CheckIfEnoughToOpenExit();			// Called by previous events to check if exit should be open

		
	// Score functions
	// TODO: Expand on this in M2
	int  GetScore() { return m_iScore; }
	void AddScore() { m_iScore += 100; }
	
	//////////////////////////////////////////////////////////////////////////
	// overrides for CGCObjectGroup public interface
	// handles Collectibles
	virtual bool		VHandlesThisTypeId( GCTypeID idQueryType );
	virtual GCTypeID	VGetTypeId( void );
	virtual void		VOnGroupResourceAcquire_PostObject( void );
	virtual void		VOnGroupResourceAcquire() override;
	virtual void		VOnGroupResourceRelease( void );
	// CGCObjectGroup public interface
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
};

#endif // #ifndef _CCOLLECTIBLESGROUP_H_