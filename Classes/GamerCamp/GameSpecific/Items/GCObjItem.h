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
public:

	CGCObjItem(	void )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjItem ) )
	{}

	
	virtual void VOnResourceAcquire( void ) override;
};


#endif
