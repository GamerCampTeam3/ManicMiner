////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CGROUNDPLATFORM_H_
#include "CGroundPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

#ifndef TINYXML2_INCLUDED
#include "external\tinyxml2\tinyxml2.h"
#endif




#ifndef _GCLEVELLOADER_OGMO_H_
#include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

GCFACTORY_IMPLEMENT_CREATEABLECLASS(CGroundPlatform);

CGroundPlatform::CGroundPlatform()
	: CPlatform()
{
	m_ePlatformType = EPlatformType::Ground;
}


void CGroundPlatform::VOnResourceAcquire()
{	
	CPlatform::VOnResourceAcquire();
}

void CGroundPlatform::VOnReset()
{
	CPlatform::VOnReset();
}
