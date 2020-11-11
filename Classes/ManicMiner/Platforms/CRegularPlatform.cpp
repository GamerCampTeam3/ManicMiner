////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CREGULARPLATFORM_H_
#include "CRegularPlatform.h"
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

GCFACTORY_IMPLEMENT_CREATEABLECLASS(CRegularPlatform);

CRegularPlatform::CRegularPlatform()
	: CPlatform()
{
	m_ePlatformType = EPlatformType::Regular;
}

void CRegularPlatform::VOnResourceAcquire()
{
	CPlatform::VOnResourceAcquire();
}

void CRegularPlatform::VOnReset()
{
	CPlatform::VOnReset();
}