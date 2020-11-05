////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJITEMS_H_
#define _GCOBJITEMS_H_

#include "../../GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#include "../../GCCocosInterface/GCObjSpritePhysics.h"

class CGCObjItem
: public CGCObjSpritePhysics
{
public:
	GCFACTORY_DECLARE_CREATABLECLASS( CGCObjItem );


	CGCObjItem(	void )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjItem ) )
	{}
};


#endif
