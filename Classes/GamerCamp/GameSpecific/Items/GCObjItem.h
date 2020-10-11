////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJITEMS_H_
#define _GCOBJITEMS_H_

#include "../../GCCocosInterface/GCObjSpritePhysics.h"

class CGCObjItem
: public CGCObjSpritePhysics
{
private:
	bool m_bHasBeenCollected;
public:

	CGCObjItem(	void )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjItem ) )
		, m_bHasBeenCollected(false)
	{}

	virtual void VOnResourceAcquire( void ) override;

	void Collected() { m_bHasBeenCollected = true; }
	bool GetHasBeenCollected() { return m_bHasBeenCollected; }
};


#endif
