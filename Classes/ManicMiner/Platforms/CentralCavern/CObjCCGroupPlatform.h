
#ifndef	COBJECTGROUPPLATFORM_H_
#define	COBJECTGROUPPLATFORM_H_

//////////////////////////////////////////////////////////////////////////
// forward declare
class CGCObjSprite;
class CGCObjPlatform;

#ifndef BOX2D_H
#include "Box2d/Box2D.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif


class CObjCCGroupPlatform :
    public CGCObjectGroup
{

	std::string Test;

public:

	CObjCCGroupPlatform();
	virtual ~CObjCCGroupPlatform();

	// handles CPlatform
	virtual bool		VHandlesThisTypeId(GCTypeID idQueryType) override;

	// must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId() override;
	
	void VOnGroupResourceAcquire() override;

	void VOnGroupResourceAcquire_PostObject() override;

	void VOnGroupResourceRelease() override;

	// creates platforms in the proper layout to represent the Central Cavern level in the original game 
	void CreatePlatforms();

	//
	void DestroyPlatforms();
};

#endif