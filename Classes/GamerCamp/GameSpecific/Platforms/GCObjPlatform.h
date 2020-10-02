////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJPLATFORMS_H_
#define _GCOBJPLATFORMS_H_

#include "../../GCCocosInterface/GCObjSpritePhysics.h"

class CGCObjPlatform
: public CGCObjSpritePhysics
{
public:
	CGCObjPlatform(	void )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjPlatform ) )
	{}

	virtual void VOnResourceAcquire( void ) override;
};


#endif
