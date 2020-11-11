////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CREGULARPLATFORM_H_
#define _CREGULARPLATFORM_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

#include "CPlatform.h"



class CRegularPlatform : public CPlatform
{
public:

	CRegularPlatform();

	GCFACTORY_DECLARE_CREATABLECLASS(CRegularPlatform);
	
	void VOnResourceAcquire() override;

	void VOnReset() override;

};

#endif
