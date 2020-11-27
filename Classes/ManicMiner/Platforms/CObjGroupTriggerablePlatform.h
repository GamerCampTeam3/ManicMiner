#ifndef	_COBJGROUPTRIGGERABLEPLATFORM_H_
#define	_COBJGROUPTRIGGERABLEPLATFORM_H_


#ifndef	_GCOBJECTGROUP_H_
#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif

class CObjGroupTriggerablePlatform
    : public CGCObjectGroup
{
private:

    void CreateTriggerablePlatform();

public:

    CObjGroupTriggerablePlatform();
    virtual ~CObjGroupTriggerablePlatform() override;

    virtual void VOnGroupReset() override;

    virtual void VOn
  
};

#endif
